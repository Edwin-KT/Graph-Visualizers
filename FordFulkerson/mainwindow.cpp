#include "MainWindow.h"
#include <QPainter>
#include <QGraphicsItem>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isFinished(false), sourceNode(0), sinkNode(5) {

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    layout = new QVBoxLayout(centralWidget);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    btnNext = new QPushButton("Urmatoarea Iteratie (Retea Reziduala)", this);
    btnFinal = new QPushButton("Afiseaza Taietura Minima", this);
    lblInfo = new QLabel("Flux Maxim: 0", this);

    layout->addWidget(lblInfo);
    layout->addWidget(view);
    layout->addWidget(btnNext);
    layout->addWidget(btnFinal);

    connect(btnNext, &QPushButton::clicked, this, &MainWindow::nextStep);
    connect(btnFinal, &QPushButton::clicked, this, &MainWindow::showFinalResult);

    graph = new Graph(6);
    setupGraph();
    drawGraph(true, false);
}

MainWindow::~MainWindow() {
    delete graph;
}

void MainWindow::setupGraph() {
    nodes = {
        {0, QPointF(50, 200)},
        {1, QPointF(200, 100)},
        {2, QPointF(200, 300)},
        {3, QPointF(400, 100)},
        {4, QPointF(400, 300)},
        {5, QPointF(550, 200)}
    };

    graph->addEdge(0, 1, 10);
    graph->addEdge(0, 2, 10);
    graph->addEdge(1, 2, 2);
    graph->addEdge(1, 3, 4);
    graph->addEdge(1, 4, 8);
    graph->addEdge(2, 4, 9);
    graph->addEdge(3, 5, 10);
    graph->addEdge(4, 3, 6);
    graph->addEdge(4, 5, 10);
}

void MainWindow::nextStep() {
    if (isFinished) return;

    bool improved = graph->performStep(sourceNode, sinkNode);
    int currentFlow = graph->getMaxFlow();
    lblInfo->setText("Flux Maxim Curent: " + QString::number(currentFlow));

    if (!improved) {
        isFinished = true;
        lblInfo->setText("Algoritm finalizat. Flux Maxim: " + QString::number(currentFlow));
        btnNext->setEnabled(false);
    }

    drawGraph(true, false);
}

void MainWindow::showFinalResult() {
    while(!isFinished) {
        bool improved = graph->performStep(sourceNode, sinkNode);
        if (!improved) isFinished = true;
    }
    int currentFlow = graph->getMaxFlow();
    lblInfo->setText("Final. Flux Maxim: " + QString::number(currentFlow));
    btnNext->setEnabled(false);
    drawGraph(false, true);
}

void MainWindow::drawGraph(bool showResidual, bool showMinCut) {
    scene->clear();

    std::vector<int> minCutNodes;
    if (showMinCut) {
        minCutNodes = graph->getMinCutNodes(sourceNode);
    }

    if (showResidual && !showMinCut) {
        std::vector<Edge> resEdges = graph->getResidualEdges();
        for (const auto &e : resEdges) {
            drawArrow(nullptr, nodes[e.u].pos, nodes[e.v].pos, QString::number(e.capacity), Qt::darkGreen, true);
        }
    } else {
        std::vector<Edge> edges = graph->getEdges();
        for (const auto &e : edges) {
            QColor color = Qt::black;
            if (showMinCut) {
                if (minCutNodes[e.u] && !minCutNodes[e.v]) {
                    color = Qt::red;
                }
            }
            QString label = QString::number(e.flow) + "/" + QString::number(e.capacity);
            drawArrow(nullptr, nodes[e.u].pos, nodes[e.v].pos, label, color, false);
        }
    }

    for (const auto &n : nodes) {
        QColor brushColor = Qt::white;
        if (showMinCut && minCutNodes[n.id]) brushColor = Qt::yellow;

        scene->addEllipse(n.pos.x() - 15, n.pos.y() - 15, 30, 30, QPen(Qt::black), QBrush(brushColor));
        QGraphicsTextItem *text = scene->addText(QString::number(n.id));
        text->setPos(n.pos.x() - 10, n.pos.y() - 10);
    }
}

void MainWindow::drawArrow(QPainter *, QPointF start, QPointF end, QString text, QColor color, bool isDashed) {
    QLineF line(start, end);
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF p1 = line.p1() + QPointF(sin(angle + M_PI / 2) * 3, cos(angle + M_PI / 2) * 3);
    QPointF p2 = line.p2() + QPointF(sin(angle + M_PI / 2) * 3, cos(angle + M_PI / 2) * 3);

    double arrowSize = 10;
    QPointF arrowP1 = p2 - QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = p2 - QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead << p2 << arrowP1 << arrowP2;

    QPen pen(color);
    pen.setWidth(2);
    if (isDashed) pen.setStyle(Qt::DashLine);

    scene->addLine(QLineF(p1, p2), pen);
    scene->addPolygon(arrowHead, QPen(color), QBrush(color));

    QPointF mid = (p1 + p2) / 2;

    double textOffset = 20;
    double textX = mid.x() + std::cos(angle + M_PI / 2) * textOffset;
    double textY = mid.y() - std::sin(angle + M_PI / 2) * textOffset;

    QGraphicsTextItem *lbl = scene->addText(text);
    lbl->setDefaultTextColor(color);

    lbl->setPos(textX - lbl->boundingRect().width() / 2,
                textY - lbl->boundingRect().height() / 2);
}
