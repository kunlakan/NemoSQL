//---------------------------------------------------------------------------
//  Graph.h
//  Created by Kunlakan Cherdchusilp on 1/30/15.
//---------------------------------------------------------------------------
// Graph represents directed weigthed graph. The graph is represented as
// adjacency list, vertices, that has VertexNode as a set of location and
// EdgeNode as links/paths connecting them.
// features are included:
//   -- allows adding and removing edges
//   -- allows displaying of the whole Graph with distance and path
//   -- allows finding the shortest path between all verteices to other
//      vertices
//
// ASSUMPTIONS:
//   -- The graph will have no more than 100 vertices
//   -- Every of each VertexNode has ownership of its data stored
//   -- No graph ever share the same object (they could share the same value)
//   -- Negative edge's weight is allowed, but findShortestPath() will not be
//      efficient and reliable
//   -- Input in the file and parameter for insert and remove will have a
//      range of 1 to size. Graph will ONLY work with the elements existing
//      from the range 0 to size - 1
//   -- After insertEdge and removeEdge are called, findShortestPath should
//      be call before displaying as appropriate.
//   -- file input must be properly formatted:
//          4                      # Number of vertices
//          Olson’s office         # Vertices description
//          Classroom
//          STEM office
//          1 2 10                 # From vertex1 to vertex2 with weight 10
//          2 4 10
//          3 1  5
//          0 0  0                 # Signals the end of data
//---------------------------------------------------------------------------

#ifndef __Homework_3__Graph__
#define __Homework_3__Graph__

#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <climits>

#include "GraphData.h"

using namespace std;

class Graph
{
public:

    //-------------------------- A Default Constructor --------------------------
    // Default constructor for class Graph
    // Preconditions: None
    // Postconditions: - size is initialized to zero
    //                 - Every VertexNode element in vertices will be initialized
    //                   by its default constructor
    //                 - Every Table element in T will be intialized by its default
    //                   constructor
    Graph();
    
    //--------------------------- A Copy Constructor ----------------------------
    // Copy constructor for class Graph
    // Preconditions: - All of VertexNode.edgeHead in otherGraph's vertices points
    //                  to NULL or an EdgeNode
    //                - All of VertexNode.data in otherGraph's vertices points to
    //                  NULL or a GraphData
    // Postconditions: - size is equal to otherGraph.size
    //                 - vertices will contains the same value and order as
    //                   otherGraph.vertices but with its ownership of all data
    //                 - T will have the same value as otherGraph.T
    Graph(const Graph &otherGraph);
    
    //------------------------------- Destructor --------------------------------
    // Destructor for class Graph
    // Preconditions: - All of VertexNode.edgeHead points to NULL or an EdgeNode
    //                - All VertexNode.data points to NULL or a GraphData
    // Postconditions: All VertexNode's data and edgeHead and its list of
    //                 EdgeNode are deallocated
    ~Graph();
    
    
    //------------------------------- buildGraph --------------------------------
    // Builds a graph by reading data from an ifstream
    // Preconditions:  infile has been successfully opened and the file contains
    //                 properly formated data (according to the program specs)
    // Postconditions: One graph is read from infile and stored in the object
    void buildGraph(ifstream &infile);
    
    
    //------------------------------- insertEdge --------------------------------
    // Insert an EdgeNode into the graph
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postconditions: - If tempEdge does not exist between the two vertices,
    //                   tempEdge will be inserted
    //                 - If source and destination are not in its range, function ends
    void insertEdge(const int &source, const int &desitnation);
    
    //------------------------------- removeEdge --------------------------------
    // Remove an EdgeNode out of the graph
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postcondition: - If EdgeNode of destination does not exist in the list,
    //                  nothing will happen
    //                - If EdgeNode of destination is found, it gets removed
    void removeEdge(const int &source, const int &destination);
    
    //---------------------------- findShortestPath -----------------------------
    // Finds the shortest path from all vertices to another vertices.
    // Preconditions: - All VertexNode.edgeHead points to NULL or an EdgeNode
    //                - All VertexNode.data points to NULL or a GraphData
    // Postcondition: distance and path of all vertex to other vertex are found
    //                and stored in the table T
    void findShortestPath();
    
