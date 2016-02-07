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
#include "graphl.h" // header file

//------------------------------------------ GraphL ------------------------------------------------
// Constructor: initializes size to 0
// @param none
// @return nothing
//--------------------------------------------------------------------------------------------------
GraphL::GraphL()
{
	size = 0; //set size to 0
}// end of Graph

//------------------------------------------ ~GraphL -----------------------------------------------
// Destructor to deallocate all nodes in the graph
//
// @param none
// @return nothing
//--------------------------------------------------------------------------------------------------
GraphL::~GraphL()
{
	// a loop to delete all nodes in the graph
	if(size > 0)
	{
		for(int i = 0; i <= size; i++)
		{	
			while(nodeArray[i].edgeHead != NULL)
				removeNode(i); // call removeNode
			
			delete nodeArray[i].data;
			nodeArray[i].data = NULL;
		}
		size = 0;	// reset size to 0
	}
}
//end of ~Graph

//-------------------------------------------- removeNode ------------------------------------------
// removeNode is a helper method to remove the head node in the adjacency list of a node
//
// @param index-int-the index of the node to delete its head node in the adjacency list
// @return nothing
//--------------------------------------------------------------------------------------------------
void GraphL::removeNode(int index)
{
	// copy edgehead to nodeToDelete
	if(index >= 0 && index <= size)
	{
		EdgeNode* nodeToDelete;
		nodeToDelete = nodeArray[index].edgeHead;
		nodeArray[index].edgeHead = nodeArray[index].edgeHead->nextEdge;
		
		// deallocate edgehead
		nodeToDelete->nextEdge = NULL;
		delete nodeToDelete;
		nodeToDelete = NULL;
		}
}// end of removeNode

//------------------------------------------ buildGraph --------------------------------------------
// buildGraph builds up graph node information and adjecency list of edges between
// each node reading from a data file
//
// @param inFile-ifstream&-the file to be read
// @return nothing
//--------------------------------------------------------------------------------------------------
void GraphL::buildGraph(ifstream& inFile)
{
	int source = 0;
	int adjNode = 0;
	
	// read the size
	inFile >> size;
	inFile.get();
	
	// a loop to initialize the node information
	if(size > 0)
	{
		for(int i = 0; i <= size; i++)
		{
			nodeArray[i].data = new NodeData;
			nodeArray[i].data->setData(inFile);
			nodeArray[i].visited = false;
			nodeArray[i].edgeHead = NULL;
		}

		// a loop to insert edge between 2 nodes
		for(;;)
		{
			inFile >> source >> adjNode;
			
			if(source == 0 || adjNode == 0) // end of a graph
				break;	
			
			insertEdge(source, adjNode); // call insertEdge
		}		
	}
	else //take care of negative size
	{
		while(!inFile.eof())
		{
			string junk = "";
			inFile >> junk;
		}
	}
}// end of buildGraph

//----------------------------------------- insertEdge ---------------------------------------------
// insertEdge inserts an edge between 2 given nodes
//
// @param source-const int-the starting node
// @param destination-const int-the ending node
// @return nothing
//--------------------------------------------------------------------------------------------------
void GraphL::insertEdge(int source, int adjNode)
{
	EdgeNode* newEdgeNode = new EdgeNode;
	newEdgeNode->adjGraphNode = adjNode;
	
	if(nodeArray[source].edgeHead == NULL) // empty list
	{ 
		newEdgeNode->nextEdge = NULL;
		nodeArray[source].edgeHead = newEdgeNode;
	}
	else
	{
		newEdgeNode->nextEdge = nodeArray[source].edgeHead;
		nodeArray[source].edgeHead = newEdgeNode;
	}
}

//----------------------------------------- depthFirstSearch ---------------------------------------
// depthFirstSearch find the shortest path between two 2 nodes in a graph using depth first traversal
// It calls dfsHelper to perform part of this task.
//
// @param none
// @return nothing
//--------------------------------------------------------------------------------------------------
void GraphL::depthFirstSearch()
{
	// set all node to unvisited
	for(int i = 1; i <= size; i++)
		nodeArray[i].visited = false;
	
	cout << "Depth-fisrt ordering: ";
	
	// Recursively call dfsHelper to visit all univited nodes that are adjacent to the current node
	for(int i = 1; i <= size; i++)
		if(!nodeArray[i].visited)
			dfsHelper(i); //call dfsHelper
		
	cout << endl << endl;
}// end of depthFirstSearch

