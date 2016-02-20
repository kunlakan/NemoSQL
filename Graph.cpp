//---------------------------------------------------------------------------
//  Graph.cpp
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
//   -- size is total number vertices in the graph
//   -- Every of each VertexNode has ownership of its data stored
//   -- No graph ever share the same object (they could share the same value)
//   -- Negative edge's weight is allowed, but findShortestPath will not be
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
//          1 2 10                 # vertex1, vertex2, and itsweight
//          2 4 10
//          3 1  5
//          0 0  0                 # Signals the end of data
//---------------------------------------------------------------------------


#include "Graph.h"

//-------------------------- A Default Constructor --------------------------
// Default constructor for class Graph
// Preconditions: None
// Postconditions: - size is initialized to zero
//                 - Every VertexNode element in vertices will be initialized
//                   by its default constructor
//                 - Every Table element in T will be intialized by its default
//                   constructor
Graph::Graph()
{
    size = 0;
}


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
Graph::Graph(const Graph &otherGraph)
{
    size = otherGraph.size;
    
    // copies adjacency list vertices
    copyAllVertices(otherGraph.vertices);
    
    // copies table T
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            T[i][j].visited = otherGraph.T[i][j].visited;
            T[i][j].dist = otherGraph.T[i][j].dist;
            T[i][j].path = otherGraph.T[i][j].path;
        }
    }
}

//------------------------ PRIVATE: copyAllVertices -------------------------
// Copies every VertexNode element and all of its EdgeNode of othersVertices
// over to verticies sequentially vertices will have its ownership over all
// data and EdgeNode its stored.
// Preconditions: - size must be equal to size of otherVertices' graph
//                - otherVertices.edgeHead points to NULL or an EdgeNode
//                - otherVertices.data points to NULL or a GraphData
// Postcondition: vertices will contains the same value and structure as
//                otherGraph.vertices but with its ownership of all data
void Graph::copyAllVertices(const VertexNode *otherVertices)
{
    EdgeNode *otherEdgeHead = NULL;
    for(int i = 0; i < size; i++)
    {
        vertices[i].data = new GraphData(*otherVertices[i].data);
        
        otherEdgeHead = otherVertices[i].edgeHead;
        copyAllEdge(vertices[i].edgeHead, otherEdgeHead);
    }
}

//-------------------------- PRIVATE: copyAllEdge ---------------------------
// Creates a copy of every information recursively from otherEdge to thisEdge
// thisEdge will have its ownership over all of its data stored.
// Preconditions: - otherEdge points to either NULL or an EdgeNode
//                - thisEdge points to either NULL or an EdgeNode
// Postcondition: Every thisEdge will have the same value and structure as
//                otherEdge with its ownership of all data stored
void Graph::copyAllEdge(EdgeNode *&thisEdge, EdgeNode *&otherEdge)
{
    if(otherEdge != NULL)
    {
        thisEdge = new EdgeNode(otherEdge->adjVertex);
        copyAllEdge(thisEdge->nextEdge, otherEdge->nextEdge);
    }
}

//------------------------------- Destructor --------------------------------
// Destructor for class Graph
// Preconditions: - All of VertexNode.edgeHead points to NULL or an EdgeNode
//                - All VertexNode.data points to NULL or a GraphData
// Postconditions: All VertexNode's data and edgeHead and its list of
//                 EdgeNode are deallocated
Graph::~Graph()
{
    for(int i = 0; i < size; i++)
    {
        removeAllEdge(vertices[i].edgeHead);
        delete(vertices[i].data);
        vertices[i].data = NULL;
    }
}

//------------------------- PRIVATE: removeAllEdge --------------------------
// Removes all EdgeNode recursively
// Preconditions: currentEdge points to either NULL or an EdgeNode
// Postconditions: currentEdge->nextEdge, and currentEdge will be deallocated
//                 and set to NULL
void Graph::removeAllEdge(EdgeNode *&currentEdge)
{
    if(currentEdge != NULL)
    {
        removeAllEdge(currentEdge->nextEdge);
        delete(currentEdge->nextEdge);
        currentEdge->nextEdge = NULL;
        delete(currentEdge);
        currentEdge = NULL;
    }
}

