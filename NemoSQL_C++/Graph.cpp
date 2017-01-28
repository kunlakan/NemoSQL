//------------------------------------------------------------------------------
//  Graph.cpp
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


#include "Graph.h"

//--------------------------- A Default Constructor ----------------------------
// Default constructor for class Graph
// Preconditions: None
// Postconditions: None
Graph::Graph(){}

//--------------------------------- Destructor ---------------------------------
// Destructor for class Graph
// Preconditions: None
// Postconditions: None
Graph::~Graph()
{}


//--------------------------------- buildGraph ---------------------------------
// Builds a graph by reading data from an ifstream
// Preconditions:  infile has been successfully opened and the file contains
//                 properly formated data (according to the program specs)
// Postconditions: A graph is read from infile and stored in the object
void Graph::buildGraph(ifstream& infile)
{
    int src = -1, dest = -1;
    
    infile >> src >> dest;
    
    while (infile)
    {
        if(src != dest)
        {
            exist(src);
            exist(dest);
            
            vertices[src].insert(dest);
            vertices[dest].insert(src);
        }
        
        infile >> src >> dest;
    }
}

//------------------------------- PRIVATE: exist -------------------------------
// Check if vertex already exists in the vector vertices
// Preconditions: None
// Postconditions: If vertex already exists in the vector vertices, then do
//                 nothing. Otherwise, add vertex to the vector vertices.
void Graph::exist(const int &vertex){
    if(vertex < vertices.size())
        return;
    
    for(long i = vertices.size(); i <= vertex; i++)
        vertices.push_back(*new unordered_set<int>);
    
    vertices[vertex] = *new unordered_set<int>;
}


//---------------------------------- display -----------------------------------
// Display a all detailed path
// Preconditions: vertices[vertexFrom] and its data must exist
// Postconditions: all detailed path is displayed
void Graph::display() const
{
    cout << "From\t\t";
    cout << "To" << endl;
    
    for(int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].size() > 0)
        {
            cout << i << ": ";
            
            for (int vertice : vertices[i])
                cout << vertice << " ";
            
            cout << endl;
        }
    }
}

//------------------------------ enumerateSubgraph -----------------------------
// Enumerate size-k subgraphs of the original graph
// Preconditions: The graph should have already been built or exists
// Postcondition: The list of subgraphs are displayed
void Graph::enumerateSubgraph(const int &k)
{
    count = 0;
    
    for(int i = 0; i <= vertices.size(); i++)
    {
        if(vertices[i].size() > 0)
        {
            list<int> Vsubgraph;
            Vsubgraph.push_back(i);
            
            unordered_set<int> visited;
            visited.insert(i);
            
            unordered_set<int> Vextension;
            getExtension(Vextension, i);
            
            extendSubgraph(Vsubgraph, Vextension, visited, i, k);
        }
    }
    cerr << count << endl;
}

//--------------------------- PRIVATE: getExtension ----------------------------
// Add all neighbors of vertex to Vextension
// Precondition: Vextension must be decalired and intialized before calling
//               this function.
// Postcondition: All neighbors of vertex are added to Vextension
void Graph::getExtension(unordered_set<int> &Vextension, const int &vertex)
{
    for (int i : vertices[vertex])
    {
        if (i > vertex)
            Vextension.insert(i);
    }
}


//--------------------------- PRIVATE: extendSubgraph --------------------------
// Recursively looking size-k subgraphs of the graph.
// Precondition: The graph should have already been built or exists
// Postcondition: The list of subgraphs are displayed
void Graph::extendSubgraph(list<int> &Vsubgraph, unordered_set<int> &Vextension, unordered_set<int> &visited, const int &v, const int &k)
{
    if(Vsubgraph.size() == k-1)
    {
        for(int w : Vextension)
            count++;
        
        return;
    }
    
    list<int> unvisit;
    
    while(Vextension.size() != 0)
    {
        int w = *Vextension.cbegin();
        Vextension.erase(w);
        
        Vsubgraph.push_back(w);
        visited.insert(w);
        
        unvisit.push_back(w);
        
        unordered_set<int> Vextension2 = unordered_set<int>(Vextension);
        
        for (int vertex : vertices[w])
        {
            if (vertex > v && visited.count(vertex) == 0 && Vextension.count(vertex) == 0)
                Vextension2.insert(vertex);
        }
        
        extendSubgraph(Vsubgraph, Vextension2, visited, v, k);
        Vsubgraph.pop_back();
    }

    for(int i : unvisit)
        visited.erase(i);

}
