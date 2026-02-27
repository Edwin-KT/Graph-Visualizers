#include "graph.h"

Graph::Graph(int nodes) : n(nodes) {
    adj.resize(n);
}

void Graph::addEdge(int u, int v, int cap) {
    Edge e = {u, v, cap, 0, false};
    originalEdges.push_back(e);

    InternalEdge a = {v, cap, 0, (int)adj[v].size(), (int)originalEdges.size() - 1};
    InternalEdge b = {u, 0, 0, (int)adj[u].size(), -1};

    adj[u].push_back(a);
    adj[v].push_back(b);
}

bool Graph::performStep(int s, int t) {
    std::vector<int> parent(n, -1);
    std::vector<int> edgeIndex(n, -1);
    std::queue<std::pair<int, int>> q;

    q.push({s, 1000000000});
    parent[s] = s;

    int pathFlow = 0;
    int cur = -1;

    while (!q.empty()) {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        if (u == t) {
            pathFlow = flow;
            cur = t;
            break;
        }

        for (int i = 0; i < adj[u].size(); ++i) {
            InternalEdge &e = adj[u][i];
            if (parent[e.v] == -1 && e.capacity > e.flow) {
                parent[e.v] = u;
                edgeIndex[e.v] = i;
                int newFlow = std::min(flow, e.capacity - e.flow);
                q.push({e.v, newFlow});
            }
        }
    }

    if (cur == -1) return false;

    while (cur != s) {
        int prev = parent[cur];
        int idx = edgeIndex[cur];

        adj[prev][idx].flow += pathFlow;
        int revIdx = adj[prev][idx].revIndex;
        adj[cur][revIdx].flow -= pathFlow;

        int originalIdx = adj[prev][idx].originalIndex;
        if (originalIdx != -1) {
            originalEdges[originalIdx].flow += pathFlow;
        } else {
            int revOriginal = adj[cur][revIdx].originalIndex;
            originalEdges[revOriginal].flow -= pathFlow;
        }

        cur = prev;
    }

    return true;
}

int Graph::getMaxFlow() const {
    int maxFlow = 0;
    for (const auto &e : originalEdges) {
        if (e.u == 0) maxFlow += e.flow;
    }
    return maxFlow;
}

std::vector<Edge> Graph::getEdges() const {
    return originalEdges;
}

std::vector<Edge> Graph::getResidualEdges() const {
    std::vector<Edge> res;
    for (int u = 0; u < n; ++u) {
        for (const auto &e : adj[u]) {
            int residual = e.capacity - e.flow;
            if (residual > 0) {
                res.push_back({u, e.v, residual, 0, false});
            }
        }
    }
    return res;
}

std::vector<int> Graph::getMinCutNodes(int s) {
    std::vector<int> visited(n, 0);
    std::queue<int> q;
    q.push(s);
    visited[s] = 1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (const auto &e : adj[u]) {
            if (!visited[e.v] && e.capacity > e.flow) {
                visited[e.v] = 1;
                q.push(e.v);
            }
        }
    }
    return visited;
}

void Graph::resetFlow() {
    for (auto &edgeList : adj) {
        for (auto &e : edgeList) {
            e.flow = 0;
        }
    }
    for (auto &e : originalEdges) {
        e.flow = 0;
    }
}
