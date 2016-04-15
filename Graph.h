//------------------------------------------------------------------------------
//  Graph.h
//  Created by Kunlakan Cherdchusilp, Ngoc Luu, Jonathan Earl, and Alan Nyugen
//------------------------------------------------------------------------------
// Graph is an undirected and unweigthed graph that is represented by adjacency
// list of vertices that are connected to each other.
// features are included:
//   -- allows adding and removing edges
//   -- allows displaying of the whole Graph with distance and path
//
// ASSUMPTIONS:
//   -- No graph ever share the same object (they could share the same value)
//   -- file input must be properly formatted:
//          1   2
//          2   4       The frist number is the vertex from
//          3   1       The second number is the vertex to
//
//------------------------------------------------------------------------------

#ifndef __Homework_3__Graph__
#define __Homework_3__Graph__

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_set>
#include <climits>

using namespace std;

class Graph
{
public:
    
    //-------------------------- A Default Constructor -------------------------
    // Default constructor for class Graph
    // Preconditions: None
    // Postconditions: None
    Graph();
    
    //------------------------------- Destructor -------------------------------
    // Destructor for class Graph
    // Preconditions: None
    // Postconditions: None
    ~Graph();
    
    
    //------------------------------- buildGraph -------------------------------
    // Builds a graph by reading data from an ifstream
    // Preconditions:  infile has been successfully opened and the file contains
    //                 properly formated data (according to the program specs)
    // Postconditions: A graph is read from infile and stored in the object
    void buildGraph(ifstream &infile);
    
    
    //-------------------------------- display ---------------------------------
    // Display a all detailed path
    // Preconditions: vertices[vertexFrom] and its data must exist
    // Postconditions: all detailed path is displayed
    void display() const;
    
    
    //--------------------------- enumerateSubgraph ----------------------------
    // Enumerate size-k subgraphs of the original graph
    // Preconditions: The graph should have already been built or exists
    // Postcondition: The list of subgraphs are displayed
    void enumerateSubgraph(const int &k);
    
    
private:
    int count = 0;                          // count number of motif found
    vector<unordered_set<int>> vertices;            // adjacency list
    
    
    //----------------------------- PRIVATE: exist -----------------------------
    // Check if vertex already exists in the vector vertices
    // Preconditions: None
    // Postconditions: If vertex already exists in the vector vertices, then do
    //                 nothing. Otherwise, add vertex to the vector vertices.
    void exist(const int &vertex);
    
    //------------------------ PRIVATE: extendSubgraph -------------------------
    // Recursively looking size-k subgraphs of the graph.
    // Precondition: The graph should have already been built or exists
    // Postcondition: The list of subgraphs are displayed
    void extendSubgraph(list<int> &Vsubgraph, unordered_set<int> &Vextension, unordered_set<int> &visited, const int &v, const int &k);
    
    //-------------------------- PRIVATE: getExtension -------------------------
    // Add all neighbors of vertex to Vextension
    // Precondition: Vextension must be decalired and intialized before calling
    //               this function.
    // Postcondition: All neighbors of vertex are added to Vextension
    void getExtension(unordered_set<int> &Vextension, const int &vertex);
    
    
};

#endif /* defined(__Homework_3__Graph__) */