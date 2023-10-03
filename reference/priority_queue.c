#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h> /* for sleep() */

// create pq structure
typedef struct _pqlist
{
    int *qvertex; // the current vertex
    int *qcost; // the cost required to reach this vertex from the prev vertex
    int *qprev; // the prev vertex
    int size; // size of pq
} PQueue;

void pq_swap(PQueue *pq, int a, int b)
{
    // this function simply swaps the data of vertex a and vertex b
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

int pq_findVertex(PQueue *pq, int vertex, int cost, int index){
    if(pq->qvertex[index] == vertex) return index;
    int found;

    int left = index*2+1;
    if(left < pq->size && pq->qcost[left] <= cost){
        found = pq_findVertex(pq, vertex, cost, left);
        if(pq->qvertex[found] == vertex) return found;
    }

    int right = index*2+2;
    if(right < pq->size && pq->qcost[right] <= cost){
        found = pq_findVertex(pq, vertex, cost, right);
        if(pq->qvertex[found] == vertex) return found;
    }

    return -1;
}

int main() {
    PQueue pq;
    pq.qvertex = malloc(sizeof(int)*10);
    pq.qcost = malloc(sizeof(int)*10);
    pq.qprev = malloc(sizeof(int)*10);
    pq.size = 0;

    pq_insert(&pq, 10, 10, 0);
    pq_insert(&pq, 9, 9, 10);
    pq_insert(&pq, 8, 8, 9);
    pq_insert(&pq, 7, 7, 8);
    pq_insert(&pq, 6, 6, 7);
    pq_insert(&pq, 5, 5, 6);
    pq_insert(&pq, 4, 4, 5);
    pq_insert(&pq, 3, 3, 4);
    pq_insert(&pq, 2, 2, 3);
    
    printf("BEFORE:\n");
    for(int i=0; i<pq.size; i++) printf("%d ", pq.qvertex[i]);
    printf("\n");
    for(int i=0; i<pq.size; i++) printf("%d ", pq.qcost[i]);
    printf("\n");
    for(int i=0; i<pq.size; i++) printf("%d ", pq.qprev[i]);
    printf("\n");

    int find = 8;
    printf("Found at: %d\n", pq_findVertex(&pq, find, find, 0));

    printf("AFTER:\n");
    for(int i=0; i<pq.size; i++) printf("%d ", pq.qvertex[i]);
    printf("\n");
    for(int i=0; i<pq.size; i++) printf("%d ", pq.qcost[i]);
    printf("\n");
    for(int i=0; i<pq.size; i++) printf("%d ", pq.qprev[i]);
}