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


//----------------------------- A Copy Constructor -----------------------------
// Copy constructor for class Graph
// Preconditions: - All of VertexNode.edgeHead in otherGraph's vertices points
//                  to NULL or an EdgeNode
//                - All of VertexNode.data in otherGraph's vertices points to
//                  NULL or a GraphData
// Postconditions: - size is equal to otherGraph.size
//                 - vertices will contains the same value and order as
//                   otherGraph.vertices but with its ownership of all data
Graph::Graph(const Graph &otherGraph)
{
    // copies adjacency list vertices
    copyAllVertices(otherGraph.vertices);
}

//-------------------------- PRIVATE: copyAllVertices --------------------------
// Copies every VertexNode element and all of its EdgeNode of othersVertices
// over to verticies sequentially vertices will have its ownership over all
// data and EdgeNode its stored.
// Preconditions: - size must be equal to size of otherVertices' graph
//                - otherVertices.edgeHead points to NULL or an EdgeNode
//                - otherVertices.data points to NULL or a GraphData
// Postcondition: vertices will contains the same value and structure as
//                otherGraph.vertices but with its ownership of all data
void Graph::copyAllVertices(const vector<VertexNode> &otherVertices)
{
    EdgeNode *otherEdgeHead = NULL;
    for(int i = 0; i < vertices.size(); i++)
    {
        vertices[i].data = new GraphData(*otherVertices[i].data);
        
        otherEdgeHead = otherVertices[i].edgeHead;
        copyAllEdge(vertices[i].edgeHead, otherEdgeHead);
    }
}

//---------------------------- PRIVATE: copyAllEdge ----------------------------
// Creates a copy of every information recursively from otherEdge to thisEdge.
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

//--------------------------------- Destructor ---------------------------------
// Destructor for class Graph
// Preconditions: - All of VertexNode.edgeHead points to NULL or an EdgeNode
//                - All VertexNode.data points to NULL or a GraphData
// Postconditions: All VertexNode's data and edgeHead and its list of
//                 EdgeNode are deallocated
Graph::~Graph()
{
    for(int i = 0; i < vertices.size(); i++)
    {
        removeAllEdge(vertices[i].edgeHead);
        delete(vertices[i].data);
        vertices[i].data = NULL;
    }
}

//--------------------------- PRIVATE: removeAllEdge ---------------------------
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

//--------------------------------- buildGraph ---------------------------------
// Builds a graph by reading data from an ifstream
// Preconditions:  infile has been successfully opened and the file contains
//                 properly formated data (according to the program specs)
// Postconditions: A graph is read from infile and stored in the object
void Graph::buildGraph(ifstream& infile)
{
    if (infile.eof())
        return;
    //infile.ignore();                         // throw away '\n' go to next line
    
    //    char tempString[MAX_CHAR_LENGTH];
    //
    //    for (int v = 0; v < size; v++)
    //    {
    //        infile.getline(tempString, MAX_CHAR_LENGTH, '\n');
    //
    //		vertices[v].data = new GraphData(tempString);
    //    }
    
    int src = 1, dest = 1;
    for (;;) {
        infile >> src >> dest;
        if (src == 0 || infile.eof())
            break;
        insertEdge(src, dest);
    }
    
}


//--------------------------------- insertEdge ---------------------------------
// Insert an EdgeNode into the graph
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postconditions: - If tempEdge does not exist between the two vertices,
//                   tempEdge will be inserted
//                 - If source and destination are not in its range, the
//                   function will end
void Graph::insertEdge(const int &source, const int &destination)
{
    if(source != destination && areInRange(source, destination))
    {
        exist(source);
        exist(destination);
        
        EdgeNode *tempEdge = new EdgeNode(destination);
        insertHelper(vertices[source].edgeHead, tempEdge);
        
        tempEdge = new EdgeNode(source);
        insertHelper(vertices[destination].edgeHead, tempEdge);
        
        tempEdge = NULL;
    }
}

bool Graph::exist(const int &vertex){
    if(vertex < vertices.size() && vertices[vertex].data != NULL)
        return true;
    
    for(long i = vertices.size(); i <= vertex; i++)
        vertices.push_back(*new VertexNode());
    
    vertices[vertex].data = new GraphData(to_string(vertex));
    
    return false;
}


//---------------------------- PRIVATE: insertHelper ---------------------------
// Recursively looking if the edge already exist inside the list. If so, ignore
// the insertion. Otherwise, inserts edge at the end of the list.
// Preconditions: current and edge points to NULL or a first EdgeNode
// Postconditions: - If edge does not already exist in the list, it will be
//                   inserted to the end of the list
//                 - If edge already exist in the list, the insertion will be
//                   ignored
void Graph::insertHelper(EdgeNode *&current, EdgeNode *&edge)
{
    if(current == NULL)
    {
        edge->nextEdge = current;
        current = edge;
    }
    else if(current->adjVertex == edge->adjVertex)
        return;
    else
        insertHelper(current->nextEdge, edge);
}

