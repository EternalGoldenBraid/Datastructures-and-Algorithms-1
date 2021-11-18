As part of the assignment submission, a document should be added to git (in the same 
directory/folder as the code). This document should contain reasons for choosing the 
data structures used in the assignment (especially performance reasons). Acceptable 
formats are plain text (readme.txt), markdown (readme.md), and Pdf (readme.pdf).

# Justification for choice of datastructures used in the assignment.

The principal datastructures I use are: 

### Town struct
I decided to use a struct to maintain information about towns and their states 
due to its convenience and ease of use.
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

### std::unordered_map<TownID, Town> towns
I went for the unordered_map due to the delicious average constant time accessing 
and adding of keys. In hindsight or if I were to do this assignment again,
I would use a vector for the sequential memory allocation with the cost of 
potentially more complex code. Atleast based on my memories of the course material

    
### std::vector<TownID> towns_alpha_sorted;
Container of towns ids sorted by town names.
I decided to implement a semi-duplicate container for the id's of the towns.
I justified this by the appeal of serving constant time access to alphabetically 
sorted towns. Constant access is guaranteed when no new towns are added in between
calls of this function.

### std::unordered_set<TownID> towns_added_alpha;
Temporary container (buffer) for towns not yet sorted.
This container is used when new towns are added in between towns_alpha_sorted calls.
When towns_added_alpha is called I check whether this buffer contains any elements.
If it does, I sort the buffer and use in_place merge

    // Container of towns ids sorted by town distance from origin.
    std::vector<TownID> towns_dist_sorted;
    // Temporary container for towns not yet sorted.
    std::unordered_set<TownID> towns_added_dist;

    // A hint for the longest path of master-vassal relationships
    // used for reserving memory for relevant vectors.
    size_t known_depth;
