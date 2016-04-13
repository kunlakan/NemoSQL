//------------------------------------------------------------------------------
//  Graph.h
//  Created by Kunlakan Cherdchusilp, Ngoc Luu, and Vuochly Ky
//------------------------------------------------------------------------------
// Graph is an undirected and unweigthed graph that is represented by adjacency
// list of vertices that are connected to each other.
// features are included:
//   -- allows adding and removing edges
//   -- allows displaying of the whole Graph with distance and path
//
// ASSUMPTIONS:
//   -- The graph will have no more than 100 vertices
//   -- Every of each VertexNode has ownership of its data stored
//   -- No graph ever share the same object (they could share the same value)
//   -- Input in the file and parameter for insert and remove will have a
//      range of 1 to size. Graph will ONLY work with the elements existing
//      from the range 0 to size - 1
//   -- file input must be properly formatted:
//          1   2
//          2   4       The frist number is the vertex from
//          3   1       The second number is the vertex to
//          0   0
//
//------------------------------------------------------------------------------

#ifndef __Homework_3__Graph__
#define __Homework_3__Graph__

#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <unordered_set>
#include <climits>

#include "GraphData.h"

using namespace std;

class Graph
{
public:

    //-------------------------- A Default Constructor -------------------------
    // Default constructor for class Graph
    // Preconditions: None
    // Postconditions:  - size is initialized to zero
    //                  - Every VertexNode element in vertices will be
    //                    initialized by its default constructor
    Graph();
    
    //--------------------------- A Copy Constructor ---------------------------
    // Copy constructor for class Graph
    // Preconditions: - All of VertexNode.edgeHead in otherGraph's vertices
    //                  points to NULL or an EdgeNode
    //                - All of VertexNode.data in otherGraph's vertices points
    //                  to NULL or a GraphData
    // Postconditions: - size is equal to otherGraph.size
    //                 - vertices will contains the same value and order as
    //                   otherGraph.vertices but with its ownership of all data
    Graph(const Graph &otherGraph);
    
    //------------------------------- Destructor -------------------------------
    // Destructor for class Graph
    // Preconditions: - All of VertexNode.edgeHead points to NULL or an EdgeNode
    //                - All VertexNode.data points to NULL or a GraphData
    // Postconditions: All VertexNode's data and edgeHead and its list of
    //                 EdgeNode are deallocated
    ~Graph();
    
    
    //------------------------------- buildGraph -------------------------------
    // Builds a graph by reading data from an ifstream
    // Preconditions:  infile has been successfully opened and the file contains
    //                 properly formated data (according to the program specs)
    // Postconditions: A graph is read from infile and stored in the object
    void buildGraph(ifstream &infile);
    
    
    //------------------------------- insertEdge -------------------------------
    // Insert an EdgeNode into the graph
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postconditions: - If tempEdge does not exist between the two vertices,
    //                   tempEdge will be inserted
    //                 - If source and destination are not in its range, the
    //                   function will end
    void insertEdge(const int &source, const int &desitnation);
    
    //------------------------------- removeEdge -------------------------------
    // Remove an EdgeNode out of the graph
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postcondition: - If EdgeNode of destination does not exist in the list,
    //                  nothing will happen
    //                - If EdgeNode of destination is found, it gets removed
    void removeEdge(const int &source, const int &destination);
    
    //------------------------------- displayAll -------------------------------
    // Display a all detailed path
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postconditions: all detailed path is displayed
    void displayAll() const;
    
    //--------------------------------- display --------------------------------
    // Display a single detailed path
    // Preconditions: - source and destination should be within the input range
    //                - vertices[vertexFrom] and its data must exist
    // Postconditions: - If source and destination is not in the range, an error
    //                   message is shown
    //                 - a single detailed path is displayed with its locations'
    //                   description
    void display(const int &source, const int &destination) const;
    
    //--------------------------- enumerateSubgraph ----------------------------
    // Enumerate size-k subgraphs of the original graph
    // Preconditions: The graph should have already been built or exists
    // Postcondition: The list of subgraphs are displayed
    void enumerateSubgraph(const int &k);
    
    
private:
    ofstream outfile;
    int count = 0;                          // count number of motif found
    vector<list<int>> vertices;            // adjacency list
    
    
    //--------------------------- PRIVATE: isExist --------------------------
    bool exist(const int &vertex);
    
    //------------------------ PRIVATE: extendSubgraph -------------------------
    // Recursively looking size-k subgraphs of the graph.
    // Precondition: The graph should have already been built or exists
    // Postcondition: The list of subgraphs are displayed
    //void extendSubgraph(vector<int> Vsubgraph, list<int> &Vextension, const int &v, const int &k);
    void extendSubgraph(vector<int> &Vsubgraph, list<int> &Vextension, unordered_set<int> visited, const int &v, const int &k);

    //-------------------------- PRIVATE: getExtension -------------------------
    // Create a list contain all neighbors of v
    // Precondition: None
    // Postcondition: list of v's neighbors is returned
    list<int> getExtension(const int &v, const list<int>& Vextension) const;

    list<int> getExtension(const int &v, const int &w, const list<int>& Vextension, const unordered_set<int> &visited) const;
    
    vector<int> getExclusiveNeighbore(const unordered_set<int> &visited, const int&w) const;
   };

#endif /* defined(__Homework_3__Graph__) */