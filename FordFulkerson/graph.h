#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <algorithm>
#include "Edge.h"

class Graph {
public:
    Graph(int nodes);
    void addEdge(int u, int v, int cap);
    bool performStep(int s, int t);
    int getMaxFlow() const;
    std::vector<Edge> getEdges() const;
    std::vector<Edge> getResidualEdges() const;
    std::vector<int> getMinCutNodes(int s);
    void resetFlow();

private:
    int n;
    struct InternalEdge {
        int v;
        int capacity;
        int flow;
        int revIndex;
        int originalIndex;
    };
    std::vector<std::vector<InternalEdge>> adj;
    std::vector<Edge> originalEdges;
};

#endif // GRAPH_H
