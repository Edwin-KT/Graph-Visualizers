#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "graph.h"

class MapWidget : public QWidget {
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    void setGraph(Graph* g);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Graph* graph;
    double scaleFactor;
    double offsetX;
    double offsetY;

    long startNodeId;
    long endNodeId;
    std::vector<long> path;

    QPoint lastMousePos;
    bool isDragging;
};

#endif // MAPWIDGET_H
