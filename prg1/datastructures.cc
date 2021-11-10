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
    //std::unordered_map<TownID, Town> towns = {};
    towns = {};
    towns_added_alpha = {};
    towns_alpha_sorted = {};

    towns_added_dist = {};
    towns_dist_sorted = {};

    size_t known_depth = 0;
}

Datastructures::~Datastructures() {
    // Write any cleanup you need here
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
    Datastructures::Town new_town = {.town_id=id,.name=name,
                                    .coord=coord,.tax=tax,
                                    .master=nullptr, 
                                    .vassals=std::unordered_set<Town*,IdHash_ptr>{},
                                    .vassals_id=std::vector<TownID>{}
    };
    bool is_added = (towns.emplace(new_town.town_id, new_town)).second;
    if (is_added) {
        towns_added_alpha.emplace(new_town.town_id);
        towns_added_dist.emplace(new_town.town_id);

        // DEBUG FOUND TOWNS
        std::cout << "Awaiting alpha: " << std::endl;
        for ( auto f : towns_added_alpha ){
            std::cout << f<< std::endl;
        }
        // END DEBUG
    }
    return is_added;
    

}

Name Datastructures::get_town_name(TownID id)
{
    Name name;
    try {
        // DEBUG
        std::cout << "Attempting to find town: " << id << std::endl;
        // END DEBUG
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

    for ( auto &town:towns) {
        if (town.second.name == name){
            found.push_back(name);
        }
    }

    // DEBUG FOUND TOWNS
    std::cout << "Found towns: " << std::endl;
    for ( auto f : found){
        std::cout << f<< std::endl;
    }
    // END DEBUG
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

        size_t alpha_sz = towns_alpha_sorted.size();
        size_t added_sz = towns_added_alpha.size();
        towns_alpha_sorted.reserve(alpha_sz+added_sz);

        auto itr = towns_added_alpha.begin();
        while (itr != towns_added_alpha.end()) {
            towns_alpha_sorted.emplace_back(*itr);
            itr++;
        }
        auto comp = [&](TownID a, TownID b){ 
                return towns.at(a).name < towns.at(b).name;
        };
        std::sort(towns_alpha_sorted.begin(),towns_alpha_sorted.end(),
                comp);
        
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

    size_t dist_sz = towns_alpha_sorted.size();
    size_t added_sz = towns_added_dist.size();
    towns_dist_sorted.reserve(dist_sz+added_sz);
    
    auto itr = towns_added_dist.begin();
    while (itr != towns_added_dist.end()) {
        towns_dist_sorted.emplace_back(*itr);
        itr++;
    }
    
    auto comp = [&](TownID a, TownID b){ 
            auto dist_a = dist(a);
            auto dist_b = dist(b);
            return dist_a < dist_b;
    };
    
    
    std::sort(towns_dist_sorted.begin(),towns_dist_sorted.end(),
            comp);
    
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
    try {
        // Old attempt with uniquer pointers.
        //
        //auto vassal_ptr = std::make_unique<Town>(towns.at(vassalid));
        //auto master_ptr = std::make_unique<Town>(towns.at(masterid));
        //vassal_ptr->master = std::move(master_ptr);
        //((towns.at(masterid)).vassals).emplace(std::move(vassal_ptr));
        
        auto *vassal_ptr = &towns.at(vassalid);
        auto *master_ptr = &towns.at(masterid);
        vassal_ptr->master = master_ptr;
        master_ptr->vassals.emplace(vassal_ptr);

        //master_ptr->vassals_id.emplace(vassal_ptr->town_id);
        master_ptr->vassals_id.emplace_back(vassal_ptr->town_id);
    } 
    catch(std::out_of_range &e) {
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

        auto ptr = towns.at(id).master;
        
        std::vector<TownID> path = {towns.at(id).town_id};
        path.reserve(known_depth+1);

        size_t k = 0;
        while ( ptr != nullptr ){
            k++;
            ptr = ptr->master;
            path.push_back(ptr->town_id);
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
    try {
        Town *master = towns.at(id).master;
        auto v_begin = towns.at(id).vassals.begin();
        auto v_end = towns.at(id).vassals.end();
        if (master != nullptr and v_begin != v_end )
        {
            auto itr = v_begin;
            while (itr != v_end) 
            {
                (*itr)->master = master;
            }

            auto peer_vassals = 

        } else if ( master == nullptr and v_begin != v_end ){
            auto itr = v_begin;
            while (itr != v_end) 
            {
                (*itr)->master = nullptr;
            }
        } else if ( master != nullptr and v_begin == v_end ){
            
        }


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