    //-------------------------------- displayAll -------------------------------
    // Display a all detailed path
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postconditions: all detailed path is displayed
    void displayAll() const;
    
    //--------------------------------- display ---------------------------------
    // Display a single detailed path
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postconditions: - If source and destination is not in the range, an error
    //                   message is shown
    //                 - a single detailed path is displayed with its locations'
    //                   description
    void display(const int &source, const int &destination) const;
    
    void enumerateSubgraph(const int &k);
    void extendSubgraph(vector<int> Vsubgraph, list<int> &Vextension, int v, const int &k);
    list<int> getExtension(const int &v, const list<int> &Vextension) const;
    bool isDuplicate(const int &target, const list<int>& Vextension) const;
    
    
private:
    //----------------------------- EdgeNode Struct -----------------------------
    // EdgeNode struct represents an edge connecting from one vertex to the other.
    // There are 2 types of constructor in this struct: defualt constructor and
    // two paremeter constructor.
    //---------------------------------------------------------------------------
    struct EdgeNode {
        int adjVertex;                      // subscript of the adjacent vertex
        int weight;                         // weight of edge
        EdgeNode *nextEdge;
        
        //-------------------------- A Default Constructor --------------------------
        // Default constructor for struct EdgeNode
        // Preconditions: None
        // Postconditions: - adjVertex and weight is set to 0, and
        //                 - nextEdge points to NULL
        EdgeNode():
            adjVertex(0), weight(0), nextEdge(NULL){}
        
        //----------------------- A Two Parameters Constructor ----------------------
        // Two parameter constructor for struct EdgeNode
        // Preconditions: None
        // Postcontitions: - adjVertex is set to vertex
        //                 - nextEdge points to NULL
        EdgeNode(const int &vertex):
            adjVertex(vertex), nextEdge(NULL){}
    };
    
    //----------------------------- EdgeNode Struct -----------------------------
    // VertexNode struct represents a vertex. There is a defualt constructor.
    //---------------------------------------------------------------------------
    struct VertexNode {
        EdgeNode *edgeHead;                 // head of the list of edges
        GraphData *data;                    // store vertex data here
        
        //-------------------------- A Default Constructor --------------------------
        // Default constructor for struct VertexNode
        // Preconditions: None
        // Postconditions: edgeHead and data point to NULL
        VertexNode():
            edgeHead(NULL), data(NULL){}
    };
    
    //------------------------------ Table Struct -------------------------------
    // VertexNode struct represents a vertex. There is a defualt constructor.
    //---------------------------------------------------------------------------
    struct Table {                          // table of information for Dijkstra
        bool visited;                       // whether vertex has been visited
        int path;                           // previous vertex in path of min dist
        int dist;                           // shortest known distance from source
        
        //-------------------------- A Default Constructor --------------------------
        // Default constructor for struct Table
        // Preconditions: None
        // Postconditions: visited is set to false, path to 0, and dist to INT_MAX
        Table():
            visited(false), path(0), dist(INT_MAX){}
    };
    
    //---------------------------- Comparator Struct ----------------------------
    // Comparator struct uses to compare two vertices to maintain a priority in
    // the priority queue. So, when we pop a vertex from priority queue the vertex
    // with minimum d value is extracted.
    //---------------------------------------------------------------------------
    struct Comparator{
        bool operator() (const pair<int, int> &p1, const pair<int, int> &p2)
        {
            return p1.second > p2.second;
        }
    };
    
    static const int MAX_VERTICES = 100;
    static const int MAX_CHAR_LENGTH = 50;
    
