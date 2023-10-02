#include <stdio.h>
#include <stdlib.h>

enum GraphType {ADJ_MATRIX, ADJ_LIST}; // Types of Graph Representation

typedef struct _listnode
{
    int vertex;
	struct _listnode *next;
} ListNode;

union GraphForm{
    int **matrix;
    ListNode **list;
};

typedef struct _graph{
    int V;
    int E;
    enum GraphType type;
    union GraphForm adj;
}Graph;

void printGraphMatrix(Graph );
void adjM2adjL(Graph *);
void adjL2adjM(Graph *);
void printGraphList(Graph );
void calDegreeV(Graph,int *);
void printDegreeV(int *,int );

int main()
{
    Graph g;
    int i,j;

    int* degreeV;

    printf("Enter the number of vertices:\n");
    scanf("%d",&g.V);

    g.E = 0;
    g.adj.matrix = (int **)malloc(g.V*sizeof(int *));
    for(i=0;i<g.V;i++)
        g.adj.matrix[i] = (int *)malloc(g.V*sizeof(int));

    for(i=0;i<g.V;i++)
        for(j=0;j<g.V;j++)
            g.adj.matrix[i][j] = 0;
    g.type = ADJ_MATRIX;

    degreeV = (int *) malloc(g.V*sizeof(int));
    for(i=0;i<g.V;i++)
        degreeV[i]=0;

    int V1, V2;
    printf("Enter two vertices which are adjacent to each other:\n");
    while(scanf("%d %d",&V1,&V2)==2)
    {
        if(V1>0 && V1<=g.V && V2>0 && V2<=g.V) //seems like its an undirected graph with no weights
        {
            g.adj.matrix[V1-1][V2-1] = 1;
            g.adj.matrix[V2-1][V1-1] = 1;
            g.E++;
        }
        else
            break;
        printf("Enter two vertices which are adjacent to each other:\n");
    }

    calDegreeV(g,degreeV);

    printGraphMatrix(g);
    printDegreeV(degreeV,g.V);

    adjM2adjL(&g);

    calDegreeV(g,degreeV);

    printGraphList(g);
    printDegreeV(degreeV,g.V);

    adjL2adjM(&g);
    printGraphMatrix(g);

    return 0;
}

void printGraphMatrix(Graph g)
{
    int i,j;
    if(g.type == ADJ_LIST) {printf("Error"); return;}

    for(i=0;i<g.V;i++){
        for(j=0;j<g.V;j++)
            printf("%d\t",g.adj.matrix[i][j]);
        printf("\n");
    }

}

void printDegreeV(int *degreeV,int V)
{
    int i;
    for(i=0;i<V;i++)
        printf("%d: %d degree\n",i+1,degreeV[i]);
}

void printGraphList(Graph g){
    int i;
    ListNode* temp;

    if(g.type == ADJ_MATRIX) {printf("Error"); return;}

    for(i=0;i<g.V;i++)
    {
        printf("%d:\t",i+1);
        temp = g.adj.list[i];
        while(temp!=NULL){
            printf("%d -> ",temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

void adjM2adjL(Graph *g) // Question 1 -----------------------------------
{
    //check if GraphType is a matrix or list. return if list, else 
    if(g->type == ADJ_LIST) return;
    //if matrix
    else {
        int i, j;

        //generate the initial list with no links
        ListNode **adjList = (ListNode **)malloc(g->V * sizeof(ListNode *));

        //cycle through matrix
        for(i=0;i<g->V;i++){
            for(j=0;j<g->V;j++){
                //add item to list if there is a relation
                if(g->adj.matrix[i][j] == 1){
                    //push to list
                    ListNode *cur = (ListNode *)malloc(sizeof(ListNode));
                    cur->vertex = j+1;
                    cur->next = adjList[i];
                    adjList[i] = cur;
                }
            }
        }

        g->type = ADJ_LIST;
        free(g->adj.matrix);
        g->adj.list = (ListNode **)malloc(g->V * sizeof(ListNode *));
        g->adj.list = adjList;

        return;
    }

}

void adjL2adjM(Graph *g){
	if(g->type == ADJ_MATRIX) return;
    else{
        int i, j;

        int **adjMatrix = (int **)malloc(g->V*sizeof(int *));
        for(i=0;i<g->V;i++) adjMatrix[i] = (int *)malloc(g->V*sizeof(int));

        for(i=0;i<g->V;i++)
            for(j=0;j<g->V;j++) adjMatrix[i][j]=0;

        for(i=0;i<g->V;i++){
            ListNode *temp = g->adj.list[i];
            while(temp!=NULL){
                adjMatrix[i][temp->vertex-1] = 1;
                temp = temp->next;
            }
        }

        free(g->adj.list);
        g->type = ADJ_MATRIX;
        g->adj.matrix = (int **)malloc(g->V*sizeof(int *));
        g->adj.matrix = adjMatrix;

        return;
    }
}

void calDegreeV(Graph g, int *degreeV)
{
    int counter = 0, i, j;

	if(g.type == ADJ_LIST){
        // if graph is an adjacency list

        for(i=0;i<g.V;i++){
            ListNode *temp = g.adj.list[i];
            while(temp!=NULL){
                counter++;
                temp = temp->next;
            }
            degreeV[i] = counter;
            counter = 0;
        }
    }
    else{
        // if graph is an adjacency list

        for(i=0;i<g.V;i++){
            for(j=0;j<g.V;j++){
                if(g.adj.matrix[i][j] == 1){
                    counter++;
                }
            }
            degreeV[i] = counter;
            counter = 0;
        }
    }
}

