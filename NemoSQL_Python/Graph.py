import sqlite3
import datetime

connection = sqlite3.connect(':memory:')

k = 0
count = 0
table_names = {}
cursor = connection.cursor()
visited = set()
vertices = []

# ------------------------------------ main ------------------------------------
def main():
    buildGraph("./big_ecoli.txt")

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
    global count, k, visited, vertices

    k = sizeK
    count = 0
    visited = set()

    # test hash table vs sql query table
    # ************** hash table ***************
    # for v in table_names:
    #     Vextension = getExtension(v)
    #     visited.add(v)
    #     extendSubgraph([v], Vextension)
    #     visited.remove(v)

    # *********** sql query table **************
    for v in vertices:
        Vextension = getExtension(v[0])
        visited.add(v[0])
        extendSubgraph([v[0]], Vextension)
        visited.remove(v[0])

    print count


# -------------------------------- getExtension --------------------------------
def getExtension(w, v = None, Vextension = set()):
    global visited, table_names

    newExtension = set(Vextension)

    if v is None:
        v = w
    # Test construct string vs look up string
    # ***********   using hash_table   ****************
    cursor.execute(table_names[w], [v]) # marginally faster ~2.5% - 5%
    # ***********   constructing string ***************
    # cursor.execute("SELECT neighbor FROM _" + str(w) + " WHERE neighbor > ?", [v])

    allNeighbor = cursor.fetchall()

    for neighbor in allNeighbor:
        if neighbor[0] not in visited:
            newExtension.add(neighbor[0])

    return newExtension


# ------------------------------- extendSubgraph -------------------------------
# Recursively looking size-k subgraphs of the graph.
# Precondition: The graph should have already been built or exists
# Postcondition: The number of subgraphs found is displayed
def extendSubgraph(Vsubgraph, Vextension):
    global count, visited

    if len(Vsubgraph) == k-1 :
        for w in Vextension:
            count += 1
        return

    unvisited = []

    while len(Vextension) > 0:
        w = Vextension.pop()

        Vsubgraph.append(w)
        unvisited.append(w)
        visited.add(w)

        Vextension2 = getExtension(w, Vsubgraph[0], Vextension)

        extendSubgraph(Vsubgraph, Vextension2)
        Vsubgraph.pop()

    for i in unvisited:
        visited.remove(i)



main()