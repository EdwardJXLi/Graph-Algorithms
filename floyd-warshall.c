//Floyd Warshall Shortest Path Algorithm

#include <stdio.h>
#include <stdlib.h>
#define INFINITY 2147483647
#define NEG_INFINITY -2147483647
#define true 1
#define false 0

//Decide on weather to compile main (Used as library or used in standalone)
// !!Comment out if used as library to prevent main redefinition!! 
#define _DEFMAIN

//Debug Printer for printing all values in distance array
void debugPrint(int *distMap, int size){
    printf("DBG PRINT\n");
    for(int y = 0; y < size; y++){
        for(int x = 0; x < size; x++){
            printf("%d ", distMap[(y*size) + x]);
        }
        printf("\n");
    }
}

//The meat of the the algorithm
//Takes In:
//map -> 2d matrix of distances between nodes
//cMap -> 2d matrix of Connectivity Booleans
//size -> Size of 2d matrixs (Number of nodes)
int * floydWarshall(int *map, int *cMap, int size){
    //2d Matrix Of All Distances in Map
    //Using calloc to pre-generate with all zeros
    int * distMap = (int*)calloc(size*size, sizeof(int)); //Using size*size because 2d array

    //For navigating all 2d arrays, using "(from*size) + to"
    //AKA "(i*size + j)"


    //Go Through All Values Of Map, Setting Distance Values Either Self Values Or Infinity
    for(int j = 0; j < size; j++){
        for(int i = 0; i < size; i++){
            if(cMap[(i*size) + j]){
                distMap[(i*size) + j] = map[(i*size) + j];  
            }
            else{
                distMap[(i*size) + j] = INFINITY;
            }
        }
    }
    /*
     * This is the FUN part of the algorithm
     * It took me around AN HOUR to wrap my head around this, so I'll try the explain this as well as I can
     * The Formula Loops Through i, k, and j. (Hence Why Its O(n^3))
     * 
     * For This Formula, I have Chosen:
     * i -> From Destination
     * k -> Alternative Node / Pivot Node
     * j -> To Destination
     * 
     * These Variables Result In:
     * (i to j) -> Original Connection
     * (i to k) -> Alternative Connection
     * (k to j) -> Connect Alternative Back To Destination
     * 
     * To Sum It Up:
     * The Computer Loops Through All Possible "From" Nodes
     * The Computer Loops Through All Possible "To" Nodes
     * The Computer Loops Through All Possible "Alternative" Nodes
     * The Computer Checkes If The Connection From The "From" Node To The "Alternative" Node (continued)
     * Plus The "Alternative" Node To The "To" Node Is Shorter Than (continued)
     * Connection From The "From" Node To The "To" Node
     * 
     * This way the computer all possible deviations from one node to another node.
     * 
     * This is the best explination I have for this algorithm for now!
    */
    //Loop through k, i, j
    for(int k = 0; k < size; k++){
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                //Check if i->k and k->j is reachable (Not INFINITY)
                if(distMap[(i*size) + k] != INFINITY && distMap[(k*size) + j] != INFINITY){ //if(distMap[i][k] != INFINITY && distMap[k][j] != INFINITY){
                    //Checking if new connection is shorter than old connection
                    if(distMap[(i*size) + j] > distMap[(i*size) + k] + distMap[(k*size) + j]){ //if(distMap[i][j] > distMap[i][k] + distMap[k][j]){
                        //Set connection to new (and shorter) connection
                        distMap[(i*size) + j] = distMap[(i*size) + k] + distMap[(k*size) + j]; //distMap[i][j] = distMap[i][k] + distMap[k][j];
                    }
                }
            }
        }
    }

    //Check for nagative cycles
    //Similar to first run through, but check if Alternate / Pivot Node Is A Negative Value
    //(Negative Value Means It Has To Be Part Of A Negative Cycle)
    //If so, set original connection value to -Infinity (Self Loop Flag)
    for(int k = 0; k < size; k++){
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                //Check if i->k and k->j is reachable (Not INFINITY)
                if(distMap[(i*size) + k] != INFINITY && distMap[(k*size) + j] != INFINITY){ //if(distMap[i][k] != INFINITY && distMap[k][j] != INFINITY){
                    //Checking if Self Distance of Alternate / Pivot Node Is Negative 
                    if(distMap[(k*size) + k] < 0){ //if(distMap[k][k] < 0){
                        //Set connection to -Infinity (Flag for negative loop) 
                        distMap[(i*size) + j] = NEG_INFINITY; //distMap[i][j] = NEG_INFINITY;
                    }
                }
            }
        }
    }

    //Return distMap 2d array
    return distMap;
}


