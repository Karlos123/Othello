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
    GuiBoardArea(Game g, QWidget *parent = 0);
    Game game{8, HUMAN, HUMAN, AI_AB};
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    QRadialGradient setStoneType(int player);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event);

private:
    int fieldSize;         // Velkost policka jedneho kamena - aby sa to nemuselo dookola prepocitavat
};

#endif // GUIBOARDAREA_H
