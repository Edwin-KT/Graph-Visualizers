#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include "Graph.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onLoadData();
    void onRunFloydWarshall();
    void onRunKruskal();
    void onRunTSP();

private:
    Graph graph;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QPushButton *btnLoad;
    QPushButton *btnFloyd;
    QPushButton *btnKruskal;
    QPushButton *btnTSP;
};

#endif
