#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

/* Triedy widgetov vyuzivane v hlavnom okne */
class QComboBox;
class QLabel;
class GuiBoardArea;


/**
 * @class GuiWindow
 *
 * @brief Okno aplikacie
 *
 * Predstavuje samotne okno aplikacie, v ktorom su ukladane prvky pomocou rozlozeni v zavislosti od aktualneho menu.
 * Sluzi pre nastavenie novej hry, nacitanie ulozenej hry, alebo ovladanie prebiehajucej hry tlacidlami.
 */
class GuiWindow : public QWidget
{
    Q_OBJECT

public:
    GuiWindow();

private slots:
    void mainMenu(); /* Hlavne menu, ktore umoznuje si vybrat novu hru alebo nacitanie rozohranej hry */
    void newGame(); /* Menu moznosti novej hry */
    void loadGame(); /* Otvorenie "Open file" dialogu a nacitanie obsahu suboru ulozenej hry */
    void game(QByteArray save = ""); /* Menu samotnej hry */
    void saveGame(); /* Otvorenie "Save file" dialogu a ulozenie hry */
    void clearLayout(); /* Metoda pre vycistenie rozlozenia okna */
    void histBack(); /* Reaguje na kliknutie na tlacidlo navratu v historii */
    void histForw(); /* Reaguje na kliknutie na tlacidlo posunu vpred v historii */
    void hideAISelection(); /* Skryje vyber AI, pokial si hrac zvolil ako oponenta ineho hraca */

protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    GuiBoardArea *boardArea; /**< Renderovacia plocha, na ktoru sa bude vykreslovat hracia doska */
    QComboBox *boardSizeComboBox; /**< Vyber velkosti hracieho pola */
    QComboBox *gameTypeComboBox; /**< Vyber typu hry */
    QComboBox *aiSelectComboBox; /**< Vyber AI */
    QLabel *aiSelectLabel; /**< Napis "AI:" v menu novej hry */
    bool gameInitialized; /**< Uchovava informaciu, ci hra uz bola vytvorena, resp. nacitana */
};

#endif // GUIWINDOW_H
