#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <QMap>
#include <vector>
#include <QXmlStreamReader>
#include <QFile>
#include <limits>
#include <queue>
#include <cmath>
#include <algorithm>

struct Node {
    long id;
    double lat;
    double lon;
    double x;
    double y;
};

struct Edge {
    long toNodeId;
    double length;
};

struct KdNode {
    Node node;
    KdNode* left;
    KdNode* right;

    KdNode(Node n) : node(n), left(nullptr), right(nullptr) {}
};

class Graph {
public:
    Graph();
    ~Graph();

    bool loadFromXml(const QString& filePath);
    void normalizeCoordinates(int width, int height);
    std::vector<long> dijkstra(long startId, long endId);

    long getNearestNode(double x, double y);

    const QMap<long, Node>& getNodes() const;
    const QMap<long, std::vector<Edge>>& getAdjList() const;

    double minLat, maxLat, minLon, maxLon;

private:
    QMap<long, Node> nodes;
    QMap<long, std::vector<Edge>> adjList;


    KdNode* root;

    void clearKdTree(KdNode* node);
    KdNode* buildKdTree(std::vector<Node>& nodes, int depth);
    void searchKdTree(KdNode* node, double targetX, double targetY, int depth, Node& bestNode, double& minDstSq);
};

#endif // GRAPH_H
