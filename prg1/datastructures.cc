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
        //for ( Town *v : t.vassals ) {
        //    delete(v);
        //}
    }
     
}

unsigned int Datastructures::town_count()
{
    return towns.size();
}

void Datastructures::clear_all()
{
    towns.clear();
    towns_alpha_sorted.clear();
    towns_added_alpha.clear();
    towns_dist_sorted.clear();
    towns_added_dist.clear();
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
    new_town.master->town_id = NULL_MASTER;

    //bool is_added = (towns.try_emplace(new_town.town_id, new_town)).second;
    bool is_added = (towns.insert({new_town.town_id, new_town})).second;
    if (is_added) {
        towns_added_alpha.emplace_back(new_town.town_id);
        towns_added_dist.emplace_back(new_town.town_id);
    }

    return is_added;
    

}

Name Datastructures::get_town_name(TownID id)
{
    auto itr = towns.find(id);
    if ( itr == towns.end() ) return NO_NAME;
    return itr->second.name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    auto itr = towns.find(id);
    if ( itr == towns.end() ) return NO_COORD;
    return itr->second.coord;
}

int Datastructures::get_town_tax(TownID id)
{
    auto itr = towns.find(id);
    if ( itr == towns.end() ) return NO_VALUE;
    return itr->second.tax;
    //return (*itr).second.tax;
}