//--------------------------------------------- dfsHelper ------------------------------------------
// dfsHelper is a helper method for DepthFirstSearch. It recursively call itself to perform the task
//
//--------------------------------------------------------------------------------------------------
void GraphL::dfsHelper(int currentNode)
{
	// mark currentNode as visited
	nodeArray[currentNode].visited = true;
	cout << currentNode << " ";
	
	// for each node that is adjacent to current node, recursively call the method to visit all
	// unvisited nodes that are adjacent to cur
	EdgeNode* cur = nodeArray[currentNode].edgeHead;
	while(cur != NULL)
	{
		if(!nodeArray[cur->adjGraphNode].visited)
			dfsHelper(cur->adjGraphNode); // recursive call
		cur = cur->nextEdge;
	}
}// end of dfsHelper

//------------------------------------------ displayGraph ------------------------------------------
// displayGraph displays the path from node _ to node _
// 
// @return nothing
//--------------------------------------------------------------------------------------------------
void GraphL::displayGraph()
{
	// a loop to display all nodes information
	cout << "Graph:" << endl;
	for(int i = 1; i <= size; i++)
	{
		cout << "Node" << i;
		cout << "       ";
		cout << *nodeArray[i].data << endl << endl;
		
		if(nodeArray[i].edgeHead != NULL)// list not empty
		{
			EdgeNode* cur;
			cur = nodeArray[i].edgeHead;
			
			while(cur != NULL)// there is another edge
			{
				cout << setw(8) << "edge " << i << " " << cur->adjGraphNode << endl;
				cur = cur->nextEdge;
			}
		}
	}
	cout << endl;
}

//------------------------------- enumerateSubgraph ------------------------------------------------
// Algorithm: ENUMERATESUBGRAPH(G,k) (ESU)
// Input: A graph G = (V,E) and an integer 1 <= k <= |V|
// Output: All size-k subgraphs in G
// for each vertex v in V do
// 		V_extension <-- {u in N({v}}: u > v}
//		call EXTENDSUBGRAPH({v}, V_extension, v)
//--------------------------------------------------------------------------------------------------
void GraphL::enumerateSubgraph(int k) // k is the size of subgraphs
{
	//loop to generate V_extension and call extendSubgraph
	for(int i = 0; i < MAXNODES; i++)
	{
        // PS. using maxnodes for the loop limit is not very efficient.
        // because there could be some index of the ajc list that has no element
        // in it.
        
        // It is easier to use either use the stack or queue of int.
        queue<int> Vextension;
        getExtension(i, Vextension);
        
        //GraphNode V_extension[MAXNODES]; // array of extension nodes

        vector<int> Vsubgraph;
        Vsubgraph[0] = i;
        
		extendSubgraph(Vsubgraph, Vextension, i, k); // call extendSubgraph
	}
}//end of enumeratedSubgraph

//------------------------------- extendSubgraph ---------------------------------------------------
// Algorithm: EXTENDSUBGRAPHS(V(subgraph), V(extension), v)
// if |V_subgraph| = k then output G |V_subgraph| and return
// while V_extension is not empty
//		remove an arbitrary chosen vertex w from V_extension
//		V'_extension <-- V_extension U {u in N_exclude(w, V_subgraph: u > v}
//		call EXTENDSUBGRAPH(V_subgraph U {w}, V'_extension,v)
//--------------------------------------------------------------------------------------------------
void GraphL::extendSubgraph(vector<int> Vsubgraph, queue<int> Vextension, int v, const int &k)
{
    if(Vsubgraph.size() == k)
    {
        // print Vsubgraph (have no decided how to display the subgraph.
		/*int i = 0;
		while(!Vsubgraph.size() == 0)
			cout << Vsubgraph[i++] << " ";*/
        return;
    }
    
    while( Vextension.size() != 0 )
    {
        int u = Vextension.front();
        Vextension.pop();
        
        Vsubgraph.push_back(u);
        
        getExtension(u, Vextension);
        extendSubgraph(Vsubgraph, Vextension, v, k);
    }
}


void GraphL::getExtension(const int &v, queue<int> Vextension){
    for(EdgeNode *w = nodeArray[v].edgeHead; w != NULL; w = w->nextEdge)
        Vextension.push(w->adjGraphNode);
}