//------------------------------- buildGraph --------------------------------
// Builds a graph by reading data from an ifstream
// Preconditions:  infile has been successfully opened and the file contains
//                 properly formated data (according to the program specs)
// Postconditions: One graph is read from infile and stored in the object
void Graph::buildGraph(ifstream& infile)
{
    infile >> size;                          // data member stores array size
    
    if (infile.eof())
        return;
    infile.ignore();                         // throw away '\n' go to next line
    
    char tempString[MAX_CHAR_LENGTH];

    for (int v = 0; v < size; v++)
    {
        infile.getline(tempString, MAX_CHAR_LENGTH, '\n');
  
		vertices[v].data = new GraphData(tempString);
    }
    
    int src = 1, dest = 1;
    for (;;) {
        infile >> src >> dest;
        if (src == 0 || infile.eof())
            break;
        insertEdge(src, dest);
    }
}

//------------------------------- insertEdge --------------------------------
// Insert an EdgeNode into the graph
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postconditions: - If tempEdge does not exist between the two vertices,
//                   tempEdge will be inserted
//                 - If source and destination are not in its range, function ends
void Graph::insertEdge(const int &source, const int &destination)
{
    int vertexFrom = source - 1;
    int vertexTo = destination - 1;
    
    if(vertexFrom != vertexTo && areInRange(vertexFrom, vertexTo))
    {
        EdgeNode *tempEdge = new EdgeNode(vertexTo);
        insertHelper(vertices[vertexFrom].edgeHead, tempEdge);
        tempEdge = NULL;
    }
}

//-------------------------- PRIVATE: insertHelper --------------------------
// Recursively looking if the edge already exist inside the list. If so, the
// existing weight will be changed to a new weight. Otherwise, inserts edge
// at the end of the list.
// Preconditions: current and edge points to NULL or a first EdgeNode
// Postconditions: - If edge does not already exist in the list, it will be
//                   inserted to the end of the list
//                 - If edge already exist in the list, found edge will be
//                   replaced its weight with with edge->weigth
void Graph::insertHelper(EdgeNode *&current, EdgeNode *&edge)
{
    if(current == NULL)
    {
        edge->nextEdge = current;
        current = edge;
    }
    else if(current->adjVertex == edge->adjVertex)
        current->weight = edge->weight;
    else
        insertHelper(current->nextEdge, edge);
}

//------------------------------- removeEdge --------------------------------
// Remove an EdgeNode out of the graph
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postcondition: - If EdgeNode of destination does not exist in the list,
//                  nothing will happen
//                - If EdgeNode of destination is found, it gets removed
void Graph::removeEdge(const int &source, const int &destination)
{
    int vertexFrom = source - 1;
    int vertexTo = destination - 1;
    
    if(vertexFrom != vertexTo && areInRange(vertexFrom, vertexTo))
    {
        removeHelper(vertices[vertexFrom].edgeHead, vertexTo);
    }
}

//-------------------------- PRIVATE: removeHelper --------------------------
// Recursively looking for the EdgeNode containing destination to remove
// Preconditions: current points to NULL or an EdgeNode
// Postconditions: If EdgeNode containing destination is found, it will be
//                 deallocated. Otherwise, funtions ends
void Graph::removeHelper(EdgeNode *&current, int destination)
{
    if(current == NULL)
        return;
    else if(current->adjVertex == destination)
    {
        EdgeNode *temp = current;
        current = temp->nextEdge;
        delete(temp);
        temp = NULL;
    }
    else
        removeHelper(current->nextEdge, destination);
}

//---------------------------- findShortestPath -----------------------------
// Finds the shortest path from all vertices to another vertices.
// Preconditions: - All VertexNode.edgeHead points to NULL or an EdgeNode
//                - All VertexNode.data points to NULL or a GraphData
// Postcondition: distance and path of all vertex to other vertex are found
//                and stored in the table T
void Graph::findShortestPath()
{
    resetTable();
    
    for(int i = 0; i < size; i++)
    {
        dijkstraHelper(i);
    }
}

