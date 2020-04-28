//Floyd Warshall Shortest Path Algorithm

#include <stdio.h>
#include <stdlib.h>
#define INFINITY 2147483647
#define NEG_INFINITY -2147483647
#define true 1
#define false 0

//Define Maximum Sizes
#define FW_MAX_SIZE 9

//Decide on weather to compile main (Used as library or used in standalone)
// !!Comment out if used as library to prevent main redefinition!! 
#define _DEFMAIN

//Debug Printer for printing all values in distance array
void debugPrint(int distMap[FW_MAX_SIZE][FW_MAX_SIZE]){
    printf("DBG PRINT\n");
    for(int x = 0; x < FW_MAX_SIZE; x++){
        for(int y = 0; y < FW_MAX_SIZE; y++){
            printf("%d ", distMap[x][y]);
        }
        printf("\n");
    }
}

//The meat of the the algorithm
//Takes In:
//map -> 2d matrix of distances between nodes
//cMap -> 2d matrix of Connectivity Booleans
int floydWarshall(int map[FW_MAX_SIZE][FW_MAX_SIZE], int cMap[FW_MAX_SIZE][FW_MAX_SIZE]){
    //2d Matrix Of All Distances in Map
    int distMap[FW_MAX_SIZE][FW_MAX_SIZE] = {false};

    //Go Through All Values Of Map, Setting Distance Values Either Self Values Or Infinity
    for(int i = 0; i < FW_MAX_SIZE; i++){
        for(int j = 0; j < FW_MAX_SIZE; j++){
            if(cMap[i][j]){
                distMap[i][j] = map[i][j];  
            }
            else{
                distMap[i][j] = INFINITY;
            }
        }
    }
    /*
     * This is the FUN part of the algorithm
     * It took me around AN HOUR to wrap my head around this, so I'll try the explain this as well as I can
     * The Formula Loops Through a, b, and c. (Hence Why Its O(n^3))
     * 
     * For This Formula, I have Chosen:
     * a -> From Destination
     * b -> Alternative Node
     * c -> To Destination
     * 
     * These Variables Result In:
     * (a to c) -> Original Connection
     * (a to b) -> Alternative Connection
     * (b to c) -> Connect Alternative Back To Destination
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
    for(int a = 0; a < FW_MAX_SIZE; a++){
        for(int b = 0; b < FW_MAX_SIZE; b++){
            for(int c = 0; c < FW_MAX_SIZE; c++){
                if(distMap[a][b] != INFINITY && distMap[b][c] != INFINITY){
                    if(distMap[a][c] > distMap[a][b] + distMap[b][c]){
                        distMap[a][c] = distMap[a][b] + distMap[b][c];
                    }
                }
            }
        }
    }

    //Checking For Negative Cycles (If the Value Of Self Is Negative)
    for(int v = 0; v < FW_MAX_SIZE; v++){
        if(distMap[v][v] < 0){
            distMap[v][v] = NEG_INFINITY;
        }
    }
    
    //Printing Final Values
    debugPrint(distMap);
}


#ifdef _DEFMAIN //Check if main should be compiled
int main(){


    /*
	int map[FW_MAX_SIZE][FW_MAX_SIZE] = {
        {0, 5, 0, 10}, 
		{0, 0, 3, 0}, 
		{0, 0, 0, 1}, 
		{0, 0, 0, 0} 
	};
	int cMap[FW_MAX_SIZE][FW_MAX_SIZE] = {
        {1, 1, 0, 1}, 
		{0, 1, 1, 0}, 
		{0, 0, 1, 1}, 
		{0, 0, 0, 1} 
	};
    */

    /*
    int map[FW_MAX_SIZE][FW_MAX_SIZE] = {
        {0, 1, 0, 0}, 
        {0, 0, -1, 0}, 
        {0, 0, 0, -1}, 
        {-1, 0, 0, 0}
    }; 
    int cMap[FW_MAX_SIZE][FW_MAX_SIZE] = {
        {1, 1, 0, 0}, 
        {0, 1, 1, 0}, 
        {0, 0, 1, 1}, 
        {1, 0, 0, 1}
    }; 
    */

    int map[FW_MAX_SIZE][FW_MAX_SIZE] = {
    {0, 4, 0, 0, 0, 0, 0, 8, 0}, 
    {4, 0, 8, 0, 0, 0, 0, 11, 0}, 
    {0, 8, 0, 7, 0, 4, 0, 0, 2}, 
    {0, 0, 7, 0, 9, 14, 0, 0, 0}, 
    {0, 0, 0, 9, 0, 10, 0, 0, 0}, 
    {0, 0, 4, 14, 10, 0, 2, 0, 0}, 
    {0, 0, 0, 0, 0, 2, 0, 1, 6}, 
    {8, 11, 0, 0, 0, 0, 1, 0, 7}, 
    {0, 0, 2, 0, 0, 0, 6, 7, 0}};

    int cMap[FW_MAX_SIZE][FW_MAX_SIZE] = {
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
    floydWarshall(map, cMap);

    //Expected Output:
    /*
    DBG PRINT
    8 4 12 19 26 16 9 8 14
    4 8 8 15 22 12 12 11 10
    12 8 4 7 14 4 6 7 2
    19 15 7 14 9 11 13 14 9
    21 24 14 9 18 10 12 13 18
    11 12 4 11 10 4 2 3 6
    9 12 6 13 12 2 2 1 6
    8 11 7 14 13 3 1 2 7
    14 10 2 9 16 6 6 7 4
    */
}
#endif //_DEFMAIN
