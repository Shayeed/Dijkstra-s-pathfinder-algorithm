// C program for finding shortest route using Dijkstra's algorithm
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <limits.h>
#include<float.h>
#include <stdbool.h>

 
//-----------------Structures and Global variable-------------------------------
//Structure to city data read from files
struct city {
   int cityCode;
   char city[20];
};

//Structure to hold road data read from files
struct road {
   int sourceCode;
   int destinationCode;
   float distance;
};

struct city City[50]; //Global variable to hold city names
int cityCounter=-1;   //Counter for city set to -1

struct road Road[1300]; //Global variable to hold road data
int roadCounter=-1;  //Counter for road set to -1
//--------------------------------------------------------------------------------

//------------------Functions to read city and road data--------------------------
//Funtion to read city
void readCity() {
     FILE *cityFile;
     cityFile = fopen("City.bin","rb");
     if(cityFile!=NULL) {	
    	fseek(cityFile, 0, SEEK_END);  //Move pointer to end of file
    	long fileSize = ftell(cityFile);   //Finds size of file
    	rewind(cityFile);    //Move pointer to starting of file
    	
    	int numStructures = (int)(fileSize / (sizeof(struct city)));   //Find the number of structures in file
    	
    	size_t returnValue;  //Number of structures read
    	returnValue = fread(City, sizeof(struct city), numStructures, cityFile );  //Read the file in structures
    	cityCounter=returnValue-1;  //Allot counter for city
    	fclose(cityFile);
    	printf("City file read successfull. Print enter to continue...");
        getch();
     }
     else {
     printf("File not found. Press enter to exit...");
     getch();
     exit(0);
     }
}  

//Funtion to read roads
void readRoad() {
     FILE *roadFile;
     roadFile = fopen("Road.bin","rb");
     if(roadFile!=NULL) {	
    	fseek(roadFile, 0, SEEK_END);  //Move pointer to end of file
    	long fileSize = ftell(roadFile);   //Finds size of file
    	rewind(roadFile);    //Move pointer to starting of file
    	
    	int numStructures = (int)(fileSize / (sizeof(struct road)));   //Find the number of structures in file
    	
    	size_t returnValue;  //Number of structures read
    	returnValue = fread(Road, sizeof(struct road), numStructures, roadFile );  //Read the file in structures
    	roadCounter=returnValue-1;  //Allot counter for city
    	fclose(roadFile);
    	printf("\nRoad file read successfull. Print enter to continue...");
        getch();
     }
     else {
     printf("File not found. Press enter to exit...");
     getch();
     exit(0);
     }
} 
//--------------------------------------------------------------------------------

//---------------Structures and functions for graph implementation----------------
// A structure to represent an adjacency list node
struct AdjListNode {
    int cityNumber;         //Code of city linked
    char cityLinked[20];    //Name of city linked
    float distance;         //Distance of city linked
    struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList {
    int cityNumber;
    char city[20];
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
//Graph is representated as an adjacewncy list
struct Graph {
    int V;
    struct AdjList* array;
};
struct Graph* graph;      //Global variable graph
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, float dist) {
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->cityNumber=dest;
    strcpy(newNode->cityLinked, graph->array[dest].city);
    newNode->distance = dist;
    newNode->next = NULL;
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph() {
    graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = cityCounter+1;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc((cityCounter+1) * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i<=cityCounter; i++) {
        graph->array[i].cityNumber=City[i].cityCode;      //Initialise all city codes and names from structure 
        strcpy(graph->array[i].city, City[i].city);
        graph->array[i].head = NULL;
    }
}

// Adds an edge to an undirected graph (Liks or roads)
void addEdge() {
     int i;
     int src, dest;
     float dist;
     for(i=0;i<=roadCounter;i++) {     //Loop to add all road links to graph
        src=Road[i].sourceCode;
        dest=Road[i].destinationCode;
        dist=Road[i].distance;
        
        // Add an edge from src to dest.  A new node is added to the adjacency
        // list of src.  The node is added at the begining
        struct AdjListNode* newNode = newAdjListNode(dest,dist);
        newNode->next = graph->array[src].head;
        graph->array[src].head = newNode;
 
        // Since graph is undirected, add an edge from dest to src also
        newNode = newAdjListNode(src,dist);
        newNode->next = graph->array[dest].head;
        graph->array[dest].head = newNode;
      }
}
 
// A utility function to print the adjacenncy list representation of graph
void printGraph() {
    int v;
    printf("\n");
    for (v = 0; v < graph->V; v++) {
        printf("City code : %d\n",graph->array[v].cityNumber);
        printf("City name : ");
        puts(graph->array[v].city);
        
        struct AdjListNode* pCrawl = graph->array[v].head;    //Temporary pointer to hold head and traverse the list of adjacent cities
        printf("Links");
        while (pCrawl) {
            printf(" -> %d,%s,%.2f", pCrawl->cityNumber, pCrawl->cityLinked, pCrawl->distance);
            pCrawl = pCrawl->next;
        }
        printf("\n\n");
    }
}
//--------------------------------------------------------------------------------

//-----------Structures and functions for minheap representation------------------
// Structure to represent a min heap node
struct MinHeapNode {
    int  v;
    float dist;
};
 
// Structure to represent a min heap
struct MinHeap {
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, float dist) {
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}
 
// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
 
// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest, left, right;
    struct MinHeapNode *smallestNode, *idxNode;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;
 
    if (smallest != idx) {
        // The nodes to be swapped in min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];
 
        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
 
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
 
        minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap) {
    return minHeap->size == 0;
}
 
// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;
 
    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
 
    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
 
    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
 
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return root;
}
 
// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, float dist) {
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
    
    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;
 
    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
 