//--------------------------- PRIVATE: resetTable ---------------------------
// Resets table T
// Preconditions: None
// Postconditions: All Table T.visited is resetted to false, T.dist to
//                 INT_MAX, and T.path to 0
void Graph::resetTable()
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            T[i][j].visited = false;
            T[i][j].dist = INT_MAX;
            T[i][j].path = 0;
        }
    }
}

//------------------------- PRIVATE: dijkstraHelper -------------------------
// Dijkstra's algorithm of finding the shortest path. priority queue is used
// in this function. Note that If some of the edge has a nevagive weight,
// this algorithm will not be efficient and reliable
// Preconditions: - VertexNode in vertices and its data must exist
//                - All VertexNode.edgeHead points to NULL or an EdgeNode
// Postconditions: distance and path of every vertex to every other vertex are
//                 found and stored in the table T
void Graph::dijkstraHelper(const int &source)
{
    int newDistance = 0;
    int v = 0;

    priority_queue<pair<int, int>, vector<pair<int,int>>, Comparator> nextVisit;
    nextVisit.push(make_pair(source, T[source][source].dist));
    
    T[source][source].path = -1;        // The path from the source to itself is defined to be -1
    T[source][source].dist = 0;         // The distance from the source to itself is defined to be zero
    
    // Repeat size - 1 times
    for(int i = 0; i < size-1 && !nextVisit.empty(); i++)
    {
        // Let v be the unvisited vertex with the minimum distance from the source
        v = nextVisit.top().first;
        nextVisit.pop();
        
        cout << "VISITED " << v+1 << endl;
                
        // Mark v as visited
        T[source][v].visited = true;

        // For each unvisited vertex w that is adjacent to v
        for(EdgeNode *w = vertices[v].edgeHead; w != NULL; w = w->nextEdge)
        {
            if(!T[source][w->adjVertex].visited)
            {
                newDistance = T[source][v].dist + w->weight;
                
                // If Dw > Dv + Dv,w
                if(newDistance < T[source][w->adjVertex].dist)
                {
                    // Set Dw = Dv + Dv,w
                    T[source][w->adjVertex].dist = newDistance;
                    
                    // Remember going through v to get w
                    T[source][w->adjVertex].path = v;
                    
                    nextVisit.push(make_pair(w->adjVertex, T[source][w->adjVertex].dist));
                }
            }
        }
        
        // Fiding the next shortest unvisited vertex with minimum distance from the source
        if(!nextVisit.empty())
        {
            while(T[source][nextVisit.top().first].visited && nextVisit.size() > 1)
                nextVisit.pop();
        }
    }
}

//-------------------------------- displayAll -------------------------------
// Display a all detailed path
// Preconditions: vertices[vertexFrom] and its data must exist
// Postconditions: all detailed path is displayed
void Graph::displayAll() const
{
    cout << "Description\t\t\t\t\t";
    cout << "From\t";
    cout << "To\t\t";
    cout << "Distance\t";
    cout << "Path" << endl;

    for(int i = 0; i < size; i++)
    {
        diplayAllHelper(i);
    }
}

//------------------------ PRIVATE: displayAllHelper ------------------------
// Recursively display a all detailed path
// Preconditions: vertices[vertexFrom] and its data must exist
// Postconditions: All detailed path is displayed
void Graph::diplayAllHelper(const int &source) const
{
    cout << *vertices[source].data << endl;

    for(int i = 0; i < size; i++)
    {
        if(source != i){
            cout << "\t\t\t\t\t\t\t";
            cout << (source+1) << "\t\t";
            cout << (i+1) << "\t\t";
            
            if(T[source][i].dist == INT_MAX)
            {
                cout << "--" << "\t\t\t";
                cout << "--" << endl;
            }
            else
            {
                cout << T[source][i].dist << "\t\t\t";
                displayPath(source, T[source][i].path, i);
                cout << endl;
            }
        }
    }
}

