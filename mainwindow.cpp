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




void MainWindow::crearLaberinto()
{
    int configLab[11][21] = {
        {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };



    // Crear Enemigos
    for (int i = 0; i < 3; ++i) {
        QGraphicsPixmapItem* Enemigo = new QGraphicsPixmapItem();

        QPixmap pixmap(":/imagenes/enemig.png");
        if (i == 0){
            Enemigo->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //Enemigo->setPos(90, 344);
        }
        if (i == 1){
            Enemigo->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //Enemigo->setPos(950, 210);
        }
        if (i == 2){
            Enemigo->setPixmap(pixmap.scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //Enemigo->setPos(745, 267);
        }

        escenaLaberinto->addItem(Enemigo);
        Enemigos.append(Enemigo);
    }



    // RELLENAR LABERINTO //

    int fila1 = QRandomGenerator::global()->bounded(3, 10);
    if (fila1%2 == 0) fila1 += 1;
    bool pintar1 = true, pintar2 = true, pintar3 = true;
    int fila2 = QRandomGenerator::global()->bounded(3, 10);
    if (fila2%2 == 0) fila2 += 1;
    int columna = QRandomGenerator::global()->bounded(1, 20);
    if (columna%2 == 0) columna += 1;

    for (int i = 0; i < 11; ++i){
        for (int j = 0; j < 21; ++j){

            if(i == fila1 && j > 0 && j < 7){if(pintar1) Enemigos.at(0)->setPos(j * 67, i * 67); pintar1 = false;} //Crear camino enemigo 1
            else if(i == fila2 && j > 13 && j < 20){if(pintar2) Enemigos.at(1)->setPos(j * 67, i * 67); pintar2 = false;} //Crear camino enemigo 3
            else if(j == columna && i > 2 && i < 7) {if(pintar3) Enemigos.at(2)->setPos(j * 67 +9, i * 67); pintar3 = false;} //Crear camino enemigo 2

            else{
                if (configLab[i][j] == 1) {
                    BloquesSolidos.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/solido.png").scaled(67, 67, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
                    BloquesSolidos.back()->setPos(j*67,i*67);
                    escenaLaberinto->addItem(BloquesSolidos.back());
                }
                else if(configLab[i][j] == 0){
                    if ((QRandomGenerator::global()->bounded(1, 101))%2 == 0){ // Genera un número entre 1 y 100 y le saco %2
                        BloquesDestruibles.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/destruible.png").scaled(67, 67, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
                        BloquesDestruibles.back()->setPos(j*67,i*67);
                        escenaLaberinto->addItem(BloquesDestruibles.back());
                    }
                }
                else if(configLab[i][j] == 5){
                    escenaLaberinto->addRect(j * 67, i * 67, 67, 77, QPen(Qt::NoPen), QBrush(QColor(101, 67, 33)));
                }
            }
        }
    }



    // Configuramos ul temporizador para mover los enemigos
    timerEnemigos = new QTimer(this);
    connect(timerEnemigos, &QTimer::timeout, this, &MainWindow::moverEnemigos);
    timerEnemigos->start(25);



    // Cargar los sprites del personaje principal
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/i0.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/i1.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/i2.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/d0.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/d1.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/d2.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/a0.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/a1.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/a2.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));


    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/f0.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/f1.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    spritesBom.append(new QGraphicsPixmapItem(QPixmap(":/imagenes/f2.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    spritesBom[3]->setPos(80,145);
    escenaLaberinto->addItem(spritesBom[3]);
    imagenActual = spritesBom[3];


    // CARGAR SPRITES DE EXPLOSION //
    imgExplosion.append(new QGraphicsPixmapItem ((QPixmap(":/imagenes/exp1.png").scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation))));
    imgExplosion.append(new QGraphicsPixmapItem ((QPixmap(":/imagenes/exp2.png").scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation))));
    //imgExplosion.append(new QGraphicsPixmapItem ((QPixmap(":/imagenes/exp3.png").scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation))));
    imgExplosion.append(new QGraphicsPixmapItem ((QPixmap(":/imagenes/exp4.png").scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation))));

    for (int i = 0; i < 3; ++i) {
        imgExplosion[i]->setVisible(false);  // Establecer los ítems como invisibles inicialmente
        escenaLaberinto->addItem(imgExplosion[i]);  // Agregar los ítems a la escena
    }

    // LABELS PARA MOSTRAR ESTADISTICAS DE LAS PARTIDAS //
    LabelReloj = escenaLaberinto->addText("TIEMPO RESTANTE 150", QFont("Arial", 20, QFont::Bold));
    LabelReloj->setDefaultTextColor(Qt::white);
    LabelReloj->setPos(80, 18);
    LabelVidas = escenaLaberinto->addText("VIDAS 10", QFont("Arial", 20, QFont::Bold));
    LabelVidas->setDefaultTextColor(Qt::white);
    LabelVidas->setPos(600, 18);
    LabelPuntaje = escenaLaberinto->addText("PUNTOS 0", QFont("Arial", 20, QFont::Bold));
    LabelPuntaje->setDefaultTextColor(Qt::white);
    LabelPuntaje->setPos(1000, 18);

}