    int size;                               // number of vertices in the graph
    VertexNode vertices[MAX_VERTICES];      // adjacency list
    Table T[MAX_VERTICES][MAX_VERTICES];    // Dijkstra's algorithm table
    
    
    //------------------------ PRIVATE: copyAllVertices -------------------------
    // Copies every VertexNode element and all of its EdgeNode inside othersVertices
    // over to verticies sequentially starting from index 0 to size. vertices will
    // have its ownership over all data and EdgeNode elements its stored.
    // Preconditions: size must be equal to size of otherVertices' graph
    // Postcondition: - Every VertexNode element in vertices from index 0 to size
    //                  will the same as otherVertices and stores the same data
    //                  but with its ownership (alloated new memory)
    //                - Every EdgeNode linked from vertices will have the same
    //                  value and added order as EdgeNode liked from otherVertices
    void copyAllVertices(const VertexNode *othersVertices);
    
    //-------------------------- PRIVATE: copyAllEdge ---------------------------
    // Creates a copy of every information recursively from otherEdge to thisEdge.
    // thisEdge will have its ownership over all of its data stored.
    // Preconditions: None
    // Postcondition: Every thisEdge will have the same value and added order as
    //                otherEdge
    void copyAllEdge(EdgeNode *&thisEdge, EdgeNode *&otherEdge);
    
    //------------------------- PRIVATE: removeAllEdge --------------------------
    // Removes all EdgeNode recursively
    // Preconditions: currentEdge points to either NULL or an EdgeNode
    // Postconditions: currentEdge->nextEdge, and currentEdge will be deallocated
    //                 and set to NULL
    void removeAllEdge(EdgeNode *&current);

    
    //-------------------------- PRIVATE: insertHelper --------------------------
    // Recursively looking if the edge already exist inside the list. If so, the
    // existing weight will be changed to a new weight. Otherwise, inserts edge
    // at the end of the list.
    // Preconditions: current and edge points to NULL or a first EdgeNode
    // Postconditions: - If edge does not already exist in the list, it will be
    //                   inserted to the end of the list
    //                 - If edge already exist in the list, found edge will be
    //                   replaced its weight with with edge->weigth
    void insertHelper(EdgeNode *&head, EdgeNode *&edge);
    
    //-------------------------- PRIVATE: removeHelper --------------------------
    // Recursively looking for the EdgeNode containing destination to remove
    // Preconditions: current points to NULL or an EdgeNode
    // Postconditions: If EdgeNode containing destination is found, it will be
    //                 deallocated. Otherwise, funtions ends
    void removeHelper(EdgeNode *&current, int destination);
    
    //------------------------- PRIVATE: dijkstraHelper -------------------------
    // Dijkstra's algorithm of finding the shortest path. priority queue is used
    // in this function. Note that If some of the edge has a nevagive weight,
    // this algorithm will not be efficient and reliable
    // Preconditions: - VertexNode in vertices and its data must exist
    //                - All VertexNode.edgeHead points to NULL or an EdgeNode
    // Postconditions: distance and path of every vertex to every other vertex are
    //                 found and stored in the table T
    void dijkstraHelper(const int &source);
    
    //--------------------------- PRIVATE: resetTable ---------------------------
    // Resets table T
    // Preconditions: None
    // Postconditions: All Table T.visited is resetted to false, T.dist to
    //                 INT_MAX, and T.path to 0
    void resetTable();
    
    //------------------------ PRIVATE: displayAllHelper ------------------------
    // Recursively display a all detailed path
    // Preconditions: vertices[vertexFrom] and its data must exist
    // Postconditions: All detailed path is displayed
    void diplayAllHelper(const int &source) const;
    
    //-------------------------- PRIVATE: displayPath ---------------------------
    // Recursively display shortest path
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postconditions: shortest path is displayed
    void displayPath(const int &source, const int &path, const int &destination) const;
    
    //------------------------ PRIVATE: displayPathName -------------------------
    // Recursively display shortest path's location description
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postconditions: shortest path's location description is displayed
    void displayPathName(const int &source, const int &path, const int &destination) const;
    
    
    //--------------------------- PRIVATE: areInRange ---------------------------
    // Checks if source and destination are in the range of 0 to size
    // Preconditions: None
    // Postconditions: true is returned if the source and destination are in the
    //                 range. Otherwise, false is returned
    bool areInRange(const int &source , const int &destination) const;
};

#endif /* defined(__Homework_3__Graph__) */