#ifndef EDGE_H
#define EDGE_H

#include <QPointF>

struct Edge {
    int u;
    int v;
    int capacity;
    int flow;
    bool isReverse;
};

struct Node {
    int id;
    QPointF pos;
};

#endif // EDGE_H
