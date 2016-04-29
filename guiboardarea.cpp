#include "guiboardarea.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <iostream>

// Texturu pozadia by bolo zrejme vhodne vlozit sem
GuiBoardArea::GuiBoardArea(Game g, QWidget *parent)
    : QWidget(parent)
{
    game = g;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    /*
    std::cout << "W: " << width() << ", H: " << height() << std::endl;
    if(width() < height())
        fieldSize = width();
    else
        fieldSize = height();
    fieldSize /= g.board.getSize();
    */
}

// Qt bez tejto metody vrieska ako male dieta
QSize GuiBoardArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

// ditto
QSize GuiBoardArea::sizeHint() const
{
    return QSize(400, 200);
}


void GuiBoardArea::resizeEvent(QResizeEvent * event)
{   // Tu ani nemusia byt tie event->size(), ale ked je tu ciste len width() tak
    // prekladac sa stazuje ze je event nepouzity
    if(event->size().width() < event->size().height())
        fieldSize = event->size().width();
    else
        fieldSize = event->size().height();
    fieldSize /= game.board.getSize();
}

void GuiBoardArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int x = event->x()/fieldSize; // stlpec (1, 2, ...)
        int y = event->y()/fieldSize; // riadok (a, b, ...)
        if(x < game.board.getSize() && y < game.board.getSize()){
            std::cout << "Hrac na tahu...\n";
            if(!game.execTurnHuman(x, y)) return;
            update();
            if(game.isEnd()){
                std::cout << "Koniec hry.\n";
                // fanfary a ine sracky
                exit(0);
            }
            if(game.onTurnAI() == AI){
                std::cout << "Na tahu je AI...\n";
                game.execTurnAI();
                update();
                if(game.isEnd()){
                    std::cout << "Koniec hry.\n";
                    // fanfary a ine sracky - part 2
                    exit(0);
                }
            }
        }
    }
}


// Vrati gradient na kamen hraca player
QRadialGradient GuiBoardArea::setStoneType(int player)
{
    QRadialGradient radialGradient (fieldSize*5/8, fieldSize*5/8, fieldSize*5/8, fieldSize*5/16, fieldSize*3/16);
    switch(player){
        case 0:
            radialGradient.setColorAt(0, Qt::gray);
            radialGradient.setColorAt(0.05, Qt::darkGray);
            radialGradient.setColorAt(0.3, Qt::black);
            break;
        case 1:
            radialGradient.setColorAt(0, Qt::white);
            radialGradient.setColorAt(0.2, Qt::white);
            radialGradient.setColorAt(1.0, Qt::black);
            break;
    }
    return radialGradient;
    //update();
}


// Metoda, ktora vykresluje hraciu plochu
void GuiBoardArea::paintEvent(QPaintEvent * /* event */)
{
    int boardSize = game.board.getSize();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Obdlznik (vlastne stvorec) podla ktoreho sa spravi kamen
    QRect rect(fieldSize/8, fieldSize/8, fieldSize*3/4, fieldSize*3/4);

    // Ziskanie kamenov a ich vkladanie do hracej plochy - aktualne to len vyplni
    // hraciu plochu kamenmi
    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            painter.save();
            painter.translate(x*fieldSize, y*fieldSize);
            if(game.board.getStone(x, y)){
                painter.setBrush(setStoneType(game.board.getStone(x, y)-1));
                painter.drawEllipse(rect);
            }
            painter.restore();
        }
    }

    // Vykreslovanie mriezky okolo hracieho pola
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, fieldSize*boardSize, fieldSize*boardSize));
    for(int i = 1; i < boardSize/2; i++){
        painter.drawRect(QRect(i*fieldSize - 1, 0, fieldSize*boardSize - 2*i*fieldSize, fieldSize*boardSize));
        painter.drawRect(QRect(0, i*fieldSize - 1, fieldSize*boardSize, fieldSize*boardSize - 2*i*fieldSize));
    }
    painter.drawLine(0, fieldSize*boardSize/2, fieldSize*boardSize, fieldSize*boardSize/2);
    painter.drawLine(fieldSize*boardSize/2, 0, fieldSize*boardSize/2, fieldSize*boardSize);
}
