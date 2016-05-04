#include "guiboardarea.hpp"

#include <QThread>
#include <QPainter>
#include <QMouseEvent>
#include <QFontDatabase>
#include <iostream>

// Texturu pozadia by bolo zrejme vhodne vlozit sem
GuiBoardArea::GuiBoardArea(int X, TPlayer A, TPlayer B, TAI AI, QWidget *parent)
    : QWidget(parent), game{X, A, B, AI}
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    QFontDatabase::addApplicationFont(":/res/rockwell.ttf");
    //QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    //QFont rockwell(family);

    QFont f = this->font();
    f.setFamily("Rockwell");
    this->setFont(f);

    if(width() < height())
        fieldSize = width();
    else
        fieldSize = height();
    fieldSize /= game.board.getSize();

    invalidField = 0;
}

// Qt bez tejto metody vrieska ako male dieta, nastavenie najmensej velkosti okna
QSize GuiBoardArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

//
QSize GuiBoardArea::sizeHint() const
{
    return QSize(400, 200);
}

// Okno sa pri vkladani widgetov do layoutu automaticky resizuje, cize je
// potrebne takto aktualizovat velkost fieldSize
void GuiBoardArea::resizeEvent(QResizeEvent * event)
{   // Tu ani nemusia byt tie event->size(), ale ked je tu ciste len width() tak
    // prekladac sa stazuje ze je event nepouzity
    if(event->size().width() < event->size().height())
        fieldSize = event->size().width();
    else
        fieldSize = event->size().height();
    fieldSize /= game.board.getSize();
}


// Event reagujuci na stlacenie mysi, vykonava samotnu hru
void GuiBoardArea::mousePressEvent(QMouseEvent *event)
{
    if(game.isEnd())
        exit(0);

    if (event->button() == Qt::LeftButton) {
        int x = event->x()/fieldSize; // stlpec (1, 2, ...)
        int y = event->y()/fieldSize; // riadok (a, b, ...)
        if(x < game.board.getSize() && y < game.board.getSize()){
            if(game.onTurnAI() == HUMAN && !game.execTurnHuman(x, y)){ // game.onTurnAI by v tomto bode malo vzdy vraciat HUMAN
                invalidField = (x+1) * 16 + y + 1;
                repaint();
                QThread::msleep(150);
                invalidField = 0;
                repaint();
                return;
            }
            repaint();
            while(game.onTurnAI() == AI && !game.isEnd()){
                game.execTurnAI();
                QThread::msleep(500);
                repaint();
            }
        }
    }
}


// Vrati gradient na kamen hraca player, fSize je volitelny parameter pouzivany
// pre vykreslenie kamenov v GUI vedla hracej plochy
QRadialGradient GuiBoardArea::setStoneType(TColor player, int fSize)
{
    if(!fSize)
        fSize = fieldSize;
    QRadialGradient radialGradient (fSize*5/8, fSize*5/8, fSize*5/8, fSize*5/16, fSize*3/16);
    switch(player){
        case BLACK:
            radialGradient.setColorAt(0, Qt::gray);
            radialGradient.setColorAt(0.05, Qt::darkGray);
            radialGradient.setColorAt(0.3, Qt::black);
            break;
        case WHITE:
            radialGradient.setColorAt(0, Qt::white);
            radialGradient.setColorAt(0.2, Qt::white);
            radialGradient.setColorAt(1.0, Qt::black);
            break;
        case NONE:
        case MARKSTONE:
            // ???
            break;
    }
    return radialGradient;
}


