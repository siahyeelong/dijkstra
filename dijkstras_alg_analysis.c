// written by Siah Yee Long

/*
This program will take in a graph of size V and run Dijkstra's algorithm in both a matrix representation and list representation
It will run Dikjstra's algorithm multiple times per representation type and output the average CPU time to run the algorithm
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // for sleep function
#define INFINITY 9999999

// create linked list structure
typedef struct _listnode
{
    int vertex;
    int cost; // indicates the cost needed to reach this node
	struct _listnode *next;
} ListNode;

// create pq structure
typedef struct _pqlist
{
    int *qvertex; // the current vertex
    int *qcost; // the cost required to reach this vertex from the prev vertex
    int *qprev; // the prev vertex                                                // actually i realised after programming this shit that it's not really needed to track the prev_vertex in the priority queue lol
    int size; // size of pq
} PQueue;


// create graph structure
enum GraphType {ADJ_MATRIX, ADJ_LIST};
typedef struct _GraphForm{
    int **matrix;
    ListNode **list;
}GraphForm;
typedef struct _graph{
    int V;
    int E;
    enum GraphType type;
    GraphForm adj;
}Graph;

// dijkstra's alg
void dijkstra_matrix(Graph* g, int start, int* d);
void dijkstra_list(Graph* g, int start, int* d);

// helper functions
void printGraphMatrix(Graph );
void printGraphList(Graph );
void pq_swap(PQueue *pq, int a, int b); 
void pq_minHeapify(PQueue *pq, int i);
void pq_insert(PQueue *pq, int vertex, int cost, int prev);
int pq_pop(PQueue *pq);
void pq_updateCost(PQueue *pq, int vertex, int old_cost, int new_cost, int new_prev);
int pq_findVertex(PQueue *pq, int vertex, int cost, int index);
void pq_printPQ(PQueue *pq);
void pq_checkViolation(PQueue *pq);


int main()
{
    struct timespec start_time, end_time; // used to clock start and end time
    Graph g;
    int i,j;
    int start;
    g.type = ADJ_MATRIX;
    
    // ask for size of graph V
    //printf("Enter the number of vertices:\n");
    scanf("%d",&g.V);
    
    // List representation initialisation
    // initialise an array of V elements (origin node)
    g.adj.list = (ListNode **)malloc(g.V * sizeof(ListNode *));
    // Matrix representation initialisation
    // initialise matrix representation by creating a VxV matrix
    g.adj.matrix = (int **)malloc(g.V*sizeof(int *));
    for(i=0;i<g.V;i++){
        g.adj.matrix[i] = (int *)malloc(g.V*sizeof(int));
        g.adj.list[i] = NULL;
    }
    for(i=0;i<g.V;i++)
        for(j=0;j<g.V;j++)
            if(i!=j) g.adj.matrix[i][j] = INFINITY; // set cost to go to every other node as -1
            else g.adj.matrix[i][j] = 0; // set cost to go to itself as 0

    // record adjacent vertices
    g.E = 0;
    int V1, V2, COST;
    //printf("Enter two vertices which are adjacent to each other, followed by the cost:\n");
    while(scanf("%d %d %d", &V1, &V2, &COST)==3)
    {
        if(V1>=0 && V1<g.V && V2>=0 && V2<g.V && COST>0) 
        {
            // direction: V1 --COST--> V2
            ListNode *cur = (ListNode *)malloc(sizeof(ListNode));
            cur->vertex = V2;
            cur->cost = COST;
            cur->next = g.adj.list[V1];
            g.adj.list[V1] = cur;

            g.adj.matrix[V1][V2] = COST;
            g.E++;
        }
        else
            break;
        //printf("Enter two vertices which are adjacent to each other, followed by the cost:\n");
    }

    int cycles = 100; // indicates how many cycles to run to average out the runtime

    int d_matrix[cycles][g.V]; // stores the d[] values after running the alg
    float avgtime_matrix = 0; // used to compute average runtime
    for(start=0;start<cycles;start++){ // loop through different starting nodes 
        clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
        dijkstra_matrix(&g, start, d_matrix[start]);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
        avgtime_matrix += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0 + (end_time.tv_sec  - start_time.tv_sec);
    }
    //printf("Average runtime for matrix representation:\t%f seconds\n", avgtime_matrix /= cycles);
    printf("%f\t", avgtime_matrix /= cycles);

    free(g.adj.matrix);
    g.type = ADJ_LIST;
    
    int d_list[cycles][g.V];
    float avgtime_list;
    for(start=0;start<cycles;start++){ // loop through different starting nodes 
        clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
        dijkstra_list(&g, start, d_list[start]);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
        avgtime_list += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0 + (end_time.tv_sec  - start_time.tv_sec);
    }
    //printf("Average runtime for list representation:\t%f seconds\n", avgtime_list /= cycles);
    printf("%f", avgtime_list /= cycles);
    free(g.adj.list);

    // checking if the d[] outputs are the same for both methods of representation. prev[] doesnt matter since there can be another shortest path to the vertex via another vertex
    for(start=0;start<cycles;start++){
        // printf("Start: %d\n", start);
        // printf("matrix\tlist\n");
        for(i=0;i<g.V;i++) 
            //printf("%d\t%d\n", d_matrix[start][i], d_list[start][i]);
            if(d_matrix[start][i] != d_list[start][i]) printf("ERROR DETECTED\n");
        //printf("\n");
    }

}

// dijkstra's algorithms ------------------------------------

void dijkstra_matrix(Graph *g, int start, int* d)
{
    // visited[] indicates if vertex was visited (a.k.a. "S" in the lecture)
    // d[] indicates the current shortest distance found to this node 
    // prev_vertex[] indicates the previous vertex to reach the current node [a.k.a. "pi" in the lecture]
    int visited[g->V];
    int prev_vertex[g->V];
    int i;

    // explore starting vertex first while initialising everything to unvisited and no prev_vertex
    for(i=0;i<g->V;i++){
        visited[i] = 0;
        d[i] = g->adj.matrix[start][i];
        if(d[i] < 0){
            d[i] = INFINITY;
        }
        prev_vertex[i] = start;
        prev_vertex[i] = -1;
    }
    visited[start] = 1;
    int count = 1;

    // explore the rest of the vertices
    while(count<g->V-1){
        int min_distance = INFINITY;
        int next_vertex;

        // find the smallest cost neighbour to pursue
        for(i=0;i<g->V;i++){
            if(d[i] < min_distance && !visited[i]){
                next_vertex = i;
                min_distance = d[i];
            }
        }
        visited[next_vertex] = 1;
        if(count==1) prev_vertex[next_vertex] = start; // NO I CANT JUST DO THIS

        // update the new d reachable from this new node and update the prev_vertex accordingly
        for(i=0;i<g->V;i++){
            if (!visited[i]){
                if (min_distance + g->adj.matrix[next_vertex][i] < d[i]) {
                    d[i] = min_distance + g->adj.matrix[next_vertex][i];
                    prev_vertex[i] = next_vertex;
                }
            }
        }
        count++;
    }

    // print all arrays to check for correctness
    // printf("\n\ni\td\tprev_vertex\tvisited\n");
    // for(i=0;i<g->V;i++){
    //     printf("%d\t%d\t%d\t\t%d\n", i, d[i], prev_vertex[i], visited[i]);
    // }

}

void dijkstra_list(Graph *g, int start, int *d)
{
    // visited[] indicates if vertex was visited (a.k.a. "S" in the lecture)
    // d[] indicates the current shortest distance found to this node 
    // prev_vertex[] indicates the previous vertex to reach the current node [a.k.a. "pi" in the lecture]
    int visited[g->V];
    int prev_vertex[g->V];
    int i;

    PQueue *pq = malloc(sizeof(PQueue));
    pq->qvertex = malloc(sizeof(int)*g->V);
    pq->qcost = malloc(sizeof(int)*g->V);
    pq->qprev = malloc(sizeof(int)*g->V);
    pq->size = 0;
    ListNode *vertex = g->adj.list[start];
    int new_vertex;

    for(i=0;i<g->V;i++){
        visited[i] = 0;
        d[i] = INFINITY;
        prev_vertex[i] = -1;
    }

    // mark start as visited and explore it
    visited[start] = 1;
    d[start] = 0;
    while(vertex != NULL){
        d[vertex->vertex] = vertex->cost;
        prev_vertex[vertex->vertex] = start;
        vertex = vertex->next;
    }

    for(i=0;i<g->V;i++)
        pq_insert(pq, i, d[i], prev_vertex[i]);

    // while pq is not empty, extract cheapest vertex and explore it
    while(pq->size){
        // extract cheapest vertex
        new_vertex = pq_pop(pq);
        visited[new_vertex] = 1;

        // explore new vertex
        vertex = g->adj.list[new_vertex];
        while(vertex!=NULL){
            if(!visited[vertex->vertex] && d[vertex->vertex] > d[new_vertex]+vertex->cost){ // if there is a new shorter path found, update the pq and d
                int old_cost = d[vertex->vertex];
                d[vertex->vertex] = d[new_vertex]+vertex->cost;
                pq_updateCost(pq, vertex->vertex, old_cost,d[vertex->vertex], new_vertex); 
                prev_vertex[vertex->vertex] = new_vertex;
                // pq_checkViolation(pq);
            }
            vertex = vertex->next;
        }
    }

    free(pq);

    // print all arrays to check for correctness
    // printf("\n\ni\td\tprev_vertex\tvisited\n");
    // for(i=0;i<g->V;i++){
    //     printf("%d\t%d\t%d\t\t%d\n", i, d[i], prev_vertex[i], visited[i]);
    // }

}

// helper functions -----------------------------------------

void printGraphMatrix(Graph g)
{
    int i,j;
    //if(g.type == ADJ_LIST) {printf("Error"); return;}

    for(i=-1;i<g.V;i++){
        if(i==-1){
            printf("\t");
            for(j=0;j<g.V;j++) printf("%d\t", j);
            printf("\n\n");
        }
        else{
            for(j=-1;j<g.V;j++){
                if(j==-1) printf("%d\t", i);
                else{
                    if(g.adj.matrix[i][j]==INFINITY) printf("-\t");
                    else printf("%d\t",g.adj.matrix[i][j]);
                }
            }
            printf("\n");
        }
    }
}

void printGraphList(Graph g){
    int i;
    ListNode* temp;

    //if(g.type == ADJ_MATRIX) {printf("Error"); return;}

    for(i=0;i<g.V;i++)
    {
        printf("%d:\t",i);
        temp = g.adj.list[i];
        while(temp!=NULL){
            printf("-(%d)->%d ", temp->cost, temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

void pq_swap(PQueue *pq, int a, int b)
{
    // this function simply swaps the data of vertex at index a and vertex at index b
    int temp_vertex = pq->qvertex[a];
    int temp_cost = pq->qcost[a];
    int temp_prev = pq->qprev[a];

    pq->qvertex[a] = pq->qvertex[b];
    pq->qcost[a] = pq->qcost[b];
    pq->qprev[a] = pq->qprev[b];

    pq->qvertex[b] = temp_vertex;
    pq->qcost[b] = temp_cost;
    pq->qprev[b] = temp_prev;
}

void pq_minHeapify(PQueue *pq, int i) 
{
    // this function floats the smallest cost item to the top
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->qcost[left] < pq->qcost[smallest])
        smallest = left;

    if (right < pq->size && pq->qcost[right] < pq->qcost[smallest])
        smallest = right;

    if (smallest != i) {
        pq_swap(pq, i, smallest);
        pq_minHeapify(pq, smallest);
    }
}

void pq_insert(PQueue *pq, int vertex, int cost, int prev) 
{
    // at every insertion, float the smallest cost vertex to the top
    int i = pq->size++;
    pq->qvertex[i] = vertex;
    pq->qcost[i] = cost;
    pq->qprev[i] = prev;

    while (i != 0 && pq->qcost[i] < pq->qcost[(i - 1) / 2]) {
        pq_swap(pq, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

int pq_pop(PQueue *pq) 
{
    // this function pops the vertex with the smallest cost and minHeapifies to fix the heap
    if (pq->size == 0) {
        printf("Queue is empty!\n");
        return -1; 
    }
    int smallest = pq->qvertex[0];

    pq->qvertex[0] = pq->qvertex[pq->size-1];
    pq->qcost[0] = pq->qcost[pq->size-1];
    pq->qprev[0] = pq->qprev[pq->size-1];
    pq->size--;
    pq_minHeapify(pq, 0);

    return smallest;
}

void pq_updateCost(PQueue *pq, int vertex, int old_cost, int new_cost, int new_prev)
{
    // this function will search for the vertex and update its cost, then fix it

    int index = pq_findVertex(pq, vertex, old_cost, 0); // find the index of the target vertex
    if(index<0){
        printf("ERROR WITH FINDING VERTEX %d, with cost %d\n", vertex, old_cost);
        pq_printPQ(pq);
        printf("Current size: %d\n", pq->size);
        return;
    }

    // update the new cost and prev
    pq->qcost[index] = new_cost;
    pq->qprev[index] = new_prev;

    // float the vertex up due to the new cost potentially being smaller than the parent
    while (index != 0 && pq->qcost[index] < pq->qcost[(index - 1) / 2]) {
        pq_swap(pq, index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

int pq_findVertex(PQueue *pq, int vertex, int cost, int index)
{
    // this function returns the index of the found vertex
    if(pq->qvertex[index] == vertex) return index;

    int found;
    int left = index*2+1;
    if(left < pq->size && pq->qcost[left] <= cost){
        found = pq_findVertex(pq, vertex, cost, left);
        if(found!=-1 && pq->qvertex[found] == vertex) return found;
    }

    int right = index*2+2;
    if(right < pq->size && pq->qcost[right] <= cost){
        found = pq_findVertex(pq, vertex, cost, right);
        if(found!=-1 && pq->qvertex[found] == vertex) return found;
    }

    return -1;
}

void pq_printPQ(PQueue *pq){
    printf("i\tvertex\tcost\n");
    for(int i=0;i<pq->size;i++){
        printf("%d\t%d\t%d\n", i, pq->qvertex[i], pq->qcost[i]);
    }
}

void pq_checkViolation(PQueue *pq){
    int head = pq->qcost[0];
    for(int i=0;i<pq->size;i++){
        if(pq->qcost[i]<head) printf("VIOLATION\n");
        return;
    }
}