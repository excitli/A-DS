#include <iostream>
#include "timsort.h"
#include <fstream>
#include <string>
#include <sstream>
#include <queue>

void dgs_recursive(int u, EdgeArray& edges, bool* visited, string* names) {
    visited[u] = true;
    cout << names[u] << " ";

    for (int i = 0; i < edges.size(); ++i) {
        int v = -1;
        if (edges[i].u == u) v = edges[i].v;
        else if (edges[i].v == u) v = edges[i].u;
        if (v != -1 && !visited[v]) {
            dgs_recursive(v, edges, visited, names);
        }
    }
}


void dgs(int startNode, int vertexCount, EdgeArray& edges, string* names) {

    bool* visited = new bool[vertexCount];
    for (int i = 0; i < vertexCount; ++i) visited[i] = false;

    cout << "DGS: ";
    dgs_recursive(startNode, edges, visited, names);
    cout << endl;

    delete[] visited;
}


void wgs(int startNode, int vertexCount, EdgeArray& edges, string* names) {

    bool* visited = new bool[vertexCount];
    for (int i = 0; i < vertexCount; ++i) visited[i] = false;


    queue<int> q;

    visited[startNode] = true;
    q.push(startNode);

    cout << "WGS: ";

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        cout << names[u] << " ";

        
        for (int i = 0; i < edges.size(); ++i) {
            int v = -1;
            if (edges[i].u == u) v = edges[i].v;
            else if (edges[i].v == u) v = edges[i].u;

            if (v != -1 && !visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    cout << endl;

    delete[] visited; 
}



class DSU {
private:
    IntArray parent;
public:
    DSU(int n) {
        for (int i = 0; i < n; ++i) {
            parent.push_back(i);
        }
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if ( root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};


int main() {
    ifstream file("matrix.txt");
    if (!file.is_open()) {
        cout << "No such file" << endl;
        return 1;
    }
    string line;
    string vertexNames[100];
    int vertexCount = 0;
    if (getline(file, line)) {
        stringstream ss(line);
        string name;
        while (ss >> name) {
            vertexNames[vertexCount++] = name;
        }
    }

    EdgeArray edges; 

    for (int i = 0; i < vertexCount; ++i) {
        if (!getline(file, line)) break;
        stringstream ss(line);
        //string skip;

        int weight;
        for (int j = 0; j < vertexCount; ++j) {
            ss >> weight;
            if (j > i && weight != 0) {
                edges.push_back(Edge(i, j, weight, vertexNames[i], vertexNames[j]));
            }
        }

    }
    file.close();

    if (vertexCount > 0) {
        wgs(0, vertexCount, edges, vertexNames);
        dgs(0, vertexCount, edges, vertexNames);
    }


    timsort(edges);

    DSU dsu(vertexCount);
    EdgeArray MST;
    int totalWeight = 0;

    for (int i = 0; i < edges.size(); ++i) {
        Edge nextEdge = edges[i];
        if (dsu.find(nextEdge.u) != dsu.find(nextEdge.v)) {
            dsu.unite(nextEdge.u, nextEdge.v);
            MST.push_back(nextEdge);
            totalWeight += nextEdge.weight;
        }
    }
    for (int i = 0; i < MST.size(); ++i) {
        cout << MST[i].u_name << " " << MST[i].v_name << endl;
    }
    cout << totalWeight << endl;
    return 0;
}