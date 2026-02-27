#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "graph.h"
#include "Edge.h"
#include "vector"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void nextStep();
    void showFinalResult();

private:
    void setupGraph();
    void drawGraph(bool showResidual, bool showMinCut);
    void drawArrow(QPainter *painter, QPointF start, QPointF end, QString text, QColor color, bool isDashed);

    Graph *graph;
    std::vector<Node> nodes;

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPushButton *btnNext;
    QPushButton *btnFinal;
    QLabel *lblInfo;

    bool isFinished;
    int sourceNode;
    int sinkNode;
};

#endif // MAINWINDOW_H
