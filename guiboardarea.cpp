#include "guiboardarea.hpp"

#include <QThread>
#include <QPainter>
#include <QMouseEvent>
#include <QFontDatabase>
#include <iostream>


/**
 * @brief Konstruktor vykreslovacej plochy, parametre su rovnake ako tie u konstruktoru hry.
 * @param X Velikost herni desky
 * @param A Logika hrace hrajiciho za cerne kameny
 * @param B Logika hrace hrajiciho za bile kameny
 * @param AI Typ inteligence pocitace (obtiznost)
 * Nastavi zakladne nastavenia vykreslovacej plochy, ako napr. automaticke vykreslovanie pozadia, velkost a typ pisma.
 */
GuiBoardArea::GuiBoardArea(int X, TPlayer A, TPlayer B, TAI AI, QWidget *parent) : QWidget(parent), game{X, A, B, AI} {
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  QFontDatabase::addApplicationFont(":/res/rockwell.ttf");

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


/**
 * @brief Nastavi velkost pola pre kamen na zaklade velkosti vykreslovacej plochy.
 * @param event ResizeEvent, na ktory tato metoda reaguje
 */
void GuiBoardArea::resizeEvent(QResizeEvent * event){
  if(event->size().width() < event->size().height())
    fieldSize = event->size().width();
  else
    fieldSize = event->size().height();
  fieldSize /= game.board.getSize();
}


/**
 * @brief Reaguje na stlacenie mysi, cim vykona tah ludskeho hraca. Ak je nasledne za nim na tahu AI, tak vykona svoj tah, resp. svoje tahy.
 * @param event MousePressEvent, na ktory tato metoda reaguje
 */
void GuiBoardArea::mousePressEvent(QMouseEvent *event){
  if(game.isEnd())
  exit(0);

  if (event->button() == Qt::LeftButton) {
  int x = event->x()/fieldSize; // stlpec (1, 2, ...)
    int y = event->y()/fieldSize; // riadok (a, b, ...)
    if(x < game.board.getSize() && y < game.board.getSize()){
      if(game.onTurnAI() == HUMAN && !game.execTurnHuman(x, y)){ // Hrac previedol ilegalny tah, vyznaci sa stlacene pole
        invalidField = (x+1) * 16 + y + 1;
        repaint();
        QThread::msleep(150);
        invalidField = 0;
        repaint();
        return;
      }
      repaint();
      while(game.onTurnAI() == AI && !game.isEnd()){ // Vykonavaju sa tahy AI
        game.execTurnAI();
        QThread::msleep(500);
        repaint();
      }
    }
  }
}


/**
 * @brief Vytvori gradient na kamen, ktory sa aktualne vykresluje.
 * @param player Hrac, ktory je na tahu
 * @param fSize Velkost pola, specifikuje sa pre vykreslenie kamenov vedla hracej plochy
 * Vytvori radialny gradient pre vykreslovany kamen podla toho, ktory hrac je na tahu.
 */
QRadialGradient GuiBoardArea::setStoneType(TColor player, int fSize){
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


/**
 * @brief Prekreslovanie plochy hracej dosky a skore. Vykonava sa pri kazdej zmene hracej dosky.
 */
void GuiBoardArea::paintEvent(QPaintEvent *){
  // Zakladne nastavenie plochy pre vykreslovanie
  int boardSize = game.board.getSize();
  int gridOffset = ((width() > height() ? height() : width()) - fieldSize*boardSize)/2;
  QPainter painter(this);
  painter.translate(gridOffset, gridOffset);  // Posunutie od laveho horneho rohu, esteticka zalezitost
  painter.setRenderHint(QPainter::Antialiasing, true);

  // Vyplnenie hracej dosky zelenou farbou na pozadi
  painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
  painter.drawRect(QRect(0, 0, fieldSize*boardSize, fieldSize*boardSize));

  // Obdlznik (vlastne stvorec), podla ktoreho sa budu vykreslovat kamene
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

  // Vykreslenie informacii o hre vedla hracej plochy
  painter.save();
  painter.translate(boardSize*fieldSize, 0);
  int fSize = fieldSize*boardSize/8; // pouzivany ako default velkost kamenov, ktore sa vykresluju vedla hracej plochy

  // Nastavenie pisma pre vypis informacii
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

  // Vykreslenie "Game Over" screen - netvori sa nove okno, len sa hracia plocha prekryje a vykresli sa okienko
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
