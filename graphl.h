//------------------------------------------ graphl.h ---------------------------------------------
// Programmer Name: Jeen Cherdchusilp & Ngoc Luu 
// Creation Date: 10/26/2015 // Date of Last Modification: 02/06/2016
// -------------------------------------------------------------------------------------------------
// Purpose - a brief statement of the program's function 
// This project aims to implement depth first search algorithm
// ------------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions. 
// -------------------------------------------------------------------------------------------------
// GraphL class: 
// This class aims to implement the depth first search algorithm.
//
// Implementation and assumptions:
// -- A graph can have at most 100 nodes. (This assumption might change to the scale of the project)
// -- This program assumes the input data has correctly formatted, valid data.
// -- This program displays the result to demonstrate the algorithm works properly.
//--------------------------------------------------------------------------------------------------

#ifndef GRAPHL_H
#define GRAPHL_H
#include <string>	// for string
#include <iostream>	// for output
#include <fstream>	// for file reading
#include <iomanip> 	// for output formatting
#include "limits.h" // for INT_MAX
#include "nodedata.h" // for data type
#include <vector>
#include <queue>

class GraphL
{
	public:
		const static int MAXNODES = 101; // max number of nodes in a graph 
		GraphL();  // constructor
		~GraphL(); // destructor
		
		void buildGraph(ifstream&); // build a graph from a text file
		void insertEdge(int, int);  // inset an edge between 2 nodes
		void depthFirstSearch();	// perform pre-order traversal in a graph
		void displayGraph();		// display the result of depth first search
		
		//struct GraphNode;
		void enumerateSubgraph(int); //enumerateSubgraph
		void extendSubgraph(vector<int>, queue<int>, int, const int&); //extendSubgraph
		void getExtension(const int&, queue<int>);
	
	private:
		struct EdgeNode;		 // forward reference for the compiler
		struct GraphNode 		 // structs used for simplicity, use classes if desire
		{
			EdgeNode* edgeHead;  // head of the list of edges
			NodeData* data; 	 // data information about each node
			bool visited;		 // determine if a node is visited or not
		};
		
		struct EdgeNode
		{
			int adjGraphNode; 	 // subscript of the adjacent graph node
			EdgeNode* nextEdge;
		};	
		GraphNode nodeArray[MAXNODES]; // array of graph nodes
		int size; 					   // current used size of the graph
		void removeNode(int);		   // remove a node in the graph
		void dfsHelper(int);		   // recursive helper method
};
#endif
