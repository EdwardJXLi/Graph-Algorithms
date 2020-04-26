//Dijkstra's Pathfinding Algorithm (Matrix Edition)

#include <stdio.h>
#include <stdlib.h>
#define INFINITY 2147483647
#define NEG_INFINITY -2147483647
#define true 1
#define false 0

//Debug Printer for printing all values in distance array
void debugPrint(int *dist, int size){
    printf("Vertex tDistance from Source\n");
    for(int node = 0; node < size; node++){
        printf("%d t %d\n", node, dist[node]);
    }
}

//Function to get the shortest valid node
int shortestNode(int *dist, int *checked, int size){
    int min = INFINITY;
    int minIndex;
    //Loop through every node
    for(int i = 0; i < size; i++){
        //Check if its the new smallest node and it has not already been checked off.
        if(dist[i] < min && checked[i] == false){
            min = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

//The meat of the the algorithm
//Takes In:
//map -> Pointer of 2d matrix of distances between nodes
//cMap -> Pointer of 2d matrix of Connectivity Booleans
//size -> Size of 2d matrixs (Number of nodes)
//start -> Starting Node
int * dijkstra(int *map, int *cMap, int size, int start){
    //Distance Array -> Contains Values Of Current Smallest Distances
    int * dist = (int*)malloc(sizeof(int) * size);
    //Set all to infinity
    for(int i = 0; i < size; i++){dist[i] = INFINITY;}

    //Checked Array -> Indicates What Arrays Has Already Been Checked
    //Using calloc to pre-generate with all zeros
    int * checked = (int*)calloc(size, sizeof(int));

    //Set the distance of the starting node to 0
    dist[start] = 0;

    //Loop Through size Times To Go Through All From Nodes
    for(int cout = 0; cout < size; cout++){
        //Get Smallest Node In Distance Array
        int from = shortestNode(dist, checked, size);
        checked[from] = true;
        //Go Through Each Node, Checking Connections
        for(int to = 0; to < size; to++){
            //For navigating 2d arrays, using `(from*size) + to`
            //AKA `(rows*size + column)`

            //Checking If:
            //The Nodes Are Connected (using cMap)
            //The New Node Pair Are The Smallest Value
            //Checked Status Of To Node Is False
            if(cMap[(from*size) + to] == true && dist[to] > dist[from] + map[(from*size) + to] && checked[to] == false){
                //Change Distance To New Shortest Value
                dist[to] = dist[from] + map[(from*size) + to];
            }
        }
    }

    //Free Malloced Memory
    free(checked);

    return dist;

}


// Demo Values


int main(){
    int map[9][9] = {
    {0, 4, 0, 0, 0, 0, 0, 8, 0}, 
    {4, 0, 8, 0, 0, 0, 0, 11, 0}, 
    {0, 8, 0, 7, 0, 4, 0, 0, 2}, 
    {0, 0, 7, 0, 9, 14, 0, 0, 0}, 
    {0, 0, 0, 9, 0, 10, 0, 0, 0}, 
    {0, 0, 4, 14, 10, 0, 2, 0, 0}, 
    {0, 0, 0, 0, 0, 2, 0, 1, 6}, 
    {8, 11, 0, 0, 0, 0, 1, 0, 7}, 
    {0, 0, 2, 0, 0, 0, 6, 7, 0}};

    int cMap[9][9] = {
    {0, 1, 0, 0, 0, 0, 0, 1, 0}, 
    {1, 0, 1, 0, 0, 0, 0, 1, 0}, 
    {0, 1, 0, 1, 0, 1, 0, 0, 1}, 
    {0, 0, 1, 0, 1, 1, 0, 0, 0}, 
    {0, 0, 0, 1, 0, 1, 0, 0, 0}, 
    {0, 0, 1, 1, 1, 0, 1, 0, 0}, 
    {0, 0, 0, 0, 0, 1, 0, 1, 1}, 
    {1, 1, 0, 0, 0, 0, 1, 0, 1}, 
    {0, 0, 1, 0, 0, 0, 1, 1, 0}};

    int * result = dijkstra(&map[0][0], &cMap[0][0], 9, 0);
    
    //Printing Final Values
    debugPrint(result, 9);

    //Free result
    free(result);
    
}	