#ifdef _DEFMAIN //Check if main should be compiled
int main(){

    //Test Values

    /*
    //Simple No-Frills Graph
    int map[4][4] = {
        {0, 5, 0, 10}, 
        {0, 0, 3, 0}, 
        {0, 0, 0, 1}, 
        {0, 0, 0, 0} 
    };
    int cMap[4][4] = {
        {1, 1, 0, 1}, 
        {0, 1, 1, 0}, 
        {0, 0, 1, 1}, 
        {0, 0, 0, 1} 
    };
    */
    

    /*
    //Map With All Negative Cycles
    int map[4][4] = {
        {0, 1, 0, 0}, 
        {0, 0, -1, 0}, 
        {0, 0, 0, -1}, 
        {-1, 0, 0, 0}
    }; 
    int cMap[4][4] = {
        {1, 1, 0, 0}, 
        {0, 1, 1, 0}, 
        {0, 0, 1, 1}, 
        {1, 0, 0, 1}
    }; 
    */


    /*
    //Large Map All Negative Cycles
    int map[8][8] = {
        {0, 4, 4, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 4, -2, 0, 0},
        {3, 0, 2, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, -2, 0},
        {0, 3, 0, 0, -3, 0, 0, 0},
        {0, 0, 0, 0, 0, 2, 0, 2},
        {0, 0, 0, 0, -2, 0, 0, 0}
    }; 
    int cMap[8][8] = {
        {0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0},
        {1, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, -2, 0, 0, 0}
    }; 
    */


    /*
    //Large Map With SOME Negative Cycles (Same one in bellman ford algo)
    int map[10][10] = {
        {0, 5, 0, 0, 0, 0, 0, 0, 0, 0}, //0
        {0, 0, 20, 0, 0, 30, 60, 0, 0, 0}, //1
        {0, 0, 0, 10, 75, 0, 0, 0, 0, 0}, //2
        {0, 0, -15, 0, 0, 0, 0, 0, 0, 0}, //3
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 100}, //4
        {0, 0, 0, 0, 25, 0, 5, 0, 50, 0}, //5
        {0, 0, 0, 0, 0, 0, 0, -50, 0, 0}, //6
        {0, 0, 0, 0, 0, 0, 0, 0, -10, 0}, //7
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //8
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //9
    };

    int cMap[10][10] = {
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //0
        {0, 1, 1, 0, 0, 1, 1, 0, 0, 0}, //1
        {0, 0, 1, 1, 1, 0, 0, 0, 0, 0}, //2
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0}, //3
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, //4
        {0, 0, 0, 0, 1, 1, 1, 0, 1, 0}, //5
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, //6
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 0}, //7
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, //8
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //9
    };
    */
   
    
    //Large map
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
    

    //Run Floyd Warshall And Get Result
    int * resultMap = floydWarshall(&map[0][0], &cMap[0][0], 9);
    
    //Printing Final Values
    debugPrint(resultMap, 9);   

    //Free resultMap
    free(resultMap);

    //Expected Output:
    /*
    DBG PRINT
    8 4 12 19 21 11 9 8 14
    4 8 8 15 22 12 12 11 10
    12 8 4 7 14 4 6 7 2
    19 15 7 14 9 11 13 14 9
    21 22 14 9 18 10 12 13 16
    11 12 4 11 10 4 2 3 6
    9 12 6 13 12 2 2 1 6
    8 11 7 14 13 3 1 2 7
    14 10 2 9 16 6 6 7 4
    */
}
#endif //_DEFMAIN
