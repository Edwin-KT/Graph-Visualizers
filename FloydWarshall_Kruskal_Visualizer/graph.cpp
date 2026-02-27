#include "Graph.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>

Graph::Graph() : n(0), state(INITIAL_GRAPH) {}

void Graph::loadFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) return;

    cities.clear();
    std::string tempName;
    int tx, ty;
    int idCounter = 0;

    while (fin >> tempName && tempName != "EndCities") {
        fin >> tx >> ty;
        cities.push_back({tempName, tx, ty, idCounter++});
    }

    n = cities.size();
    adjMatrix.assign(n, std::vector<double>(n, 1e9));
    for (int i = 0; i < n; ++i) adjMatrix[i][i] = 0;

    int u, v;
    double w;
    currentEdges.clear();
    while (fin >> u >> v >> w) {
        if (u < n && v < n) {
            adjMatrix[u][v] = w;
            adjMatrix[v][u] = w;
            currentEdges.push_back({u, v, w});
        }
    }
    state = INITIAL_GRAPH;
}

void Graph::runFloydWarshall() {
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjMatrix[i][k] != 1e9 && adjMatrix[k][j] != 1e9) {
                    if (adjMatrix[i][j] > adjMatrix[i][k] + adjMatrix[k][j]) {
                        adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
                    }
                }
            }
        }
    }

    currentEdges.clear();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            currentEdges.push_back({i, j, adjMatrix[i][j]});
        }
    }
    state = COMPLETE_KN;
}

int Graph::findSet(std::vector<int>& parent, int i) {
    if (parent[i] != i)
        parent[i] = findSet(parent, parent[i]);
    return parent[i];
}

void Graph::unionSets(std::vector<int>& parent, std::vector<int>& rank, int x, int y) {
    int xroot = findSet(parent, x);
    int yroot = findSet(parent, y);

    if (xroot != yroot) {
        if (rank[xroot] < rank[yroot])
            parent[xroot] = yroot;
        else if (rank[xroot] > rank[yroot])
            parent[yroot] = xroot;
        else {
            parent[yroot] = xroot;
            rank[xroot]++;
        }
    }
}

void Graph::runKruskalMST() {
    std::vector<Edge> allEdges;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            allEdges.push_back({i, j, adjMatrix[i][j]});
        }
    }
    std::sort(allEdges.begin(), allEdges.end());

    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;

    currentEdges.clear();
    mstAdjList.assign(n, std::vector<int>());

    int edgesCount = 0;
    for (const auto& edge : allEdges) {
        int u = edge.source;
        int v = edge.dest;
        if (findSet(parent, u) != findSet(parent, v)) {
            currentEdges.push_back(edge);
            unionSets(parent, rank, u, v);
            mstAdjList[u].push_back(v);
            mstAdjList[v].push_back(u);
            edgesCount++;
        }
    }
    state = MST_RESULT;
}

void Graph::runTSPPreorder() {
    if (mstAdjList.empty() || n == 0) return;

    tspPath.clear();
    std::vector<bool> visited(n, false);
    std::stack<int> s;

    s.push(0);

    while (!s.empty()) {
        int u = s.top();
        s.pop();

        if (!visited[u]) {
            visited[u] = true;
            tspPath.push_back(u);

            for (int v : mstAdjList[u]) {
                if (!visited[v]) {
                    s.push(v);
                }
            }
        }
    }

    if (!tspPath.empty()) {
        tspPath.push_back(tspPath[0]);
    }

    currentEdges.clear();
    for (size_t i = 0; i < tspPath.size() - 1; ++i) {
        int u = tspPath[i];
        int v = tspPath[i+1];
        currentEdges.push_back({u, v, adjMatrix[u][v]});
    }
    state = TSP_CYCLE;
}

std::vector<City> Graph::getCities() const {
    return cities;
}

std::vector<Edge> Graph::getEdgesToDraw() const {
    return currentEdges;
}

int Graph::getState() const {
    return state;
}