//--------------------------------- removeEdge ---------------------------------
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

//--------------------------- PRIVATE: removeHelper ----------------------------
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
        if(vertices[i].data != NULL)
            diplayAllHelper(i);
    }
}

//------------------------- PRIVATE: displayAllHelper --------------------------
// Recursively display a all detailed path
// Preconditions: vertices[vertexFrom] and its data must exist
// Postconditions: All detailed path is displayed
void Graph::diplayAllHelper(const int &source) const
{
    cout << *vertices[source].data;
    if(source < 1000)
        cout << "\t";
    cout << "\t\t";
    
    for(EdgeNode *v = vertices[source].edgeHead; v != NULL; v = v->nextEdge){
        cout << v->adjVertex;
        
        if(v->adjVertex < 1000)
            cout << "\t";
        
        cout << "\t";
    }
    cout << endl;
}

//----------------------------------- display ----------------------------------
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
    }
}


//------------------------------ enumerateSubgraph -----------------------------
// Enumerate size-k subgraphs of the original graph
// Preconditions: The graph should have already been built or exists
// Postcondition: The list of subgraphs are displayed
void Graph::enumerateSubgraph(const int &k)
{
    count = 0;
    outfile.open("/Users/shokorakis/Desktop/Homework_3/Homework_3/output.txt");
    if(!outfile)
        cerr << "File could not be opend." << endl;
    
    for(int i = 0; i <= vertices.size(); i++)
    {
        if(vertices[i].data != NULL){
            vector<int> Vsubgraph;
            Vsubgraph.push_back(i);
            list<int> Vextension = getExtension(i, Vextension);
            
            extendSubgraph(Vsubgraph, Vextension, i, k);
        }
    }
    cout << count << endl;
}

//--------------------------- PRIVATE: extendSubgraph --------------------------
// Recursively looking size-k subgraphs of the graph.
// Precondition: The graph should have already been built or exists
// Postcondition: The list of subgraphs are displayed
void Graph::extendSubgraph(vector<int> Vsubgraph, list<int> &Vextension, int v, const int &k)
{
    if(Vsubgraph.size() == k)
    {
        for(int i = 0; i < Vsubgraph.size(); i++){
            outfile << Vsubgraph[i] << " ";
        }
    
        count++;
        
        outfile << "\n";
        return;
    }
    
    while(Vextension.size() != 0 && Vsubgraph.size() < k)
    {
        int w = Vextension.front();
        
        Vextension.pop_front();
        Vsubgraph.push_back(w);
        
        list<int> Vextension2 = getExtension(v, w, Vextension);
        extendSubgraph(Vsubgraph, Vextension2, v, k);
        
        Vsubgraph.pop_back();
    }
}

//--------------------------- PRIVATE: getExtension ----------------------------
// Create a list contain all neighbors of v
// Precondition: None
// Postcondition: list of v's neighbors is returned
list<int> Graph::getExtension(const int &v, const list<int>& Vextension) const
{
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

list<int> Graph::getExtension(const int &v, const int &w, const list<int>& Vextension) const
{
    list<int> newExtension = Vextension;
    
    vector<int> neighbore = getExclusiveNeighbore(v, w);
    for( int i = 0; i < neighbore.size(); i++)
    {
        if(neighbore[i] > v)
        {
            if(!isDuplicate(neighbore[i], newExtension))
                newExtension.push_back(neighbore[i]);
        }
    }
    
    return newExtension;
}

vector<int> Graph::getExclusiveNeighbore(const int&v, const int&w) const{
    vector<int> exclusiveNeighbore;
    
    for(EdgeNode *i = vertices[w].edgeHead; i != NULL; i = i->nextEdge)
    {
        bool isExclusive = true;
        for(EdgeNode *j = vertices[v].edgeHead; j != NULL; j = j->nextEdge)
        {
            if( i->adjVertex == j->adjVertex ){
                isExclusive = false;
                break;
            }
        }
    
        if( isExclusive )
            exclusiveNeighbore.push_back(i->adjVertex);
    }
    
    return exclusiveNeighbore;
}
//---------------------------- PRIVATE: isDuplicate ----------------------------
// Checks if target already exists in the Vextension
// Preconditions: None
// Postcondition: - true is return if target is contained in Vextension
//                - false is return if target is not contained in Vextension
bool Graph::isDuplicate(const int &target, const list<int>& Vextension) const
{
    for(list<int>::const_iterator it = Vextension.begin(); it != Vextension.end(); it++)
    {
        if(target == *it)
            return true;
    }
    return false;
}


//----------------------------- PRIVATE: areInRange ----------------------------
// Checks if source and destination are in the range of 0 to size
// Preconditions: None
// Postconditions: true is returned if the source and destination are in the
//                 range. Otherwise, false is returned
bool Graph::areInRange(const int &source , const int &destination) const
{
    bool sourceInRange = (0 <= source);
    bool destInRange = (0 <= destination);
    
    return sourceInRange && destInRange;
}