std::vector<TownID> Datastructures::all_towns()
{
    if(!towns_added_alpha.empty()) sort_by_distance();
    return towns_dist_sorted;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> found = {};
    for ( auto &town:towns) {
        if (town.second.name == name){
            found.push_back(town.second.town_id);
        }
    }
    return found;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    auto itr = towns.find(id);
    if ( itr == towns.end() ) return false;
    itr->second.name = newname;
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{

    if (!towns_added_alpha.empty()) {

        auto comp = [&](TownID a, TownID b){ 
                //std::cout << a <<", " << b  << std::endl;
                return towns.at(a).name < towns.at(b).name;
        };
        std::sort(towns_added_alpha.begin(),towns_added_alpha.end(),
                comp);
        
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

//Distance Datastructures::dist(TownID &a,static const Coord &coord){
Distance Datastructures::dist(TownID &a,Coord &coord){

    using namespace std;
    auto coord_a = towns.at(a).coord;
    auto dist_a = sqrt(pow(coord_a.x-coord.x,2) + pow(coord_a.y-coord.y,2));
    return dist_a;

}

void Datastructures::sort_by_distance() {

    
    auto comp = [&](TownID a, TownID b){ 
            auto dist_a = dist(a, default_coord);
            auto dist_b = dist(b, default_coord);
            return dist_a < dist_b;
    };
    std::sort(towns_added_dist.begin(),towns_added_dist.end(),
            comp);
      
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
    if (towns_dist_sorted.empty()) {
        sort_by_distance();
    }else if (!towns_added_dist.empty()) {
        sort_by_distance();
    }
    return towns_dist_sorted.front();
}

TownID Datastructures::max_distance()
{
    if (towns_dist_sorted.empty()) {
        sort_by_distance();
    }else if (!towns_added_dist.empty()) {
        sort_by_distance();
    }
    return towns_dist_sorted.back();
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    auto vassal_ptr = towns.find(vassalid);
    if ( vassal_ptr == towns.end() or 
            vassal_ptr->second.master->town_id != NULL_MASTER ) {
        return false;
    }
    
    auto master_ptr = towns.find(masterid);
    if ( master_ptr == towns.end() ) return false;

    vassal_ptr->second.master = &(master_ptr->second);
    master_ptr->second.vassals.emplace(&(vassal_ptr->second));
    
    master_ptr->second.vassals_id.emplace_back(
            vassal_ptr->second.town_id);
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    auto itr = towns.find(id);
    if ( itr == towns.end() ) 
        return std::vector<TownID>{NO_TOWNID};
    return itr->second.vassals_id;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    auto itr = towns.find(id);
    if ( itr == towns.end() ) return std::vector<TownID>{NO_NAME};

    //auto master = itr->second.master;
    
    //std::vector<TownID> path = {towns.at(id).town_id};
    std::vector<TownID> path;

    // Use previous estimates of longest path length to 
    // reserve memory for depth estimate.
    path.reserve(known_depth+1);

    size_t k = 0;
    Town *current_town = &(itr->second);
    while ( current_town->master != nullptr ){
        k++;
        path.push_back(current_town->town_id);

        if ( current_town->master ) {
            current_town = current_town->master;
        }
    }

    if (known_depth < k) known_depth = k;

    return path;
}

bool Datastructures::remove_town(TownID id)
{
    auto it = towns.find(id);
    if ( it == towns.end() ) return false;

    Town *town = &it->second;
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
        master->vassals_id.shrink_to_fit();
    }


    auto itr_alpha = std::remove_if(
            towns_alpha_sorted.begin(), towns_alpha_sorted.end(), 
            [&](auto id_){return id_==id;});
    towns_alpha_sorted.erase(itr_alpha);

    auto itr_dist = std::remove_if(
            towns_dist_sorted.begin(), towns_dist_sorted.end(), 
            [&](auto id_){return id_==id;});
    towns_dist_sorted.erase(itr_dist);

    towns.erase(id);
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    auto comp = [&](TownID a, TownID b){ 
            auto dist_a = dist(a, coord);
            auto dist_b = dist(b, coord);
            return dist_a < dist_b;
    };

    // Add unprocessed data from buffer if buffer not empty.
    if (!towns_added_alpha.empty()) sort_by_distance();

    std::vector<TownID> temp(
            towns_alpha_sorted.begin(), towns_alpha_sorted.end()
            );
    std::sort(temp.begin(),temp.end(),comp);

    return temp;
}


void Datastructures::count_path(TownID &master, TownID &id,
        std::vector<TownID> &path,
        unsigned int depth, std::pair<TownID, unsigned int> &max_depth) {

    auto vassals = towns.at(id).vassals_id;
    auto itr = vassals.begin();

    if (vassals.empty()) return;

    depth++;
    if ( max_depth.second < depth ) {
        max_depth.first = *itr;
        max_depth.second = depth;
    }

    while (itr != vassals.end()) {
        count_path(master, (*itr), path, depth, max_depth);
        itr++;
    }

    if ( towns.at(*(itr-1)).master->town_id == master ) {
        path.reserve(max_depth.second);
        auto id_depth = &max_depth.first;
        while ( *id_depth != master ) {
            path.push_back(*id_depth);
            id_depth = &(towns.at(*id_depth).master->town_id);
        }
        path.push_back(*id_depth);

        // Update global information about longest depth for memory 
        // reservation purposes.
        // Is this a potential memory hoag?
        known_depth = max_depth.second;
    }
}


std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    std::vector<TownID> path;
    if ( towns.find(id) == towns.end() ) {
        path.push_back(NO_TOWNID);
        return path;
    }
    unsigned int depth = 0;
    std::pair<TownID, unsigned int> max_depth;
    max_depth.second = 0;

    count_path(id, id, path, depth, max_depth);
    std::vector<TownID> flipped(path.rbegin(), path.rend());
    return flipped;
}

int Datastructures::vassal_taxes(Town &town, unsigned int sum=0) {

    auto vassals = town.vassals_id;
    auto itr = vassals.begin();

    if (vassals.empty()) return town.tax;

    while (itr != vassals.end()) {
        sum += int(0.1*vassal_taxes(towns.at(*itr), sum));
        std::cout << "Visiting" << std::endl;
        itr++;
    }
    return sum + town.tax;
}

int Datastructures::total_net_tax(TownID id)
{
    auto town = towns.find(id);
    if ( town == towns.end() ) return NO_VALUE;
    if ( town->second.vassals.empty() ) {

        if (town->second.master->town_id == NULL_MASTER) {
            return town->second.tax;
        } else
        {
            return int(0.9*town->second.tax);
        }
    } 
    if (town->second.master->town_id == NULL_MASTER) {
        return int(vassal_taxes(town->second));
    }
    return int(0.9*vassal_taxes(town->second));
}
