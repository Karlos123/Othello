#ifndef GUIBOARDAREA_H
#define GUIBOARDAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

class GuiBoardArea : public QWidget
{
    Q_OBJECT

public:

    GuiBoardArea(QWidget *parent = 0);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void setSize(const int size);
    void setGrad(int player);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int boardSize;         // Velkost pola - odstranit
    int fieldSize;         // Velkost policka jedneho kamena - aby sa to nemuselo dookola prepocitavat
    QRadialGradient rgrad; // Radialny gradient kamena - pouzivany na testovacie ucely, teraz v sebe
                           // uschovava gradient aktualne vyplnovaneho kamena - zrejme odstranit
};

#endif // GUIBOARDAREA_H