//--------------------------------- display ---------------------------------
// Display a single detailed path
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postconditions: - If source and destination is not in the range, an error
//                   message is shown
//                 - a single detailed path is displayed with its locations'
//                   description
void Graph::display(const int &source, const int &destination) const
{
    int vertexFrom = source - 1;
    int vertexTo = destination - 1;

    
    if(!areInRange(vertexFrom, vertexTo))
        cout << "DISPLAY ERROR: No path exists" << endl;
    else
    {
        cout << (source) << "\t";
        cout << (destination) << "\t";
    
        if(vertexFrom == vertexTo)
        {
            cout << T[vertexFrom][vertexTo].path+1 << "\t";
            cout << source << endl;
            cout << *vertices[vertexFrom].data;
        }
        else if(T[vertexFrom][vertexTo].dist == INT_MAX)
        {
            cout << "--" << "\t\t";
            cout << "--" << endl;
        }
        else
        {
            cout << T[vertexFrom][vertexTo].dist << "\t";
            
            displayPath(vertexFrom, T[vertexFrom][vertexTo].path, vertexTo);
            cout << endl;
            displayPathName(vertexFrom, T[vertexFrom][vertexTo].path, vertexTo);
        }
    }
}

//-------------------------- PRIVATE: displayPath ---------------------------
// Recursively display shortest path
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postconditions: shortest path is displayed
void Graph::displayPath(const int &source, const int &path, const int &destination) const
{
    if(path == -1)
        cout << destination+1;
    else
    {
        displayPath(source, T[source][path].path, path);
        cout << " " << destination+1;
    }
}

//------------------------ PRIVATE: displayPathName -------------------------
// Recursively display shortest path's location description
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postconditions: shortest path's location description is displayed
void Graph::displayPathName(const int &source, const int &path, const int &destination) const
{
    if(path == -1)
        cout << *vertices[destination].data << endl;
    else
    {
        displayPathName(source, T[source][path].path, path);
        cout << *vertices[destination].data << endl;
    }
}

//--------------------------- PRIVATE: areInRange ---------------------------
// Checks if source and destination are in the range of 0 to size
// Preconditions: None
// Postconditions: true is returned if the source and destination are in the
//                 range. Otherwise, false is returned
bool Graph::areInRange(const int &source , const int &destination) const
{
    bool sourceInRange = (0 <= source) && (source < size);
    bool destInRange = (0 <= destination) && (destination < size);
    
    return sourceInRange && destInRange;
}



void Graph::enumerateSubgraph(const int &k) // k is the size of subgraphs
{

    cout << "size " << size << endl;
    for(int i = 0; i <= size; i++)
    {
        vector<int> Vsubgraph;
        Vsubgraph.push_back(i);
        list<int> Vextension = getExtension(i, Vextension);
        
        extendSubgraph(Vsubgraph, Vextension, i, k); // call extendSubgraph
    }
}

void Graph::extendSubgraph(vector<int> Vsubgraph, list<int> &Vextension, int v, const int &k)
{
    if(Vsubgraph.size() == k)
    {
        for(int i = 0; i < Vsubgraph.size(); i++)
            cout << Vsubgraph[i] + 1 << " ";
        cout << endl;
        return;
    }
    
    while(Vextension.size() != 0 && Vsubgraph.size() < k)
    {
        int w = Vextension.front();	// remove an arbitrary w from Vextension
        
        Vextension.pop_front();		// Vextension = Vextension - w
        Vsubgraph.push_back(w);		// add w to Vsubgraph
        
        list<int> Vextension2 = getExtension(w, Vextension);
        extendSubgraph(Vsubgraph, Vextension2, v, k);
        
        Vsubgraph.pop_back();
    }
}

list<int> Graph::getExtension(const int &v, const list<int>& Vextension) const{
    list<int> newExtension = Vextension;
    
    for(EdgeNode *w = vertices[v].edgeHead; w != NULL; w = w->nextEdge)
    {
        if(w->adjVertex > v)
        {
            if(!isDuplicate(w->adjVertex, newExtension))
                newExtension.push_back(w->adjVertex);
        }
    }

    return newExtension;
}

bool Graph::isDuplicate(const int &target, const list<int>& Vextension) const
{
    for(list<int>::const_iterator it = Vextension.begin(); it != Vextension.end(); it++)
    {
        if(target == *it)
            return true;
    }
    return false;
}

