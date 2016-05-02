#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class QCheckBox;
class QComboBox;
class QLabel;
class QSpinBox;

class GuiBoardArea;

class GuiWindow : public QWidget
{
    Q_OBJECT

public:
    GuiWindow();

private slots:
    void newGame();
    void loadGame();
    void game(QByteArray save = "");
    void saveGame();
    void clearLayout();
    void histBack();
    void histForw();

private:
    GuiBoardArea *boardArea;
    QComboBox *boardSizeComboBox; // Vyber velkosti hracieho pola
    QComboBox *gameTypeComboBox;  // Vyber typu hry
    QComboBox *aiSelectComboBox;  // Vyber AI
    QLineEdit *fileNameLine;      // Nazov suboru
};

#endif // GUIWINDOW_H