bool MainWindow::tocarPared()
{
    bool tocoLaPared = false;
    for(auto iter = BloquesSolidos.begin(); iter < BloquesSolidos.end(); ++iter){
        if(imagenActual->collidesWithItem(*iter)){
            tocoLaPared = true;
        }
    }
    for(auto iter = BloquesDestruibles.begin(); iter < BloquesDestruibles.end(); ++iter){
        if(imagenActual->collidesWithItem(*iter)){
            tocoLaPared = true;
        }
    }

    return tocoLaPared;
}

void MainWindow::tocarEnemigo()
{
    for(auto iter = Enemigos.begin(); iter < Enemigos.end(); ++iter){
        if (escenaLaberinto->items().contains(*iter)){
            if(imagenActual->collidesWithItem(*iter)){
                --vidas;
                LabelVidas->setPlainText("VIDAS " + QString::number(vidas));
                if(vidas == 0){
                    timerEnemigos->stop();
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("PERDISTE");
                    msgBox.setText("¿Quieres empezar de nuevo?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int reply = msgBox.exec();
                    if (reply == QMessageBox::Yes) {
                        QApplication::quit();
                        QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
                    } else {
                        QApplication::quit();
                    }
                }
            }
        }
    }
}




void MainWindow::moverEnemigos()
{
    static int contador = 0, contador2 = 0;
    tocarEnemigo(); //Verificamos si se estan tocando el principal con los enemigos


    static bool cambiarDireccionDelMovimiento;
    if (contador <= 0) cambiarDireccionDelMovimiento = true;
    else if (contador >= 300) cambiarDireccionDelMovimiento = false;
    if (cambiarDireccionDelMovimiento){
        contador += 5;
        if (Enemigos.at(0)->scene() == escenaLaberinto) Enemigos.at(0)->setPos(Enemigos.at(0)->x()+5, Enemigos.at(0)->y());
        if (Enemigos.at(1)->scene() == escenaLaberinto) Enemigos.at(1)->setPos(Enemigos.at(1)->x()+5, Enemigos.at(1)->y());
        if (Enemigos.at(2)->scene() == escenaLaberinto) Enemigos.at(2)->setPos(Enemigos.at(2)->x(), Enemigos.at(2)->y()+2.6);
    }
    else{
        contador -= 5;
        if (Enemigos.at(0)->scene() == escenaLaberinto) Enemigos.at(0)->setPos(Enemigos.at(0)->x()-5, Enemigos.at(0)->y());
        if (Enemigos.at(1)->scene() == escenaLaberinto) Enemigos.at(1)->setPos(Enemigos.at(1)->x()-5, Enemigos.at(1)->y());
        if (Enemigos.at(2)->scene() == escenaLaberinto) Enemigos.at(2)->setPos(Enemigos.at(2)->x(), Enemigos.at(2)->y()-2.6);
    }


    // MEDIMOS EL TIEMPO DE JUEGO //
    ++contador2;
    if(contador2 == 40){
        if (reloj >= 0){
            LabelReloj->setPlainText("TIEMPO RESTANTE " + QString::number(reloj));
        }
        else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "PERDISTE", "¿Quieres empezar de nuevo?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QApplication::quit(); // Cierra la aplicación
                QProcess::startDetached(QApplication::applicationFilePath(), QStringList()); // La reinicia
            } else {
                QApplication::quit(); // Cierra todo el programa
            }
        }
        --reloj;
        contador2 = 0;
    }
}





