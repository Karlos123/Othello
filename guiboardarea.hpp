#ifndef GUIBOARDAREA_H
#define GUIBOARDAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "game.hpp"


/**
 * @class GuiBoardArea
 *
 * @brief Vykreslovacia plocha
 *
 * Trieda v sebe uchovava potrebne informacie pre beh a ovladanie hry pomocou grafickeho uzivatelskeho rozhrania.
 * Predstavuje plochu, na ktoru sa vykresluju kamene, kto je na tahu a skore.
 */
class GuiBoardArea : public QWidget{
    Q_OBJECT

  public:
    GuiBoardArea(int X, TPlayer A, TPlayer B, TAI AI, QWidget *parent = 0); /* Konstruktor renderovacej plochy */
    Game game; /**< Samotna hra, ktora bude ovladana touto plochou */

  private:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE; /* Udalost kliknutia mysi, ktora tvori zaklad ovladania hry. */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE; /* Udalost vykreslovania obrazovky, ktora sa vykonava po kazdej zmene hracej plochy. */
    void resizeEvent(QResizeEvent *event); /* Udalost zmeny velkosti renderovacej plochy, ktora aktualizuje velkost jedneho policka. */
    int fieldSize; /**< Velkost policka jedneho kamena na renderovacej ploche */
    uchar invalidField; /**< Pole, na ktore klikol uzivatel a nie je tam mozne umiestnit kamen, teda pole ilegalneho tahu */
    QRadialGradient setStoneType(TColor player, int fSize = 0); /* Nastavenie gradientu pre vykreslovanie kamenov na hraciu plochu */
};

#endif // GUIBOARDAREA_H
