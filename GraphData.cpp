//------------------------------------------------------------------------------
//  GraphData.h
//  Created by Kunlakan Cherdchusilp on 1/30/15.
//------------------------------------------------------------------------------
// GraphData represents an Object that stores a string as a data.
// Additional features including:
//  -- allow assignment of 2 GraphData
//  -- allow output and input of GraphData
//------------------------------------------------------------------------------

#include "GraphData.h"


//---------------------------- A Default Constructor ---------------------------
// Default constructor for class  GraphData
// Preconditions: None
// Postconditions: data is set to an empty string ("")
GraphData::GraphData()
{
    data = "";
}

//------------------------- A One Parameter Constructor ------------------------
// One parameter constructor for class GraphData
// Preconditions: None
// Postcontitions: this->data is set to data
GraphData::GraphData(const string &data)
{
    this->data = data;
}

//---------------------------------- getData -----------------------------------
// Gets value of data.
// Preconditions: None
// Postconditions: data is returned
string GraphData::getData() const
{
    return data;
}

//----------------------------------- setData ----------------------------------
// Sets value of data.
// Preconditions: None
// Postconditions: data is set to newData
void GraphData::setData(const string &newData)
{
    data = newData;
}


//------------------------------- Output Operator ------------------------------
// Overloaded output operator for class GraphData
// Preconditions: None
// Postconditions: data is sent to the output istream
ostream& operator<<(ostream &outStream, const GraphData &graphData)
{
    outStream << graphData.data;
    return outStream;
}

//------------------------------- Input Operator -------------------------------
// Overloaded input operator for class GraphData
// Preconditions: None
// Postconditions: data is set to the the inStream
istream& operator>>(istream &inStream, GraphData &graphData)
{
    inStream >> graphData.data;
    return inStream;
}