void MainWindow::colocarBomba() {
    if (bombasActivas.size() >= maxBombas) { // Verificamos si se alcanzó el límite de bombas activas
        return;
    }

    // Creamos la bomba
    QGraphicsPixmapItem* bomba = new QGraphicsPixmapItem(QPixmap(":/imagenes/bomba.png").scaled(50, 50));
    bomba->setPos(imagenActual->pos());
    escenaLaberinto->addItem(bomba);
    bombasActivas.append(bomba);

    // Configuramos un temporizador para hacer que la bomba explote después de 2 segundos
    QTimer::singleShot(2000, this, [=]() {
        // Creamos un área de explosión alrededor de la bomba
        QRectF areaExplosion(bomba->x() - 50, bomba->y() - 50, 150, 150);
        QList<QGraphicsItem*> itemsAfectados = escenaLaberinto->items(areaExplosion);


        // Recorremos los elementos afectados para aplicar la lógica de explosión
        for (QGraphicsItem* item : itemsAfectados) {
            if (BloquesDestruibles.contains(static_cast<QGraphicsPixmapItem*>(item))) {
                escenaLaberinto->removeItem(item);
                BloquesDestruibles.removeOne(static_cast<QGraphicsPixmapItem*>(item));
                delete item;
                puntaje += 5+reloj;
                LabelPuntaje->setPlainText("PUNTOS " + QString::number(puntaje));
            }
            else if (Enemigos[0] == item || Enemigos[1] == item || Enemigos[2] == item) {
                escenaLaberinto->removeItem(item);
                puntaje += 10+reloj;
                LabelPuntaje->setPlainText("PUNTOS " + QString::number(puntaje));
            }
            else if (item == imagenActual) { // SI LA BOMBA AFECTA AL PERSONAJE PRINCIPAL //
                --vidas;
                LabelVidas->setPlainText("VIDAS " + QString::number(vidas));
                if(vidas == 0){
                    timerEnemigos->stop();
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("PERDISTE");
                    msgBox.setText("¿Quieres empezar de nuevo?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int reply = msgBox.exec();
                    if (reply == QMessageBox::Yes) {
                        QApplication::quit();
                        QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
                    } else {
                        QApplication::quit();
                    }
                }
            }
        }


        // Removemos la bomba después de la explosión
        QPointF posicion = bomba->pos();
        escenaLaberinto->removeItem(bomba);
        bombasActivas.removeOne(bomba);
        delete bomba;
        explosion(posicion);
    });
}





void MainWindow::explosion(QPointF &posicion)
{
    audioExplocion->setSource(QUrl("qrc:/imagenes/explosion.wav"));
    audioExplocion->setAudioOutput(volumen1);
    audioExplocion->play();

    // Temporizador para la animación de la explosion
    for (int i = 0; i < 3; ++i) {
        imgExplosion[i]->setPos(posicion.x() - 50, posicion.y() - 50);
        imgExplosion[i]->setVisible(true);  // Hacer visibles los ítems temporalmente

        QTimer::singleShot(200, this, [=]() {
            imgExplosion[i]->setVisible(false);  // Volverlos invisibles tras el tiempo
        });
    }

}




MainWindow::~MainWindow()
{
    delete ui;
}


