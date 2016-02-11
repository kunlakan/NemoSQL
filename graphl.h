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
		
		void enumerateSubgraph(int); //enumerateSubgraph
		//void extendSubgraph(vector<int>, queue<int>, int, const int&); //extendSubgraph
		//void getExtension(const int&, queue<int>&, vector<int>& Vsubgraph);
		void extendSubgraph(vector<int>, vector<int>, int, const int&); //extendSubgraph
		void getExtension(const int&, vector<int>&, vector<int>& Vsubgraph);
		bool exclusiveNeighbor(int vertex, vector<int>& Vsubgraph);
	
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
