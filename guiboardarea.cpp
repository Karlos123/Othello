#include "guiboardarea.hpp"

#include <QPainter>

// Texturu pozadia by bolo zrejme vhodne vlozit sem
GuiBoardArea::GuiBoardArea(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
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

// Nastavi velkost hracej plochy - odstranit
void GuiBoardArea::setSize(const int size)
{
    this->boardSize = size;
    update();
}

// Nastavi atribut rgrad na aktualny kamen
void GuiBoardArea::setGrad(int player)
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
    this->rgrad = radialGradient;
    update();
}


// Metoda, ktora vykresluje hraciu plochu
void GuiBoardArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Toto sa presunie, nech sa to vypocita raz pri zacati novej hry a nech sa
    // to nevypocitava stale dookola - bolo to uzitocne ked sa dalo menit
    // velkost okna
    if(width() < height())
        fieldSize = width()/boardSize;
    else
        fieldSize = height()/boardSize;

    // Obdlznik (vlastne stvorec) podla ktoreho sa spravi kamen
    QRect rect(fieldSize/8, fieldSize/8, fieldSize*3/4, fieldSize*3/4);

    // Ziskanie kamenov a ich vkladanie do hracej plochy - aktualne to len vyplni
    // hraciu plochu kamenmi
    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            painter.save();
            painter.translate(x*fieldSize, y*fieldSize);
            setGrad((x+y)%2);
            painter.setBrush(rgrad);
            painter.drawEllipse(rect);
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
