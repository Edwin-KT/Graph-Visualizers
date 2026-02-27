#include "graph.h"

Graph::Graph() : root(nullptr) {
    minLat = std::numeric_limits<double>::max();
    maxLat = std::numeric_limits<double>::lowest();
    minLon = std::numeric_limits<double>::max();
    maxLon = std::numeric_limits<double>::lowest();
}

Graph::~Graph() {
    clearKdTree(root);
}

void Graph::clearKdTree(KdNode* node) {
    if (node == nullptr) return;
    clearKdTree(node->left);
    clearKdTree(node->right);
    delete node;
}

bool Graph::loadFromXml(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == QString("node")) {
                long id = xml.attributes().value("id").toLong();
                double lat = xml.attributes().value("latitude").toDouble();
                double lon = xml.attributes().value("longitude").toDouble();

                Node n;
                n.id = id;
                n.lat = lat;
                n.lon = lon;
                nodes.insert(id, n);

                if (lat < minLat) minLat = lat;
                if (lat > maxLat) maxLat = lat;
                if (lon < minLon) minLon = lon;
                if (lon > maxLon) maxLon = lon;
            }
            else if (xml.name() == QString("arc")) {
                long from = xml.attributes().value("from").toLong();
                long to = xml.attributes().value("to").toLong();
                double length = xml.attributes().value("length").toDouble();

                Edge e;
                e.toNodeId = to;
                e.length = length;
                adjList[from].push_back(e);
            }
        }
    }
    return !xml.hasError();
}

KdNode* Graph::buildKdTree(std::vector<Node>& nodeList, int depth) {
    if (nodeList.empty()) return nullptr;

    int axis = depth % 2;

    auto comparator = [axis](const Node& a, const Node& b) {
        if (axis == 0) return a.x < b.x;
        return a.y < b.y;
    };

    size_t medianIndex = nodeList.size() / 2;
    std::nth_element(nodeList.begin(), nodeList.begin() + medianIndex, nodeList.end(), comparator);

    KdNode* node = new KdNode(nodeList[medianIndex]);

    std::vector<Node> leftNodes(nodeList.begin(), nodeList.begin() + medianIndex);
    std::vector<Node> rightNodes(nodeList.begin() + medianIndex + 1, nodeList.end());

    node->left = buildKdTree(leftNodes, depth + 1);
    node->right = buildKdTree(rightNodes, depth + 1);

    return node;
}

void Graph::normalizeCoordinates(int width, int height) {
    double latRange = maxLat - minLat;
    double lonRange = maxLon - minLon;

    std::vector<Node> nodeList;

    for (auto& node : nodes) {
        node.x = ((node.lon - minLon) / lonRange) * width;
        node.y = height - ((node.lat - minLat) / latRange) * height;
        nodeList.push_back(node);
    }

    clearKdTree(root);
    root = buildKdTree(nodeList, 0);
}

void Graph::searchKdTree(KdNode* node, double targetX, double targetY, int depth, Node& bestNode, double& minDistSq) {
    if (node == nullptr) return;

    double dx = node->node.x - targetX;
    double dy = node->node.y - targetY;
    double distSq = dx * dx + dy * dy;

    if (distSq < minDistSq) {
        minDistSq = distSq;
        bestNode = node->node;
    }

    int axis = depth % 2;
    double diff = (axis == 0) ? (targetX - node->node.x) : (targetY - node->node.y);

    KdNode* nearSide = (diff < 0) ? node->left : node->right;
    KdNode* farSide = (diff < 0) ? node->right : node->left;

    searchKdTree(nearSide, targetX, targetY, depth + 1, bestNode, minDistSq);

    if (diff * diff < minDistSq) {
        searchKdTree(farSide, targetX, targetY, depth + 1, bestNode, minDistSq);
    }
}

long Graph::getNearestNode(double x, double y) {
    if (root == nullptr) return -1;

    Node bestNode;
    double minDistSq = std::numeric_limits<double>::max();

    searchKdTree(root, x, y, 0, bestNode, minDistSq);

    return bestNode.id;
}

std::vector<long> Graph::dijkstra(long startId, long endId) {
    std::priority_queue<std::pair<double, long>,
                        std::vector<std::pair<double, long>>,
                        std::greater<std::pair<double, long>>> pq;

    QMap<long, double> dist;
    QMap<long, long> parent;

    for (auto it = nodes.keyBegin(); it != nodes.keyEnd(); ++it) {
        dist[*it] = std::numeric_limits<double>::max();
    }

    dist[startId] = 0;
    pq.push({0, startId});

    while (!pq.empty()) {
        long u = pq.top().second;
        double d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == endId) break;

        if (adjList.contains(u)) {
            for (const auto& edge : adjList[u]) {
                long v = edge.toNodeId;
                double weight = edge.length;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    std::vector<long> path;
    if (dist[endId] == std::numeric_limits<double>::max()) return path;

    long curr = endId;
    while (curr != startId) {
        path.push_back(curr);
        curr = parent[curr];
    }
    path.push_back(startId);
    return path;
}

const QMap<long, Node>& Graph::getNodes() const { return nodes; }
const QMap<long, std::vector<Edge>>& Graph::getAdjList() const { return adjList; }
