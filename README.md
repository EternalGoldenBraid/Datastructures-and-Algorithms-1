As part of the assignment submission, a document should be added to git (in the same 
directory/folder as the code). This document should contain reasons for choosing the 
data structures used in the assignment (especially performance reasons). Acceptable 
formats are plain text (readme.txt), markdown (readme.md), and Pdf (readme.pdf).

# Justification for choice of datastructures used in the assignment.

The principal datastructures I use are: 

### Town struct
I decided to use a struct to maintain information about towns and their states 
due to its convenience and ease of use.
```
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

```
``` std::unordered_map<TownID, Town> towns ```
I went for the unordered_map due to the delicious average constant time accessing 
and adding of keys. In hindsight or if I were to do this assignment again,
I would use a vector for the sequential memory allocation with the cost of 
potentially more complex code. Atleast based on my memories of the course material
the indexing scheme seems potentially challenging N to 1 connections between nodes.

    
``` std::vector<TownID> towns_alpha_sorted; and std::vector<TownID> towns_dist_sorted; ```
Container of towns ids sorted by town names.
I decided to implement a semi-duplicate container for the id's of the towns.
I justified this by the appeal of serving constant time access to alphabetically 
sorted towns. Constant access is guaranteed when no new towns are added in between
calls of these and a few other functions.

``` std::unordered_set<TownID> towns_added_alpha; and std::unordered_set<TownID> towns_added_dist; ```
Temporary containers (buffers) for towns not yet sorted.
These containers are used when new towns are added in between towns_alpha_sorted and
towns_dist_sorted (and a few others) calls.
When towns_added_alpha is called I check whether this buffer contains any elements.
If it does, I sort the buffer and use in_place merge to merge the allready sorted towns_alpha_sorted
and the sorted towns_added_alpha.
For small number of towns added in between these buffer clearing operations I think
it smoothens out the load towards a more uniform distribution.

These few other functions are:
	-``` all_towns ```
	-``` towns_nearest ```
  -``` towns_distance_increasing ```
  -``` min/max_distance ```

``` size_t known_depth; ```
A hint for the longest path of master-vassal relationships so far queried.
This seemed like an efficient way to make sure there's enough memory in cache
reserved for stl vector push_back operations.
I should probably be decaying every now and then or using an average instead of
the all time high depending on the structures of the networks.
If a small minority of paths are very long then this causes memory to be hoarded
for no performance gain.

### Repository missing components removed due to copyright issues with course materials.
