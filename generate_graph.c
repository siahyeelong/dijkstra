/*
This program will generate the inputs for dijkstras_alg_analysis.c
It takes in a value V, which is the size of the graph, and generate a fully connected directed graph with positive weights
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int V;
    int cost;

    //printf("Enter size of V: \n");
    scanf("%d", &V);

    srand(time(NULL)); // Seed the random number generator with current time

    printf("%d\n", V);

    for(int i = 0; i < V; i++){
        for(int j = 0; j < V; j++){
            if(i != j) {
                cost = rand() % V + 1; // Generate random value from 1 to 100
                printf("%d %d %d\n", i, j, cost);
            }
        }
    }

    printf("a");
}
