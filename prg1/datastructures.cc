// Datastructures.cc
//
// Student name: Nicklas Fianda
// Student email: nicklas.fianda@tuni.fi
// Student number: 296255

#include "datastructures.hh"

#include <random>

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <memory>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
    towns = {};
    towns_added_alpha = {};
    towns_alpha_sorted = {};

    towns_added_dist = {};
    towns_dist_sorted = {};

    size_t known_depth = 0;
}

Datastructures::~Datastructures() {
    // Write any cleanup you need here
      
    for ( auto p: towns ) {
        Town t = p.second;
        delete(t.master);
        for ( Town *v : t.vassals ) {
            delete(v);
        }
    }
     
}

unsigned int Datastructures::town_count()
{
    return towns.size();
}

void Datastructures::clear_all()
{
    towns.clear();
}

bool Datastructures::add_town(TownID id, const Name &name,
   				 Coord coord, int tax)
{
    Datastructures::Town new_town = {
            .town_id=id,.name=name,
            .coord=coord,.tax=tax,
            //.master=nullptr, 
            .master=new Town(), 
            .vassals=std::unordered_set<Town*,IdHash_ptr>{},
            .vassals_id=std::vector<TownID>{}
    };


        // DEBUG
        std::cout << "NEW TOWN: " << id << std::endl;
        if ( towns.find(id) == towns.end() ) {
            std::cout << "Already exist!" << std::endl;
        } else {
            std::cout << "Does not already exist in " 
                    << towns.size() << " number of towns."<< std::endl;
            std::cout << "Towns:  ";
            for (auto t : towns) {
                std::cout << t.first<< ", ";
            }
                std::cout <<std::endl;
        }
         // END DEBUG

    //bool is_added = (towns.try_emplace(new_town.town_id, new_town)).second;
    bool is_added = (towns.insert({new_town.town_id, new_town})).second;
    if (is_added) {

        //// DEBUG
        std::cout << "ADDING TO sort buffer: " << id << std::endl;
        std::cout << "buffer empty?: " << towns_added_alpha.empty() << std::endl;
        std::cout << "buffer contents: ";
        for ( auto t : towns_added_alpha ) {
            std::cout << t << ", ";
        }
        std::cout << std::endl;
        std::cout << "is_added to towns: " << is_added << std::endl;
        std::cout << "" << std::endl;

        // END DEBUG
          
        towns_added_alpha.emplace_back(new_town.town_id);
        //towns_added_dist.emplace_back(new_town.town_id);
    }

    return is_added;
    

}

Name Datastructures::get_town_name(TownID id)
{
    Name name;
    try {
        name = towns.at(id).name;
    } 
    catch(std::out_of_range& e) {
        return NO_NAME;
    }
    
    return name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    Coord coord;
    try {
        coord = towns.at(id).coord;
    } 
    catch(std::out_of_range& e) {
        return NO_COORD;
    }
    return coord;
}

