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

 musicaFondo = new QMediaPlayer(this);
    musicaFondo->setSource(QUrl("qrc:/imagenes/bomberman.mp3"));
    volumen = new QAudioOutput(this);
    volumen->setVolume(1.0);
    musicaFondo->setAudioOutput(volumen);
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    musicaFondo->play();

    audioExplocion = new QMediaPlayer(this);
    volumen1 = new QAudioOutput(this);
    volumen1->setVolume(1.0);

}




int ITEM = 1;
// MÉTODO QUE DETECTA CUANDO SE PRECIONAN TECLAS //
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    int cantidad = 10;
    QPointF previaPos = imagenActual->pos();
    bool TeclaValida = true;

    switch (e->key()) {
    case Qt::Key_H:  // Mover a la derecha
        if (ITEM < 3 || ITEM > 5) ITEM = 3;
        imagenActual->setPos(previaPos.x() + cantidad, previaPos.y());
        if (tocarPared()) {
            imagenActual->setPos(previaPos.x() - cantidad, previaPos.y());
        }
        break;

    case Qt::Key_F:  // Mover a la izquierda
        if (ITEM > 2) ITEM = 0;
        imagenActual->setPos(previaPos.x() - cantidad, previaPos.y());
        if (tocarPared()) {
            imagenActual->setPos(previaPos.x() + cantidad, previaPos.y());
        }
        break;

    case Qt::Key_T:  // Mover hacia arriba
        if (ITEM < 6 || ITEM > 8) ITEM = 6;
        imagenActual->setPos(previaPos.x(), previaPos.y() - cantidad);
        if (tocarPared()) {
            imagenActual->setPos(previaPos.x(), previaPos.y() + cantidad);
        }
        break;

    case Qt::Key_G:  // Mover hacia abajo
        if (ITEM < 9 || ITEM > 11) ITEM = 9;
        imagenActual->setPos(previaPos.x(), previaPos.y() + cantidad);
        if (tocarPared()) {
            imagenActual->setPos(previaPos.x(), previaPos.y() - cantidad);
        }
        break;

    case Qt::Key_K:{
        colocarBomba();
        TeclaValida = false;
        break;
    }

    default:
        TeclaValida = false;
        break;
    }

    // actualizar la imagen si la tecla es válida
    if (TeclaValida) {
        spritesBom[ITEM]->setPos(imagenActual->pos());
        escenaLaberinto->removeItem(imagenActual);
        escenaLaberinto->addItem(spritesBom[ITEM]);
        imagenActual = spritesBom[ITEM];
        ++ITEM;
    }
    else imagenActual->setZValue(1); //Para mantener la imagen al frente
}

