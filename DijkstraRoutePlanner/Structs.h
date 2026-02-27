#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>

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

#endif
