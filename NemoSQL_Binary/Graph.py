# ------------------------------------------------------------------------------
# Created by:   Kunlakan (Jeen) Cherdchusilp
#               Jonathan Earl
# Created on:   May 19, 2016
# Last updated: July 8, 2016
# ------------------------------------------------------------------------------

import datetime
import sqlite3
import subprocess
import sys

# GLOBAL VARIABLES
NEIGHBORS = dict()
VERTICES = dict()

# DATABASE INITIALIZATION
con = sqlite3.connect(':memory:')
cursor = con.cursor()


# ------------------------------------------------------------------------------
def main():
    buildGraph("input/Original_Input.txt")

    # start = datetime.datetime.now()
    enumerateSubgraph(3)
    getG6(3)
    # end = datetime.datetime.now()
    # print "Time : ", (end - start).total_seconds() * 1000
    #
    # start = datetime.datetime.now()
    # enumerateSubgraph(4)
    # getG6(4)
    # # end = datetime.datetime.now()
    # print "Time : ", (end - start).total_seconds() * 1000
    #
    # start = datetime.datetime.now()
    #enumerateSubgraph(5)
    #getG6(5)
    # end = datetime.datetime.now()
    # print "Time : ", (end - start).total_seconds() * 1000

# ------------------------------------------------------------------------------
# Builds a graph by reading data from a txt file
#
# Args:
#   file : a string representing path of the input file.
# ------------------------------------------------------------------------------
def buildGraph(file):
    global VERTICES, NEIGHBORS

    with open(file, "r") as inputFile:
        code = 1

        for line in inputFile:
            data = line.strip().split()
            nodeFrom = int(data[0])
            nodeTo = int(data[1])

            # Add all vertices to vertices dictionary
            if(not VERTICES.has_key(nodeFrom)):
                if code != 1: code = code << 1
                VERTICES[nodeFrom] = code

            if(not VERTICES.has_key(nodeTo)):
                code = code << 1
                VERTICES[nodeTo] = code

            # Add vertices and all of its link to the neighbor dictionary
            if(not NEIGHBORS.has_key(nodeFrom)):
                NEIGHBORS[nodeFrom] = set()
            if(not NEIGHBORS.has_key(nodeTo)):
                NEIGHBORS[nodeTo] = set()

            NEIGHBORS[nodeFrom].add(nodeTo)
            NEIGHBORS[nodeTo].add(nodeFrom)

        # Store vertices and their codes information into database
        con.execute('CREATE TABLE IF NOT EXISTS VERTICES (vertex INT NOT NULL PRIMARY KEY, code BLOB NOT NULL)')
        for v in VERTICES:
            con.execute('INSERT OR IGNORE INTO VERTICES VALUES (?, ?)', (v,  sqlite3.Binary(bin(VERTICES[v]))))

        # Store neighbor information into database
        con.execute('CREATE TABLE IF NOT EXISTS NEIGHBORS (vertex INT NOT NULL PRIMARY KEY, neighbor BLOB NOT NULL)')
        for n in NEIGHBORS:
            con.execute('INSERT OR IGNORE INTO NEIGHBORS VALUES (?, ?)', (n, str(NEIGHBORS[n])))


# ------------------------------------------------------------------------------
# Enumerate size-k subgraphs of the original graph
#
# Args:
#   k : size of subgraph to be found
# ------------------------------------------------------------------------------
def enumerateSubgraph(k):
    global NEIGHBORS, VERTICES

    for sizek in range(2, k + 1):
        created = 0

        for i in con.execute('SELECT COUNT(*) FROM sqlite_master WHERE type="table" AND name="size' + str(sizek) + '"'):
            created = i[0]

        # ----------------------------------------------------------------------
        # CASE 1: sizek = 2
        # ----------------------------------------------------------------------
        if sizek == 2 and created == 0:
            table = 'CREATE TABLE IF NOT EXISTS size' + str(sizek) + ' (code BLOB NOT NULL PRIMARY KEY, n1 INT NOT NULL, n2 INT NOT NULL)'
            con.execute(table)

            for v in VERTICES:
                for n in NEIGHBORS[v]:
                    if n > v:
                        code = VERTICES[v] | VERTICES[n]
                        con.execute('INSERT OR IGNORE INTO size2 VALUES (?, ?, ?)', (sqlite3.Binary(bin(code)), v, n))
        # ----------------------------------------------------------------------
        # ----------------------------------------------------------------------

        # ----------------------------------------------------------------------
        # CASE 2: sizek > 2
        # ----------------------------------------------------------------------
        elif sizek != 2 and created == 0:
            table = 'CREATE TABLE IF NOT EXISTS size' + str(sizek) + ' (code BLOB NO NULL PRIMARY KEY, '
            for i in range(1, sizek + 1):
                table += 'n' + str(i) + ' INT NOT NULL'
                if i != sizek:  table += ', '
                else:           table += ')'
            con.execute(table)

            for sm_subgraph in con.execute('SELECT * FROM size' + str(sizek - 1)):
                n = set()

                for i in range(1, len(sm_subgraph)):
                    n = n.union(NEIGHBORS[sm_subgraph[i]])

                for un in n:
                    if (un > sm_subgraph[1]) and (un not in sm_subgraph[1:]):
                        code = int(str(sm_subgraph[0]), 2) | VERTICES[un]

                        temp = sm_subgraph[1:]
                        temp = (sqlite3.Binary(bin(code)),) + temp + (un, )

                        subgraph = 'INSERT OR IGNORE INTO size' + str(sizek) + ' VALUES (?'
                        for i in range(sizek):
                            subgraph += ', ?'
                        subgraph += ')'

                        con.execute(subgraph, temp)
        # ----------------------------------------------------------------------
        # ----------------------------------------------------------------------

    for i in con.execute('SELECT COUNT(*) FROM size' + str(k)):
        print i[0]

# ------------------------------------------------------------------------------
# Generate canonically labels for the given size-k subgraphs. This function must
# only been called once the size-k subgraph is generated.
#
# Args:
#   k : a size of subgraph
# ------------------------------------------------------------------------------
def getG6(k):
    global NEIGHBORS

    AllG6 = dict()
    for i in con.execute('SELECT * FROM size' + str(k)):

        # Get a bit vector
        matrix = [[0 for x in range(len(i)-1)] for y in range(len(i)-1)]

        for n in range(k):
            for m in range(k):
                if (i[n + 1] != i[m + 1] ):
                    if (i[m + 1] in NEIGHBORS[i[n+1]]):
                        matrix[n][m] = 1

        g6code = ""
        for row in range(1, len(matrix)):
            for col in range(row):
                g6code += str(matrix[col][row])

        # Pad on the right with 0 to make the length a multiple of 6
        while(len(g6code) % 6 != 0):
            g6code += "0"

        # Split into groups of 6 bits each
        new_g6code = chr(k+63)
        for j in range(0, len(g6code), 6):
            temp = int(g6code[j: j+6], 2) + 63
            new_g6code += chr(temp)

            if new_g6code not in AllG6:
                AllG6[new_g6code] = 1
            else:
                AllG6[new_g6code] = AllG6[new_g6code] + 1

    all = ""
    for i in AllG6:
        all = all + i + '\n'

    p = subprocess.Popen('nauty_WIN\labelg.exe', shell=True, stdin=subprocess.PIPE)
    p.stdin.write(all)
    (out, err) = p.communicate()
# ------------------------------------------------------------------------------

main()