//------------------------------------------------------------------------------
// main.cpp
// Created by Clark Olson and Carol Zander
// Modified by Kunlakan (Jeen) Cherdchusilp
//------------------------------------------------------------------------------
// This is a driver for Motif Decection program
//
// Assumptions:
//   -- the "input.txt" text file must exist in the same directory as this
//      programand, and it must be formatted as described in the specifications
//      stated in Graph.h
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "Graph.h"

using namespace std;

//-------------------------- main ----------------------------------------------
// The main driver of the Motif Detection program
// Preconditions:   If input.txt file exists, it must be formatted as described
//                  in the specifications stated in Graph.h
// Postconditions:  - The graph of the input will be generated
//                  - The k-size subgraphs with be generated as called
int main() {
    ifstream infile1("/Users/NewOwner/Documents/NetworkMotifs/input.txt");
    if (!infile1) {
        cerr << "File could not be opened." << endl;
        return 1;
    }
	Graph G;
	int s = G.getSize(infile1);	// read once to get the size of the graph
	infile1.clear();	// clear the buffer
	infile1.seekg(0);	// reset the reading position
    for(;;){
        G.buildGraph(infile1, s);
        if (infile1.eof())
            break;
        
        G.enumerateSubgraph(3);
        G.enumerateSubgraph(4);
        G.enumerateSubgraph(5);
        
        cout << endl;
    }
	infile1.close();
    
    return 0;
}