#ifndef GUIBOARDAREA_H
#define GUIBOARDAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "game.hpp"

class GuiBoardArea : public QWidget
{
    Q_OBJECT

public:
    GuiBoardArea(int X, TPlayer A, TPlayer B, TAI AI, QWidget *parent = 0);
    Game game;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    QRadialGradient setStoneType(TColor player, int fSize = 0);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event);

private:
    int fieldSize;         // Velkost policka jedneho kamena - aby sa to nemuselo dookola prepocitavat
    char invalidField;     // Pole, na ktore klikol uzivatel a nie je tam mozne umiestnit kamen
};

#endif // GUIBOARDAREA_H
