#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <limits>
#include <QPoint>
#include <stack>

struct City {
    std::string name;
    int x, y;
    int id;
};

struct Edge {
    int source;
    int dest;
    double weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class Graph {
public:
    Graph();
    void loadFromFile(const std::string& filename);
    void runFloydWarshall();
    void runKruskalMST();
    void runTSPPreorder();

    std::vector<City> getCities() const;
    std::vector<Edge> getEdgesToDraw() const;
    int getState() const;

    enum State {
        INITIAL_GRAPH = 0,
        COMPLETE_KN = 1,
        MST_RESULT = 2,
        TSP_CYCLE = 3
    };

private:
    int n;
    std::vector<City> cities;
    std::vector<std::vector<double>> adjMatrix;
    std::vector<Edge> currentEdges;
    std::vector<std::vector<int>> mstAdjList;
    std::vector<int> tspPath;
    int state;

    int findSet(std::vector<int>& parent, int i);
    void unionSets(std::vector<int>& parent, std::vector<int>& rank, int x, int y);
    void preorderTraversal(int u, std::vector<bool>& visited, std::vector<Edge>& pathEdges);
};

#endif
