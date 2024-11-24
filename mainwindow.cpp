#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(0, 128, 0)));
    ui->graphicsView->setGeometry(0, 0,width()*0.5,height()*0.5);

    escenaLaberinto = new QGraphicsScene();
    crearLaberinto();
    ui->graphicsView->setScene(escenaLaberinto);
