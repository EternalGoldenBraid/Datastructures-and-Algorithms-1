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
    towns_added = {};

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
                                    .coord=coord,.tax=tax};
    bool is_added = (towns.emplace(new_town.town_id, new_town)).second;
    if (is_added) {
        towns_added.emplace(new_town.town_id);

        // DEBUG FOUND TOWNS
        std::cout << "Awaiting alpha: " << std::endl;
        for ( auto f : towns_added ){
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

        // DEBUG
        std::cout << "Towns missing from alpha" << std::endl;
        for (auto a : towns_added) {
            std::cout << a << std::endl;
        }

        // END DEBUG
    if (!towns_added.empty()) {
        towns_alpha_sorted.resize(towns_alpha_sorted.size()+towns_added.size());
        auto itr = towns_added.begin();
        while (itr != towns_added.end()) {
            towns_alpha_sorted.emplace_back(*itr);
            itr++;
        }
        auto comp = [&](TownID a, TownID b){ 
                return towns.at(a).name > towns.at(b).name;
        };
        std::sort(towns_alpha_sorted.begin(),towns_alpha_sorted.end(),
                comp);
        // DEBUG
        std::cout << "Towns alpha: " << std::endl;
        for (auto a : towns_alpha_sorted) {
            std::cout << a << std::endl;
        }
        // END DEBUG
        
        //std::vector<TownID> ids;
        //ids.reserve(towns.size());
        //std::copy(towns_alpha_sorted.begin(), towns_alpha_sorted.end(),ids.begin());
        //towns_added.clear();
    }
    return towns_alpha_sorted;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Replace the line below with your implementation
    throw NotImplemented("towns_distance_increasing()");
}

TownID Datastructures::min_distance()
{
    // Replace the line below with your implementation
    throw NotImplemented("min_distance()");
}

TownID Datastructures::max_distance()
{
    // Replace the line below with your implementation
    throw NotImplemented("max_distance()");
}

bool Datastructures::add_vassalship(TownID /*vassalid*/, TownID /*masterid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_vassalship()");
}

std::vector<TownID> Datastructures::get_town_vassals(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("get_town_vassals()");
}

std::vector<TownID> Datastructures::taxer_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("taxer_path()");
}

bool Datastructures::remove_town(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_town()");
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
