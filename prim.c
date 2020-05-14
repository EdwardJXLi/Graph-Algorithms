//Prim Minimum Spanning Tree

#include <stdio.h>
#include <stdlib.h>
#define INFINITY 2147483647
#define NEG_INFINITY -2147483647
#define true 1
#define false 0

//Decide on weather to compile main (Used as library or used in standalone)
// !!Comment out if used as library to prevent main redefinition!! 
#define _DEFMAIN

//The meat of the the algorithm
//Takes In:
//map -> 2d matrix of distances between nodes
//cMap -> 2d matrix of Connectivity Booleans
//size -> Size of 2d matrixs (Number of nodes)
//start -> Starting Node
int prim(int * map, int * cMap, int size, int start){
    //Set Total Cost, Lowest From Node, Lowest To Node, and Current Vertex Cost 
    int cost = 0;
    int lowestFrom, lowestTo, currentCost;

    //Visited Array -> Indicates What Nodes Have Already Been Visited (Prevent Self-Looping)
    //Using calloc to pre-generate with all zeros
    int * visited = (int*)calloc(size, sizeof(int));
    visited[start] = true;

    //TODO: Make More Efficient
    //Loop Through size-1 So All Nodes Gets Touched
    for(int i = 0; i < size-1; i++){
        //Set lowestFrom, lowestTo, and , and currentCost to default value
        //lowestFrom and lowestTo to -1. (Set to -1 to check if value has been changed)
        //CurrentCost to INFINITY (Used for checking if cost is lowest)
        lowestFrom = -1;
        lowestTo = -1;
        currentCost = INFINITY;
        //Check for all visited nodes, acting as "from" node
        for(int from = 0; from < size; from++){
            if(visited[from]){
                //Check connection to all possible nodes, acting as "to" node
                for(int to = 0; to < size; to++){
                    //For navigating 2d arrays, using "(from*size) + to"
                    //AKA "(rows*size + column)"

                    
                    //Checking If:
                    //The Nodes Are Connected (using cMap)
                    //The Node Is Not Part Of The Visited Nodes (Prevents Self Looping)
                    //Current Cost Is Lowest Cost
                    if(cMap[(from*size) + to] && visited[to] == false && map[(from*size) + to] < currentCost){
                        currentCost = map[(from*size) + to]; //Set new lowest cost
                        lowestFrom = from;
                        lowestTo = to;
                    }
                }
            }
        }
        //Checking If lowestFrom and lowestTo has been changed
        if(lowestFrom != -1 && lowestTo != -1){
            //Has Been Changed, Adding CurrentCost to Cost
            cost = cost + currentCost;
            visited[lowestTo] = true;
            //Printing Debug Text
            #ifdef DEBUG
            printf("Vertex From %d to %d Cost %d (Now Cost %d)\n", lowestFrom, lowestTo, currentCost, cost);
            #endif
        }
        else{
            //Has Not Been Changed, Exiting Loop!
            break;
        }
    }
    
    //Free Allocated Memory (Visited Array)
    free(visited);

    //Return Cost
    return cost;
}



#ifdef _DEFMAIN //Check if main should be compiled
int main(){

    //Test Values

    /*
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
    */

    int map[5][5] = {
        {0, 2, 0, 6, 0}, 
        {2, 0, 3, 8, 5}, 
        {0, 3, 0, 0, 7}, 
        {6, 8, 0, 0, 9}, 
        {0, 5, 7, 9, 0}};

    int cMap[5][5] = {
        {0, 1, 0, 1, 0}, 
        {1, 0, 1, 1, 1}, 
        {0, 1, 0, 0, 1}, 
        {1, 1, 0, 0, 1}, 
        {0, 1, 1, 1, 0}};

    //Run Prim And Get Result
    int cost = prim(&map[0][0], &cMap[0][0], 5, 0);

    //Print Final Cost
    printf("%d\n", cost);

    //Expected Output:
    /*
    16
    */

}
#endif //_DEFMAIN