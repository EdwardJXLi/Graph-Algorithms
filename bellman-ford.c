// Bellman Ford Pathfinding Algorithm

// Import Libraries
#include <stdio.h>
#include <stdlib.h>

// Define Constants
#define INFINITY 2147483647
#define NEG_INFINITY -2147483647
#define true 1
#define false 0

// Decide on weather to compile main (Used as library or used in standalone)
// !!Comment out if used as library to prevent main redefinition!! 
#define _DEFMAIN


// Printer Function for printing all values in a distance array
void printDistances(int *dist, int size){
    printf("Vertex with Distance from Source\n");
    for(int node = 0; node < size; node++){
        printf("%d -> %d\n", node, dist[node]);
    }
}


// Function to get the shortest valid node (Similar To One Used In Dijkstras)
int shortestNode(int *dist, int *checked, int size){
    int min = INFINITY;
    int minIndex;
    // Loop through every node
    for(int i = 0; i < size; i++){
        // Check if its the new smallest node and it has not already been checked off.
        if(dist[i] < min && checked[i] == false){
            min = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}


// The meat of the the algorithm
// Takes In:
// map -> 2d matrix of distances between nodes
// cMap -> 2d matrix of Connectivity Booleans
// size -> Size of 2d matrix (Number of nodes)
// start -> Starting Node
int * bellmanFord(int *map, int *cMap, int size, int start){
    // Define Relax Order Cache
    // NOTE: This is really hacky but it does solve a few very nasty bugs and slightly improves O(avg)
    // I am pretty sure this is not the most elegant solution, but it works!!!
    // WHAT IT DOES:
    // This Array Saves The Order In Which The Nodes Are Calculated In.
    // This Increases O(avg) (As It Saves The Previous Calculations And Reuses Those), But It Fixes One Major Problem!
    // If the value of one node surpasses another node mid-calculation
    // The order in which the matrix is travelled gets changed
    // Breaking the algorithm by throwing it off track.
    // Relax Order Cache saves the original order and enforces it.
    int * relaxOrderCache = (int*)malloc(sizeof(int) * size);
    int relaxOrderCacheSet = false; //Flag dictating if relaxOrderCache has been set

    // Distance Array -> Contains Values Of Current Smallest Distances
    int * dist = (int*)malloc(sizeof(int) * size);
    // Set all to infinity
    for(int i = 0; i < size; i++){dist[i] = INFINITY;}

    // Checked Array -> Indicates What Nodes Have Already Been Checked
    int * checked = (int*)malloc(sizeof(int) * size);
    // NOTE: VALUES IN CHECKED ARRAY GETS INITIATED LATER!

    // Set the distance of the starting node to 0
    dist[start] = 0;

    // PART 1 -> GENERATING DISTANCE ARRAY VALUES
    // Loop Through size-1 Times To Make Sure All Values Propagate Throught The Matrix
    // (Algorithm Claims That Thats The Minimum Amount Of Times For It To Work)
    for(int j = 0; j < size-1; j++){
        // Changed Flag. Indicates If Any Values Were Changed
        // (Improves O(avg) By Removing Useless Loops)
        int changed = false;
        // Initiate Checked Array - Set all values in checked to zero
        for(int i = 0; i < size; i++){checked[i] = 0;}
        // Loop Through All Nodes
        for(int cout = 0; cout < size; cout++){
            int from;
            // Check If Order Cache Set Has Been Generates
            // If Not, Generate New Order Cache Set
            // If So, Use Order Cache Set
            if(!relaxOrderCacheSet){
                // Find Shortest Node Meathod (Used In Dijkstras)
                from = shortestNode(dist, checked, size);
                relaxOrderCache[cout] = from;
            }
            else{
                from = relaxOrderCache[cout];
            }
            // Go Through Each Node, Checking Connections
            checked[from] = true;
            for(int to = 0; to < size; to++){
                // For navigating 2d arrays, using `(from*size) + to`
                // AKA `(rows*size + column)`

                // Checking If:
                // The Nodes Are Connected (using cMap)
                // The New Node Pair Are The Smallest Value
                if(cMap[(from*size) + to] == true && dist[to] > dist[from] + map[(from*size) + to]){
                    // Change Distance To New Shortest Value
                    dist[to] = dist[from] + map[(from*size) + to];
                    // Set Changed Flag
                    changed = true;
                }
            }
        }
        // Set Order Cache Set Flag (Only Makes Difference First Time)
        relaxOrderCacheSet = true;

        // End Loop If Nothing Gets Changed
        if(!changed){
            break;
        }
    }

    // PART 2 -> DETECTING NEGATIVE CYCLES
    // Similar Idea To PART 1, But Instead Of Changing The Value Of Shortest Node To Value, Set To Negative Infinity
    // Because There Should Not Be Any More Changes After Part 1
    for(int j = 0; j < size-1; j++){
        // Changed Flag. Indicates If Any Values Were Changed
        // (Improves O(avg) By Removing Useless Loops)
        int changed = false;
        // Initiate Checked Array - Set all values in checked to zero
        for(int i = 0; i < size; i++){checked[i] = 0;}
        // Loop Through All Nodes
        for(int cout = 0; cout < size; cout++){
            // Order Cache SHOULD be already generated at this point!
            int from = relaxOrderCache[cout];
            checked[from] = true;
            // Go Through Each Node, Checking Connections
            for(int to = 0; to < size; to++){
                // For navigating 2d arrays, using "(from*size) + to"
                // AKA "(rows*size + column)"
                
                // Checking If:
                // The Nodes Are Connected (using cMap)
                // The New Node Pair Are The Smallest Value
                if(cMap[(from*size) + to] == true && dist[to] > dist[from] + map[(from*size) + to]){
                    // Set The Negative Infinity Because It Is A Negative Cycle
                    dist[to] = NEG_INFINITY;
                    // Set Changed Flag
                    changed = true;
                }
                // Checking If:
                // The Nodes Are Connected (using cMap)
                // The From Node Is Negative Infinity
                else if(cMap[(from*size) + to] == true && dist[from] == NEG_INFINITY){
                    // Set The Negative Infinity Because It is reachable from a Negative Cycle
                    dist[to] = NEG_INFINITY;
                    // Set Changed Flag
                    changed = true;
                }
            }
        }

        // End Loop If Nothing Gets Changed
        if(!changed) break;
    }

    // Free Allocated Memory (relaxOrderCache and checked array)
    free(relaxOrderCache);
    free(checked);

    // Return Distance Array Pointer
    return dist;
}


#ifdef _DEFMAIN  // Check if main should be compiled
int main(){

    // Test Values
    int map[10][10] = {
        {0, 5, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 20, 0, 0, 30, 60, 0, 0, 0}, 
        {0, 0, 0, 10, 75, 0, 0, 0, 0, 0}, 
        {0, 0, -15, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 100}, 
        {0, 0, 0, 0, 25, 0, 5, 0, 50, 0}, 
        {0, 0, 0, 0, 0, 0, 0, -50, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, -10, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    };

    int cMap[10][10] = {
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 1, 0, 0, 1, 1, 0, 0, 0}, 
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0}, 
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {0, 0, 0, 0, 1, 0, 1, 0, 1, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    };

    // Run Bellman Ford And Get Result
    int * result = bellmanFord(&map[0][0], &cMap[0][0], 10, 0);

    // Printing Final Values
    printDistances(result, 10);

    // Free result
    free(result);

    // Expected Output:
    /*
    Vertex tDistance from Source
    0 t 0
    1 t 5
    2 t -2147483647
    3 t -2147483647
    4 t -2147483647
    5 t 35
    6 t 40
    7 t -10
    8 t -20
    9 t -2147483647
    */
}
#endif  //_DEFMAIN