        // move to parent index
        i = (i - 1) / 2;
    }
}
 
// A utility function to check if a given vertex 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v) {
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}
//--------------------------------------------------------------------------------

//--------------------Dijksta's algorithm and printing route----------------------
void printRoute(float dist[], int par[], int n, int srcMain) {
    int i,j,k;
    int srcTemp;
    
    srcTemp=srcMain; //Source temp is user to trace back the destintion from source
    printf("\nRoute : ");
    printf("%s->",graph->array[srcTemp].city);
   
    while(par[srcTemp]!=-1) {
       for(k=0;k<graph->V;k++) {   //Loop for finding the city name from code and printing it         
          if(graph->array[k].cityNumber == par[srcTemp])
             printf("%s->",graph->array[k].city);
       }
       srcTemp=par[srcTemp]; 
    }
    printf("Destination reached\n");
    printf("Total distance = %.2f", dist[srcMain]);
}


// The main function that calulates distances of shortest paths from src to all vertices. It is a O(ELogV) function
//Here the function revolves around the destination and not the source as we have to trace back the destination from parent array
void dijkstra(int src, int dest) {
    int V = graph->V;// Get the number of vertices in graph
    float dist[V];      // dist values used to pick minimum weight edge in cut
    int parent[V];      //Parrent array to trace back route
    int v;
    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);
 
    // Initialize min heap with all vertices. dist value of all vertices 
    for (v = 0; v < V; ++v) {
        dist[v] = FLT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
 
    // Make dist value of destination vertex as 0 so that it is extracted first
    dist[dest] = 0;
    parent[dest]= -1;
    decreaseKey(minHeap, dest, dist[dest]);
 
    // Initially size of min heap is equal to V
    minHeap->size = V;
 
    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap)) {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number
 
        // Traverse through all adjacent vertices of u (the extracted vertex) and update their distance values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->cityNumber;
 
            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && dist[u] != FLT_MAX && pCrawl->distance + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->distance;
                parent[v] = u;
 
                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    printRoute(dist, parent, V, src);  // print the array of parents
}
//------------------------------------------------------------------------------------

//-------------------------------Maibn function-------------------------------------
int main() {
    int choice;
    int src, dest;
    
    readCity();
    readRoad();
    createGraph();
    addEdge();
    
    while(1) {
       printf("\n\nPlease select an option\n");
       printf("1. Print graph\n");
       printf("2. Find shortest route\n");
       printf("3. Exit\n");
       printf("Please select an option : ");
       scanf("%d",&choice);
       
       switch(choice) {
          case 1:
             printGraph();
             break;
          case 2:
             printf("Please enter the source and destination : \n");
             scanf("%d %d",&src,&dest);
             if(src<graph->V && dest<graph->V && src!=dest && src>-1 && dest>-1) {
                dijkstra(src,dest);
             }
             else
                printf("Incorrect data. Please input again\n");
             break;
          case 3:
             exit(0);
          default:
             printf("Incorrect choice. Please select again\n");
             break;  
       }       
    }
    return 0;    
}
//-------------------------------------------------------------------------------
