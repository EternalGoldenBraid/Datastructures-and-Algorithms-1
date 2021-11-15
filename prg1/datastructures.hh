// Datastructures.hh
//
// Student name: Nicklas Fianda
// Student email: nicklas.fianda@tuni.fi
// Student number: 296255

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: Constant time
    // Short rationale for estimate: std::unosrted_map::size() so 
    // described in cppref.
    unsigned int town_count();

    // Estimate of performance: Linear in number of towns.
    // Short rationale for estimate: So described in cppreference.com
    // for unsorted_map::clear();
    void clear_all();

    // Estimate of performance: Amortized constant on average,
    // worst case linear in the size of the container.
    // Short rationale for estimate: Linear if town already exists.
    // from cppreference for std::unordered_map::emplace
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance:  Average constant time complexity
    // for search of the town. Acessing struct Town.name is constant time.
    // Short rationale for estimate: So promised by cppref. for 
    // std::unsorted_map
    Name get_town_name(TownID id);

    // Estimate of performance:  Average constant time complexity
    // for search of the town. Acessing struct Town.coord is constant time.
    // Short rationale for estimate: So promised by cppref. for 
    // std::unsorted_map
    Coord get_town_coordinates(TownID id);

    // Estimate of performance:  Average constant time complexity
    // for search of the town. Acessing struct Town.tax is constant time.
    // Short rationale for estimate: So promised by cppref. for 
    // std::unsorted_map
    int get_town_tax(TownID id);

    // Estimate of performance: Constant
    // Short rationale for estimate: Returned stored array of ID's.
    std::vector<TownID> all_towns();

    // Estimate of performance: Average linear time complexity.
    // Short rationale for estimate: Looping through std::unsorted_map
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance:  Average constant time complexity
    // Linear in number of towns if town doesn't exist.
    // for search of the town. Acessing struct Town.name is constant time.
    // Short rationale for estimate: So promised by cppref. for 
    // std::unsorted_map
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: 
    //      Worst case nlog(n) in size of added towns IF the application is used 
    // such that many towns are added in between calls for
    // towns_alphabetically.
    // When the amount of towns added between calls is small nlog(n) is small
    // and the functions performs better.
    // Short rationale for estimate: 
    //      A vector of already sorted arrays and a buffer is maintained.
    //  The buffer is sorted on function call towns_alphabetically, and
    //  then merged with the already sorted vector of ID's. Buffer is cleared.
    //  As buffer size (number of towns added) converges to small numbers
    //  sorting becomes cheaper and cheaper.
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: Same as above.
    // Short rationale for estimate: Same as above.
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(n(log(n)) only when new towns have been added
    // before previous call.
    // Constant when array is already sorted, array is always sorted after 
    // call to towns_distance_increasing and before call to add_town.
    // Short rationale for estimate: TownID's are kept in an array sorted 
    // using std::sort() by distance.
    // Opted to sort the array instead of std::nth_element [ O(nlog(n)) vs .O(n)]
    // since accessing after the sorting is always constant time and sorted
    // array is also used by towns_distance_increasing.
    TownID min_distance();

    // Same as above.
    TownID max_distance();

    // Estimate of performance: Linear in number of towns if town does not
    // exist.
    // Average constant time if town exists.
    // Short rationale for estimate: cppreference describes average constant
    // time access to unsorted_map, and linear in size of map if element is not
    // found using .at() operator.
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: Constant time at the expense of higher memory.
    // Short rationale for estimate: Keeping a copy of TownIDs of direct
    // vassals for each town in a vector, which is returned in the function.
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: 
    // Worst case: Linear in number of towns if town doesn't exist. 
    // Best case: Linear in depth of master-vassal relations.
    // Short rationale for estimate: Masters are pushed back to a vector while
    // they exist.
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: Linear in size of vassals of town to be
    // removed. Also linear in size of number vassals of master of town
    // to be removed.
    // Short rationale for estimate: 
    // Merging to unordered_set is worst case linear in size of 
    // containers to be merged.
    // Inserting to vector (vector<TownID>) is linear in elements inserted.
    // Removing from the two buffer containers of distance and alpha
    // sorted arrays is linear in their size.
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);

private:
    // Add stuff needed for your class implementation here

    Distance dist(TownID &id);
    void sort_by_distance();

    // Forward declare to be used in hash computation.
    struct Town;
    class IdHash_ptr {
        public:
            size_t operator()(const Town *t) const{
            //size_t operator()(const std::unique_ptr<Town> &t) const{
                std::hash<std::string> hasher;

                if (not t) return 0; 
                return hasher(t->town_id);
            }
    };


    struct Town
    {
      	TownID town_id;
      	Name name;
        Coord coord;
        int tax;

        Town *master;

        std::unordered_set<Town*, IdHash_ptr> vassals;
        std::vector<TownID> vassals_id;
    };

    class IdHash {
        public:
            size_t operator()(const TownID id) const{
                std::hash<std::string> hasher;
                return hasher(id);
            }
    };
    std::unordered_map<TownID, Town, IdHash> towns;

    // Container of towns ids sorted by town names.
    std::vector<TownID> towns_alpha_sorted;
    // Temporary container for towns not yet sorted.
    std::vector<TownID> towns_added_alpha;

    // Container of towns ids sorted by town distance from origin.
    std::vector<TownID> towns_dist_sorted;
    // Temporary container for towns not yet sorted.
    std::vector<TownID> towns_added_dist;

    // A hint for the longest path of master-vassal relationships
    // used for reserving memory for relevant vectors.
    size_t known_depth;
    
    bool DEBUG_;

};

#endif // DATASTRUCTURES_HH
