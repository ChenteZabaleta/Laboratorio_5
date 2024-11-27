#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void crearLaberinto();
    void ActualizarImagen(QString direccion);
    bool tocarPared();
    void tocarEnemigo();
    void moverEnemigos();
    void explosion(QPointF &posicion);
    void colocarBomba();
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void resizeEvent(QResizeEvent *event) override{
        QMainWindow::resizeEvent(event);
        resizeGraphicsView();
    }
public slots:


private:
    Ui::MainWindow *ui;
    QGraphicsScene *escenaLaberinto;

    QList<QGraphicsPixmapItem*> BloquesSolidos;
    QList<QGraphicsPixmapItem*> BloquesDestruibles;


    QList<QGraphicsPixmapItem*> spritesBom;
    QGraphicsPixmapItem* imagenActual;


    QList<QGraphicsPixmapItem*> Enemigos; // Lista de items de imagen para los enemigos
    QList<QGraphicsPixmapItem*> imgExplosion;
    QTimer* timerEnemigos;
    QPixmap imagenEnemigo;

    QList<QGraphicsPixmapItem*> bombasActivas;
    int maxBombas = 3; // Número máximo de bombas simultáneas

    QGraphicsTextItem *LabelReloj, *LabelVidas, *LabelPuntaje;
    int reloj = 150, vidas = 10, puntaje = 0;

    QMediaPlayer* musicaFondo;
    QAudioOutput* volumen, *volumen1;
    QMediaPlayer* audioExplocion;
    QAudioOutput* volumenExplosion;


    void resizeGraphicsView() {
        int newWidth = width() * 0.92;
        int newHeight = height() * 0.94;

        // Calcular posición para centrar
        int posX = (width() - newWidth) / 2;
        int posY = (height() - newHeight) / 2;

        // Ajustar tamaño y posición del QGraphicsView
        ui->graphicsView->setGeometry(posX, posY+10, newWidth, newHeight);
    }

};

#endif // MAINWINDOW_H

