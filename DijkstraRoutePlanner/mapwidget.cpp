#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent) : QWidget(parent), graph(nullptr) {
    scaleFactor = 1.0;
    offsetX = 0;
    offsetY = 0;
    startNodeId = -1;
    endNodeId = -1;
    isDragging = false;
    setMouseTracking(true);
}

void MapWidget::setGraph(Graph* g) {
    graph = g;
    if(width() > 0 && height() > 0)
        graph->normalizeCoordinates(width(), height());
    update();
}

void MapWidget::resizeEvent(QResizeEvent *event) {
    if (graph) {
        graph->normalizeCoordinates(width(), height());
        update();
    }
    QWidget::resizeEvent(event);
}

void MapWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    if (!graph) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(offsetX, offsetY);
    painter.scale(scaleFactor, scaleFactor);

    QPen penEdge(Qt::lightGray, 1);
    penEdge.setCosmetic(true);
    painter.setPen(penEdge);

    const auto& nodes = graph->getNodes();
    const auto& adjList = graph->getAdjList();

    for (auto it = adjList.begin(); it != adjList.end(); ++it) {
        long u = it.key();
        if (!nodes.contains(u)) continue;
        QPointF p1(nodes[u].x, nodes[u].y);

        for (const auto& edge : it.value()) {
            long v = edge.toNodeId;
            if (nodes.contains(v)) {
                QPointF p2(nodes[v].x, nodes[v].y);
                painter.drawLine(p1, p2);
            }
        }
    }

    if (!path.empty()) {
        QPen penPath(Qt::red, 3);
        penPath.setCosmetic(true);
        painter.setPen(penPath);

        for (size_t i = 0; i < path.size() - 1; ++i) {
            long u = path[i];
            long v = path[i+1];
            if (nodes.contains(u) && nodes.contains(v)) {
                painter.drawLine(QPointF(nodes[u].x, nodes[u].y),
                                 QPointF(nodes[v].x, nodes[v].y));
            }
        }
    }

    double nodeRadius = 5.0 / scaleFactor;

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::blue);
    if (startNodeId != -1) {
        painter.drawEllipse(QPointF(nodes[startNodeId].x, nodes[startNodeId].y), nodeRadius, nodeRadius);
    }

    painter.setBrush(Qt::green);
    if (endNodeId != -1) {
        painter.drawEllipse(QPointF(nodes[endNodeId].x, nodes[endNodeId].y), nodeRadius, nodeRadius);
    }
}

void MapWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!graph) return;

        double clickX = (event->pos().x() - offsetX) / scaleFactor;
        double clickY = (event->pos().y() - offsetY) / scaleFactor;

        long clickedNode = graph->getNearestNode(clickX, clickY);

        if (startNodeId == -1) {
            startNodeId = clickedNode;
            path.clear();
            endNodeId = -1;
        } else if (endNodeId == -1) {
            endNodeId = clickedNode;
            path = graph->dijkstra(startNodeId, endNodeId);
        } else {
            startNodeId = clickedNode;
            endNodeId = -1;
            path.clear();
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        lastMousePos = event->pos();
        isDragging = true;
        setCursor(Qt::ClosedHandCursor);
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        double dx = event->pos().x() - lastMousePos.x();
        double dy = event->pos().y() - lastMousePos.y();

        offsetX += dx;
        offsetY += dy;

        lastMousePos = event->pos();
        update();
    }
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

void MapWidget::wheelEvent(QWheelEvent *event) {
    double scaleFactorDelta = 1.1;

    double mouseX = event->position().x();
    double mouseY = event->position().y();

    double oldScale = scaleFactor;

    if (event->angleDelta().y() > 0) {
        scaleFactor *= scaleFactorDelta;
    } else {
        scaleFactor /= scaleFactorDelta;
    }

    offsetX = mouseX - (mouseX - offsetX) * (scaleFactor / oldScale);
    offsetY = mouseY - (mouseY - offsetY) * (scaleFactor / oldScale);

    update();
}
