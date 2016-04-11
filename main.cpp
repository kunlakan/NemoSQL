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

#include <chrono>
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
    ifstream infile1("./input/Ecoli20111027CR_idx.txt");
    if (!infile1) {
        cerr << "File could not be opened." << endl;
        return 1;
    }
    
    Graph G;
    G.buildGraph(infile1);
    //infile1.close();
    
    //G.displayAll();
    auto start = chrono::high_resolution_clock::now();
    //G.enumerateSubgraph(3);
//    G.enumerateSubgraph(4);
    G.enumerateSubgraph(5);
    
    auto end = chrono::high_resolution_clock::now();
    auto timeInSec = end - start;
    cout << "Run Time = " << chrono::duration_cast<chrono::milliseconds>(timeInSec).count();
    
    cout << endl;
    
    return 0;
}