int Datastructures::get_town_tax(TownID id)
{
    int tax;
    try {
        tax = towns.at(id).tax;
    } 
    catch(std::out_of_range& e) {
        return NO_VALUE;
    }
    return tax;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> ids;
    ids.reserve(towns.size());
    for (const auto &[id, _]:towns) ids.push_back(id);
    return ids;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> found = {};

    //auto it = std::find_if(towns.begin(), towns.end(), 
            //[&name](auto t){ t.name==name};
            //);

    for ( auto &town:towns) {
        if (town.second.name == name){
            found.push_back(town.second.town_id);
        }
    }
    return found;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    try {
        (&towns.at(id))->name = newname;
    }
    catch(std::out_of_range &e) {return false;}
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{

    if (!towns_added_alpha.empty()) {

        auto comp = [&](TownID a, TownID b){ 
                //std::cout << a <<", " << b  << std::endl;
                return towns.at(a).name < towns.at(b).name;
        };

        try{
            std::sort(towns_added_alpha.begin(),towns_added_alpha.end(),
                    comp);
        }
        catch(std::out_of_range &e) {

            // DEBUG
            std::cout << "RESERVE FUCK" << std::endl;
            // END DEBUG
        }
        
        // Merge the two sorted arrays.
        size_t alpha_sz = towns_alpha_sorted.size();
        size_t added_sz = towns_added_alpha.size();
        towns_alpha_sorted.reserve(alpha_sz+added_sz);

        auto middle = towns_alpha_sorted.end();
        towns_alpha_sorted.insert(towns_alpha_sorted.end(), 
                towns_added_alpha.begin(), towns_added_alpha.end());
        std::inplace_merge(towns_alpha_sorted.begin(), middle, 
                towns_alpha_sorted.end());
        
        towns_added_alpha.clear();
    }

    return towns_alpha_sorted;
}

Distance Datastructures::dist(TownID &a){

    using namespace std;
    auto coord_a = towns.at(a).coord;
    auto dist_a = sqrt(pow(coord_a.x,2) + pow(coord_a.y,2));
    return dist_a;

}

void Datastructures::sort_by_distance() {

    
    auto comp = [&](TownID a, TownID b){ 
            auto dist_a = dist(a);
            auto dist_b = dist(b);
            return dist_a < dist_b;
    };
    
    // DEBUG
    try{
    std::sort(towns_added_dist.begin(),towns_added_dist.end(),
            comp);
    }
    catch(std::out_of_range &e) {
            std::cout << "RESERVE FUCK" << std::endl;
        }
    // END DEBUG
      
    size_t dist_sz = towns_dist_sorted.size();
    size_t added_sz = towns_added_dist.size();
    towns_dist_sorted.reserve(dist_sz+added_sz);

    auto middle = towns_dist_sorted.end();
    towns_dist_sorted.insert(towns_dist_sorted.end(), 
            towns_added_dist.begin(), towns_added_dist.end());
    std::inplace_merge(towns_dist_sorted.begin(), middle, 
            towns_dist_sorted.end());
    
    towns_added_dist.clear();

}
std::vector<TownID> Datastructures::towns_distance_increasing()
{
    if (!towns_added_dist.empty()) {
        sort_by_distance();
    }
    return towns_dist_sorted;
}

TownID Datastructures::min_distance()
{
    if (!towns_added_dist.empty()) {
        sort_by_distance();
    }
    return towns_dist_sorted.front();
}

TownID Datastructures::max_distance()
{
    if (!towns_added_dist.empty()) {
        sort_by_distance();
    }
    return towns_dist_sorted.back();
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    // DEBUG
    std::cout << "ADDING VASSALS" << std::endl;
    std::cout << "Contents of town: " << std::endl;
    for ( auto t: towns ) {
        std::cout << t.first;
    }
        std::cout << std::endl;
    
    // END DEBUG
    try {
        //auto vassal_ptr = new Town(towns.at(vassalid));
          
        auto vassal_ptr = &towns.at(vassalid);
        if ( vassal_ptr->master->town_id != TownID("") ) {
            //delete(vassal_ptr);
            return false;
        }

        //auto master_ptr = new Town(towns.at(masterid));
          
        auto master_ptr = &towns.at(masterid);
        vassal_ptr->master = master_ptr;
        master_ptr->vassals.emplace(vassal_ptr);

        master_ptr->vassals_id.emplace_back(vassal_ptr->town_id);
    } 
    catch(std::out_of_range &e) {
        // DEBUG
        std::cout << "ADDING VASSALS FAILED" << std::endl;
        std::cout << "Contents of towns: " << std::endl;
        for ( auto t: towns ) {
            std::cout << t.first;
        }
            std::cout << std::endl;
        // END DEBUG
        return false;
    }
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    try {

        return towns.at(id).vassals_id;
        
    } 
    catch(std::out_of_range &e) {
        return std::vector<TownID>{NO_NAME};
    }
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    try {

        auto master = towns.at(id).master;
        
        std::vector<TownID> path = {towns.at(id).town_id};
        path.reserve(known_depth+1);

        size_t k = 0;
        while ( master != nullptr ){
            k++;
            path.push_back(master->town_id);

            if ( master->master ) {
                master = master->master;
            } else {
                master = nullptr;
            }
        }

        if (known_depth < k) known_depth = k;

        return path;
        
    } 
    catch(std::out_of_range &e) {
        return std::vector<TownID>{NO_NAME};
    }
}

bool Datastructures::remove_town(TownID id)
{
    std::cout << "REMOVING TOWNS!!!!!!" << std::endl;
    try {
        Town *town = &towns.at(id);
        Town *master = town->master;
        auto *vassals = &town->vassals;

        // Set master of vassals to master of town(id).
        auto itr = vassals->begin();
        while (itr != vassals->end()) 
        {
            (*itr)->master = master;
            itr++;
        }

        // If town(id) has master, set vassals of town to vassals
        // of master.
        if ( town->master != nullptr ) {

            size_t vs_size = vassals->size();
            size_t master_vs_size = master->vassals.size();

            master->vassals.erase(&towns.at(id));
            master->vassals.reserve(master_vs_size+vs_size);
            master->vassals.merge(*vassals);

            master->vassals_id.reserve(master_vs_size+vs_size);
            auto it_begin = master->vassals_id.begin();
            auto it_end = master->vassals_id.end();
            master->vassals_id.insert(it_end, 
                    town->vassals_id.begin(), town->vassals_id.end());

            auto it = std::remove_if(it_begin, it_end, 
                    [&](auto id_){return id_==id;});
            master->vassals_id.erase(it);
            //master->vassals_id.shrink_to_fit();
        }


        auto itr_alpha = std::remove_if(towns_alpha_sorted.begin(), towns_alpha_sorted.end(), 
                [&](auto id_){return id_==id;});
        towns_alpha_sorted.erase(itr_alpha);
        town->master = nullptr;
        towns.erase(id);
        return true;
        
    } 
    catch(std::out_of_range &e) {
        return false;
    }

}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("towns_nearest()");
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("total_net_tax()");
}
