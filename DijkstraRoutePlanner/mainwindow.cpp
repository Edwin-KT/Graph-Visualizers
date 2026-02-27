#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    resize(800, 600);
    mapWidget = new MapWidget(this);
    setCentralWidget(mapWidget);

    if (graph.loadFromXml("Harta_Luxemburg.xml")) {
        mapWidget->setGraph(&graph);
    }
}

MainWindow::~MainWindow() {}
