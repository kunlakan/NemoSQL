//---------------------------------------------------------------------------
//  GraphData.h
//  Created by Kunlakan Cherdchusilp on 1/30/15.
//---------------------------------------------------------------------------
// GraphData represents an Object class that stores a string as a data.
// Additional features including:
//  -- allow assignment of 2 GraphData
//  -- allow output and input of GraphData

#ifndef __Homework_3__GraphData__
#define __Homework_3__GraphData__

#include <iostream>
#include <string>
using namespace std;

class GraphData
{
    //------------------------ Output Operator ----------------------------------
    // Overloaded output operator for class GraphData
    // Preconditions: None
    // Postconditions: data is sent to the output istream
    friend ostream& operator<<(ostream &outStream, const GraphData &Graphdata);
    
    //------------------------- Input Operator ----------------------------------
    // Overloaded input operator for class GraphData
    // Preconditions: None
    // Postconditions: data is set to the the inStream
    friend istream& operator>>(istream &inStream, GraphData &Graphdata);
    
    
public:
    //-------------------------- A Default Constructor --------------------------
    // Default constructor for class  GraphData
    // Preconditions: None
    // Postconditions: data is set to an empty string ("")
    GraphData();
    
    //----------------------- A One Parameter Constructor -----------------------
    // One parameter constructor for class GraphData
    // Preconditions: None
    // Postcontitions: this->data is set to data
    GraphData(const string &data);
    
    
    //--------------------------------- getData ---------------------------------
    // Gets value of data.
    // Preconditions: None
    // Postconditions: data is returned
    string getData() const;
    
    //--------------------------------- setData ---------------------------------
    // Sets value of data.
    // Preconditions: None
    // Postconditions: data is set to newData
    void setData(const string &newData);
    
private:
    string data;
};

#endif /* defined(__Homework_3__GraphData__) */