// Metoda, ktora vykresluje hraciu plochu
void GuiBoardArea::paintEvent(QPaintEvent * /* event */)
{
    int boardSize = game.board.getSize();
    QPainter painter(this);
    int gridOffset = ((width() > height() ? height() : width()) - fieldSize*boardSize)/2;
    painter.translate(gridOffset, gridOffset);  // Posunutie od laveho horneho rohu, lepsie to potom vyzera
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
    painter.drawRect(QRect(0, 0, fieldSize*boardSize, fieldSize*boardSize));

    // Obdlznik (vlastne stvorec) podla ktoreho sa spravi kamen
    QRect rect(fieldSize/8, fieldSize/8, fieldSize*3/4, fieldSize*3/4);

    // Vykreslenie kamenov v hracom poli
    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            TColor c = game.board.getStone(x, y);
            if(c == NONE)
                continue;

            painter.save();
            painter.translate(x*fieldSize, y*fieldSize);
            QBrush b = painter.brush();
            QPen p = painter.pen();
            if(c == MARKSTONE){
                if(game.onTurnColor() == BLACK)
                    p.setColor(Qt::black);
                else
                    p.setColor(Qt::lightGray);
                p.setStyle(Qt::DotLine);
                b.setStyle(Qt::NoBrush);
            }
            else{
                if(c == BLACK)
                    p.setColor(Qt::black);
                else
                    p.setColor(Qt::darkGray);
                p.setStyle(Qt::SolidLine);
                b = setStoneType(c);
            }
            painter.setBrush(b);
            painter.setPen(p);
            painter.drawEllipse(rect);
            painter.restore();
        }
    }

    // Rendering informacii o hre vedla hracej plochy
    painter.save();
    painter.translate(boardSize*fieldSize, 0);
    int fSize = fieldSize*boardSize/8; // pouzivany ako default velkost kamenov, ktore sa vykresluju vedla hracej plochy

    QFont font = painter.font();
    font.setPointSize(8);
    font.setPointSizeF(8.25);
    font.setPointSize(font.pointSize() * 2);
    painter.setFont(font);

    // Vypisanie textu vedla hracej plochy
    painter.drawText(fSize*0.5, fSize*0.625, "On turn:");
    font.setPointSize(font.pointSize() * 1.5);
    painter.setFont(font);
    painter.drawText(fSize*1.15, fSize*2.625, "Score");
    font.setPointSize(font.pointSize() * 0.7);
    painter.setFont(font);
    painter.drawText(fSize*1.2, fSize*3.625, QString::number(game.getScore(BLACK)));
    painter.drawText(fSize*1.2, fSize*4.625, QString::number(game.getScore(WHITE)));

    // Vykreslenie kamena hraca na tahu
    painter.translate(fSize*2.25, fSize*0.05);
    painter.setBrush(setStoneType(game.onTurnColor(), fSize));
    painter.drawEllipse(QRect(fSize/8, fSize/8, fSize*3/4, fSize*3/4));

    // Vykreslenie kamenov vedla skore
    painter.restore();
    painter.save();
    painter.translate(boardSize*fieldSize, 0);
    painter.translate(fSize*1.75, 0);
    painter.translate(0, fSize*3);
    painter.setBrush(setStoneType(BLACK, fSize));
    painter.drawEllipse(QRect(fSize/8, fSize/8, fSize*3/4, fSize*3/4));
    painter.translate(0, fSize);
    painter.setBrush(setStoneType(WHITE, fSize));
    painter.drawEllipse(QRect(fSize/8, fSize/8, fSize*3/4, fSize*3/4));
    painter.restore();

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

    // Vykreslenie "Game Over" screen - netvori sa nove okno, len sa proste hracia
    // plocha prekryje a vyrenderuje sa okienko
    if(game.isEnd()){
        // Prekrytie plochy sivou clonou
        painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
        painter.setOpacity(0.5);
        painter.drawRect(QRect(0, 0, width(), height()));

        // Vykreslenie okienka v strede
        painter.setOpacity(1);
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.drawRect(QRect(width()/2 - 150, height()/2 - 100, 300, 150));

        // Vypis informacii v okienku
        font.setPointSize(font.pointSize() * 2);
        painter.setFont(font);
        painter.drawText(width()/2-115, height()/2-50, "Game Over");
        font.setPointSize(font.pointSize() * 0.5);
        painter.setFont(font);
        if(game.getScore(BLACK) > game.getScore(WHITE))
            painter.drawText(width()/2-50, height()/2, "Black wins");
        else if(game.getScore(BLACK) < game.getScore(WHITE))
            painter.drawText(width()/2-52, height()/2, "White wins");
        else
            painter.drawText(width()/2-25, height()/2, "Draw");
        font.setPointSize(font.pointSize() * 0.5);
        painter.setFont(font);
        painter.drawText(width()/2-90, height()/2+45, "Click anywhere to close this window");
        return;
    }

    // Vyfarbenie stvorceka, kam sa hrac pokusil vykonat ilegalny tah
    if(invalidField){
        uchar invX, invY;
        invX = invalidField/16 - 1;
        invY = invalidField%16 - 1;
        if(invX >= boardSize || invY >= boardSize)
            return;
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.setOpacity(0.25);
        painter.drawRect(QRect(invX*fieldSize-1, invY*fieldSize-1, fieldSize, fieldSize));
        painter.setOpacity(1);
    }
}
