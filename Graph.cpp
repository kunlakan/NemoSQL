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
//          0   0       The last number in the file must end with 0 0 pair
//
//------------------------------------------------------------------------------


#include "Graph.h"

//--------------------------- A Default Constructor ----------------------------
// Default constructor for class Graph
// Preconditions: None
// Postconditions: - size is initialized to zero
//                 - Every VertexNode element in vertices will be initialized
//                   by its default constructor
Graph::Graph(){}

Graph::~Graph()
{
}

//--------------------------------- buildGraph ---------------------------------
// Builds a graph by reading data from an ifstream
// Preconditions:  infile has been successfully opened and the file contains
//                 properly formated data (according to the program specs)
// Postconditions: A graph is read from infile and stored in the object
void Graph::buildGraph(ifstream& infile)
{
    int src = -1, dest = -1;
    infile >> src >> dest;
    while (infile) {
        if(src != dest)
        {
            exist(src);
            exist(dest);
            
            vertices[src].push_back(dest);
            vertices[dest].push_back(src);
        }
        infile >> src >> dest;
    }
}

bool Graph::exist(const int &vertex){
    if(vertex < vertices.size())
        return true;
    
    for(long i = vertices.size(); i <= vertex; i++)
        vertices.push_back(*new list<int>);
    
    vertices[vertex] = *new list<int>;
    
    return false;
}




//--------------------------------- displayAll ---------------------------------
// Display a all detailed path
// Preconditions: vertices[vertexFrom] and its data must exist
// Postconditions: all detailed path is displayed
void Graph::displayAll() const
{
    cout << "From\t\t";
    cout << "To" << endl;
    
    for(int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].size() > 0) {
            cout << i << ": ";
            for (int vertice : vertices[i]) {
                cout << vertice << ", ";
            }
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
    //output file
    outfile.open("/Users/shokorakis/Desktop/Homework_3/Homework_3/output.txt");
    if(!outfile)
        cerr << "OUTPUT File could not be opend." << endl;
 
    count = 0;
    
    for(int i = 0; i <= vertices.size(); i++)
    {
        if(vertices[i].size() > 0){
            vector<int> Vsubgraph;
            unordered_set<int> visited;

            Vsubgraph.push_back(i);
            
            list<int> Vextension;
            for (int num : this->vertices[i]) {
                if (num > i) Vextension.push_back(num);
            }
            
            visited.insert(i);
            extendSubgraph(Vsubgraph, Vextension, visited, i, k);
        }
    }
    cerr << count << endl;
}

bool is_in_list(int target_num, list<int> &the_list) {
    for (int current_num : the_list) {
        if (target_num == current_num) return true;
    }
    return false;
}

//--------------------------- PRIVATE: extendSubgraph --------------------------
// Recursively looking size-k subgraphs of the graph.
// Precondition: The graph should have already been built or exists
// Postcondition: The list of subgraphs are displayed
void Graph::extendSubgraph(vector<int> &Vsubgraph, list<int> &Vextension, unordered_set<int> visited, const int &v, const int &k)
{
//    cout << "subgraph : ";
//    for (int vert : Vsubgraph) {
//        cout << vert << " ";
//    }
//    cout << "\tvisited : ";
//    for (int vert: visited) {
//        cout << vert << " ";
//    }
//    
//    cout << "\tneighbors : ";
//    for (int neighbor : Vextension) {
//        cout << neighbor << ", ";
//    }
//    cout << endl;
    // Display and write out the output
    if(Vsubgraph.size() == k)
    {
        for(int i = 0; i < Vsubgraph.size(); i++){
//            outfile << Vsubgraph[i] << " ";
//            cout << Vsubgraph[i] << " ";
        }
    
        count++;
        
//        outfile << "\n";
//        cout << "\n";
        return;
    }
    
    while(Vextension.size() != 0)
    {
        int w = Vextension.front();
        Vextension.pop_front();
        
        Vsubgraph.push_back(w);
        visited.insert(w);
    
        list<int> Vextension2 = list<int>(Vextension);
        
        for (int vertice : this->vertices[w]) {
            if (vertice > v
                && visited.count(vertice) == 0
                && !is_in_list(vertice, Vextension))
            {
                Vextension2.push_back(vertice);
            }
        }
        
        extendSubgraph(Vsubgraph, Vextension2, visited, v, k);
        Vsubgraph.pop_back();
    }
}


