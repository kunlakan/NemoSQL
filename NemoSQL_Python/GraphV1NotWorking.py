'''
Attempt:
Get all the neighbors of the last column of previous table where table > that last column.

Why it fails:
Big node in between two smaller nodes will cut the two smaller nodes from each other.
'''

import sqlite3
import datetime

connection = sqlite3.connect(':memory:')

k = 0
count = 0
table_names = {}
table_names_2 = {}
cursor = connection.cursor()
vertices = []

# ------------------------------------ main ------------------------------------
def main():
    #buildGraph("input/Scere20101010CR_idx.txt")
    buildGraph("input/Original_Input.txt")

    # start = datetime.datetime.now()
    # enumerateSubgraph(3)
    # end = datetime.datetime.now()
    # print "Time : ", (end - start).total_seconds() * 1000

    start = datetime.datetime.now()
    enumerateSubgraph(4)
    end = datetime.datetime.now()
    print "Time : ", (end - start).total_seconds() * 1000

    # start = datetime.datetime.now()
    # enumerateSubgraph(5)
    # end = datetime.datetime.now()
    # print "Time : ", (end - start).total_seconds() * 1000


# --------------------------------- buildGraph ---------------------------------
# Builds a graph by reading data from the given file
# Preconditions:    file has been successfully opened and contains property
#                   formatted data (according to the program specs)
# Postconditions:   A graph is read from infile and stored in the object
def buildGraph(file):
    global vertices

    cursor.execute("CREATE TABLE VERTICES (node INT NOT NULL, neighborTable TEXT NOT NULL, PRIMARY KEY(node))")

    with open(file, "r") as inputFile:
        for line in inputFile:
            data = line.strip().split()
            nodeFrom = data[0]
            nodeTo = data[1]

            if nodeFrom not in table_names:
                table_names[int(nodeFrom)] = "SELECT neighbor FROM _" + nodeFrom + " WHERE neighbor > ?"
            if nodeTo not in table_names:
                table_names[int(nodeTo)] = "SELECT neighbor FROM _" + nodeTo + " WHERE neighbor > ?"

            neighborTableName = nodeFrom
            cursor.execute("INSERT OR IGNORE INTO VERTICES VALUES (?, ?)", (nodeFrom, neighborTableName))
            cursor.execute("CREATE TABLE IF NOT EXISTS _" + neighborTableName + " (neighbor INT NOT NULL, PRIMARY KEY(neighbor))")
            cursor.execute("INSERT OR IGNORE INTO _" + neighborTableName + " VALUES (?)", [nodeTo])

            neighborTableName = nodeTo
            cursor.execute("INSERT OR IGNORE INTO VERTICES VALUES (?, ?)", (nodeTo, neighborTableName))
            cursor.execute("CREATE TABLE IF NOT EXISTS _" + neighborTableName + " (neighbor INT NOT NULL, PRIMARY KEY(neighbor))")
            cursor.execute("INSERT OR IGNORE INTO _" + neighborTableName + " VALUES (?)", [nodeFrom])

    cursor.execute("SELECT node FROM VERTICES")
    vertices = cursor.fetchall()




# ------------------------------ enumerateSubgraph -----------------------------
# Enumerate size-k subgraphs of the original graph
# Preconditions: The graph should have already been built or exists
# Postcondition: The number of subgraphs found is displayed
def enumerateSubgraph(sizeK):
    global count, k, vertices

    k = sizeK
    count = 0

    for v in vertices:
        extendSubgraph([v[0]])

    print count

# ------------------------------- extendSubgraph -------------------------------
# Recursively looking size-k subgraphs of the graph.
# Precondition: The graph should have already been built or exists
# Postcondition: The number of subgraphs found is displayed
def extendSubgraph(Vsubgraph):
    global count
    #print "length", len(Vsubgraph)
    if len(Vsubgraph) == k :

        count += 1
        return

    query = "SELECT neighbor FROM ("
    if len(Vsubgraph) == 1:
        query = table_names[Vsubgraph[0]][0:-1] + str(Vsubgraph[0])
    else:
        for node in Vsubgraph:
            query += table_names[node][0:len(table_names[node]) - 18]
            query += " UNION "
        query = query[0:len(query) - 6] + ") WHERE neighbor > " + str(Vsubgraph[-1])


    print query
    cursor.execute(query)
    next_neighbors = cursor.fetchall()
    #print next_neighbors
    for next in next_neighbors:
        #print "next:" ,next[0]
        Vsubgraph.append(next[0])
        extendSubgraph(Vsubgraph)
        Vsubgraph.remove(next[0])




main()