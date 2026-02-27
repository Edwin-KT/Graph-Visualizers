#include "MainWindow.h"
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    buttonLayout = new QHBoxLayout();

    btnLoad = new QPushButton("1. Incarca Date", this);
    btnFloyd = new QPushButton("2. Floyd-Warshall (Kn)", this);
    btnKruskal = new QPushButton("3. MST Kruskal", this);
    btnTSP = new QPushButton("4. TSP Preorder", this);

    buttonLayout->addWidget(btnLoad);
    buttonLayout->addWidget(btnFloyd);
    buttonLayout->addWidget(btnKruskal);
    buttonLayout->addWidget(btnTSP);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadData);
    connect(btnFloyd, &QPushButton::clicked, this, &MainWindow::onRunFloydWarshall);
    connect(btnKruskal, &QPushButton::clicked, this, &MainWindow::onRunKruskal);
    connect(btnTSP, &QPushButton::clicked, this, &MainWindow::onRunTSP);

    resize(800, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::onLoadData() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Data File", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        graph.loadFromFile(fileName.toStdString());
        update();
    }
}

void MainWindow::onRunFloydWarshall() {
    graph.runFloydWarshall();
    update();
}

void MainWindow::onRunKruskal() {
    graph.runKruskalMST();
    update();
}

void MainWindow::onRunTSP() {
    graph.runTSPPreorder();
    update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    auto cities = graph.getCities();
    auto edges = graph.getEdgesToDraw();
    int state = graph.getState();

    if (state == 0) painter.setPen(QPen(Qt::black, 1));
    else if (state == 1) painter.setPen(QPen(Qt::lightGray, 1));
    else if (state == 2) painter.setPen(QPen(Qt::blue, 2));
    else if (state == 3) painter.setPen(QPen(Qt::red, 3));

    for (const auto& edge : edges) {
        if (edge.source < cities.size() && edge.dest < cities.size()) {
            QPoint p1(cities[edge.source].x, cities[edge.source].y);
            QPoint p2(cities[edge.dest].x, cities[edge.dest].y);
            painter.drawLine(p1, p2);

            if (state == 2 || state == 0) {
                QPoint mid = (p1 + p2) / 2;
                painter.drawText(mid, QString::number((int)edge.weight));
            }
        }
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::yellow);
    for (const auto& city : cities) {
        painter.drawEllipse(QPoint(city.x, city.y), 6, 6);
        painter.drawText(city.x + 10, city.y, QString::fromStdString(city.name));
    }
}
