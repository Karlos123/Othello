#include "guiboardarea.hpp"
#include "guiwindow.hpp"

#include <QtWidgets>
#include <QVBoxLayout>

const int IdRole = Qt::UserRole;


/**
 * @brief Vytvori hlavne menu, nastavi nemennu velkost okna a vytvori plochu
 * na vykreslenie hracej dosky
 */
GuiWindow::GuiWindow()
{
    this->setFixedSize(size());

    // MAIN MENU

    // Nadpis "Othello"
    QLabel *appNameLabel = new QLabel(tr("Othello"));
    QFont appNameLabelFont = appNameLabel->font();
    appNameLabelFont.setWeight(QFont::Bold);
    appNameLabelFont.setPointSize(appNameLabelFont.pointSize()*3);
    appNameLabel->setFont(appNameLabelFont);
    appNameLabel->setScaledContents(true);
    appNameLabel->setAlignment(Qt::AlignCenter);

    // "New Game" tlacidlo
    QPushButton *newGameButton = new QPushButton;
    newGameButton->setMinimumHeight(newGameButton->height()*0.1);
    QFont newGameButtonFont = newGameButton->font();
    newGameButtonFont.setPointSize(newGameButtonFont.pointSize()*1.5);
    newGameButton->setFont(newGameButtonFont);
    newGameButton->setText(tr("New Game"));

    // "Load Game" tlacidlo
    QPushButton *loadGameButton = new QPushButton;
    loadGameButton->setMinimumHeight(loadGameButton->height()*0.1);
    QFont loadGameButtonFont = loadGameButton->font();
    loadGameButtonFont.setPointSize(loadGameButtonFont.pointSize()*1.5);
    loadGameButton->setFont(loadGameButtonFont);
    loadGameButton->setText(tr("Load Game"));

    // Pripojenie stlacenia tlacidla na obsluzne funkcie, ktore zmenia menu
    connect(newGameButton, SIGNAL(clicked(bool)), this, SLOT(newGame()));
    connect(loadGameButton, SIGNAL(clicked(bool)), this, SLOT(loadGame()));

    // Vytvorenie Vertical Box layoutu a vlozenie widgetov
    QVBoxLayout *mainMenuLayout = new QVBoxLayout;
    mainMenuLayout->addWidget(appNameLabel);
    mainMenuLayout->addWidget(newGameButton);
    mainMenuLayout->addWidget(loadGameButton);

    setLayout(mainMenuLayout);

    setWindowTitle(tr("Othello"));
    ///TODO: pridat ikonu okna
}


/**
 * @brief Odstrani aktualny layout, je to zredukovana verzia kodu, ktory dal
 * nejaky spasitel na StackOverflow (ten kod co tam bol spracovaval aj sublayouts,
 * ktore my nepouzivame). Bez tejto funkcie by zostavali predosle nadpisy tlacidla
 * atd na dalsom rozhrani
 */
void GuiWindow::clearLayout()
{
    QLayout *layout = this->layout();
    QLayoutItem *item;
    QWidget *widget;
    while ((item = layout->takeAt(0))) {
        if ((widget = item->widget()) != 0) {
            widget->hide();
            delete widget;
        }
        else
            delete item;
    }
    delete layout;
}


/**
 * @brief "New Game" menu, umoznuje vybrat si velkost pola, typ hry a typ AI
 */
void GuiWindow::newGame()
{
    clearLayout();

    // NEW GAME MENU

    // Nadpis "New Game Options"
    QLabel *newGameMenuLabel = new QLabel(tr("New Game Options"));
    QFont newGameMenuLabelFont = newGameMenuLabel->font();
    newGameMenuLabelFont.setWeight(QFont::Bold);
    newGameMenuLabelFont.setPointSize(newGameMenuLabelFont.pointSize()*2);
    newGameMenuLabel->setFont(newGameMenuLabelFont);
    newGameMenuLabel->setScaledContents(true);
    newGameMenuLabel->setAlignment(Qt::AlignCenter);

    // ComboBox na vyber velkosti pola + popis
    boardSizeComboBox = new QComboBox;
    boardSizeComboBox->addItem(tr("6"), 6);
    boardSizeComboBox->addItem(tr("8"), 8);
    boardSizeComboBox->addItem(tr("10"), 10);
    boardSizeComboBox->addItem(tr("12"), 12);
    boardSizeComboBox->setCurrentIndex(1);
    boardSizeComboBox->setMinimumHeight(boardSizeComboBox->height()*0.05);
    QLabel *boardSizeLabel = new QLabel(tr("Board &Size:"));
    boardSizeLabel->setBuddy(boardSizeComboBox);
    boardSizeLabel->setAlignment(Qt::AlignBottom);

    // ComboBox na vyber typu hry + popis
    gameTypeComboBox = new QComboBox;
    gameTypeComboBox->addItem(tr("Player vs. Player"), 0);
    gameTypeComboBox->addItem(tr("Player vs. AI"), 1);
    //gameTypeComboBox->addItem(tr("AI vs. AI"), 2);
    gameTypeComboBox->setCurrentIndex(1);
    gameTypeComboBox->setMinimumHeight(gameTypeComboBox->height()*0.05);
    QLabel *gameTypeLabel = new QLabel(tr("Game type:"));
    gameTypeLabel->setBuddy(gameTypeComboBox);
    gameTypeLabel->setAlignment(Qt::AlignBottom);

    // ComboBox na vyber typu AI + popis
    aiSelectComboBox = new QComboBox;
    aiSelectComboBox->addItem(tr("AI 1 (easy)"), 0);
    aiSelectComboBox->addItem(tr("AI 2 (difficult)"), 1);
    aiSelectComboBox->setMinimumHeight(aiSelectComboBox->height()*0.05);
    QLabel *aiSelectLabel = new QLabel(tr("AI:"));
    aiSelectLabel->setBuddy(aiSelectComboBox);
    aiSelectLabel->setAlignment(Qt::AlignBottom);

    // Tlacidlo na zacatie hry
    QPushButton *startButton = new QPushButton;
    startButton->setMinimumHeight(startButton->height()*0.1);
    QFont startButtonFont = startButton->font();
    startButtonFont.setPointSize(startButtonFont.pointSize()*1.5);
    startButton->setFont(startButtonFont);
    startButton->setText(tr("Start"));

    // vlozit sem connecty na event handlery, teoreticky by stacil jediny connect - na tlacidlo start hry
    // v tom pripade by sa vlastnosti hry zvolene uzivatelom nastavili pred zacatim hry a nereagovalo by sa na zbytocne eventy
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(game()));

    // Vertical Box Layout - widgety sa ukladaju pod seba v poradi, v akom sme ich vlozili
    QVBoxLayout *newGameMenuLayout = new QVBoxLayout;
    newGameMenuLayout->addWidget(newGameMenuLabel);
    newGameMenuLayout->addWidget(gameTypeLabel);
    newGameMenuLayout->addWidget(gameTypeComboBox);
    newGameMenuLayout->addWidget(boardSizeLabel);
    newGameMenuLayout->addWidget(boardSizeComboBox);
    newGameMenuLayout->addWidget(aiSelectLabel);
    newGameMenuLayout->addWidget(aiSelectComboBox);
    newGameMenuLayout->addWidget(startButton);

    // Nastavime layout a aktivujeme sizeChanged, aby sme nastavili velkost hracej plochy
    setLayout(newGameMenuLayout);
}


/**
 * @brief "Load Game" menu, treba pridat handler na nacitanie suboru daneho uzivatelom,
 * ak nie je zadany tak aktivovat "New Game" menu?
 */
void GuiWindow::loadGame()
{
    /*
    clearLayout();

    // LOAD GAME MENU

    // Nadpis "New Game Options"
    QLabel *loadGameMenuLabel = new QLabel(tr("Load Game"));
    QFont loadGameMenuLabelFont = loadGameMenuLabel->font();
    loadGameMenuLabelFont.setWeight(QFont::Bold);
    loadGameMenuLabelFont.setPointSize(loadGameMenuLabelFont.pointSize()*2);
    loadGameMenuLabel->setFont(loadGameMenuLabelFont);
    loadGameMenuLabel->setScaledContents(true);
    loadGameMenuLabel->setAlignment(Qt::AlignCenter);

    // Riadok, kam sa ma zadat cesta k suboru s ulozenou hrou
    fileNameLine = new QLineEdit;
    QLabel *fileNameLabel = new QLabel(tr("Path to the save file:"));
    fileNameLabel->setBuddy(fileNameLine);
    fileNameLabel->setAlignment(Qt::AlignBottom);

    // Tlacidlo na nacitanie hry
    QPushButton *loadButton = new QPushButton;
    loadButton->setMinimumHeight(loadButton->height()*0.1);
    QFont loadButtonFont = loadButton->font();
    loadButtonFont.setPointSize(loadButtonFont.pointSize()*1.5);
    loadButton->setFont(loadButtonFont);
    loadButton->setText(tr("Load"));

    // vlozit sem connecty na event handlery - aktualne to sposobi crash,
    // ako by to spravilo default nastavenie novej hry
    connect(loadButton, SIGNAL(clicked(bool)), this, SLOT(saveGame()));

    // Otrepany Vertical Box layout
    QVBoxLayout *loadGameMenuLayout = new QVBoxLayout;
    loadGameMenuLayout->addWidget(loadGameMenuLabel);
    loadGameMenuLayout->addWidget(fileNameLabel);
    loadGameMenuLayout->addWidget(fileNameLine);
    loadGameMenuLayout->addWidget(loadButton);

    setLayout(loadGameMenuLayout);
    */

    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Saved Game"), "", tr("Othello Save Game Files (*.osf)"));
    if(fileName.isNull())
        return;

    QByteArray save = boardArea->game.loadGame(fileName);
    if(save == "")
        return;

    game(save);
}

/**
 * @brief
 */
void GuiWindow::game(QByteArray save)
{
    uchar boardSize, pve, ai;
    if(save.isEmpty()){
        boardSize = boardSizeComboBox->itemData(boardSizeComboBox->currentIndex(), IdRole).toInt();
        pve = gameTypeComboBox->itemData(gameTypeComboBox->currentIndex(), IdRole).toInt();
        ai = aiSelectComboBox->itemData(aiSelectComboBox->currentIndex(), IdRole).toInt();
    }
    else{
        boardSize = save.at(0);
        pve = save.at(1);
        ai = save.at(1) - 1;
    }

    clearLayout();

    boardArea = new GuiBoardArea(boardSize, HUMAN, pve? AI: HUMAN, static_cast<TAI>(ai));
    if(!save.isEmpty()) // Vykonavanie nacitanych tahov z ulozenej hry
        for(int i = 2; i < save.length(); i++){
            //std::cout << "Vykonavam tah c. " << i-1 << ": x = " << save.at(i)/16-1 << ", y = " << save.at(i)%16-1 << std::endl;
            boardArea->game.execTurnHuman(static_cast<uchar>(save.at(i))/16-1, static_cast<uchar>(save.at(i))%16-1);
        }
    //boardArea->repaint();

    // Pridat tlacidlo na historiu apod.

    QPushButton *saveGameButton = new QPushButton;
    QFont saveGameButtonFont = saveGameButton->font();
    saveGameButtonFont.setPointSize(saveGameButtonFont.pointSize()*1.5);
    saveGameButton->setFont(saveGameButtonFont);
    saveGameButton->setText(tr("Save Game"));

    QPushButton *histBackButton = new QPushButton;
    QFont histBackButtonFont = histBackButton->font();
    histBackButtonFont.setPointSize(histBackButtonFont.pointSize()*1.5);
    histBackButton->setFont(histBackButtonFont);
    histBackButton->setText(tr("◄")); // ◄ ←
    /*QPushButton *histRevtButton = new QPushButton;
    QFont histRevtButtonFont = histRevtButton->font();
    histRevtButtonFont.setPointSize(histRevtButtonFont.pointSize()*1.5);
    histRevtButton->setFont(histRevtButtonFont);
    histRevtButton->setText(tr("Revert"));*/
    QPushButton *histForwButton = new QPushButton;
    QFont histForwButtonFont = histForwButton->font();
    histForwButtonFont.setPointSize(histForwButtonFont.pointSize()*1.5);
    histForwButton->setFont(histForwButtonFont);
    histForwButton->setText(tr("►")); // ► →


    connect(histBackButton, SIGNAL(clicked(bool)), this, SLOT(histBack()));
    connect(histForwButton, SIGNAL(clicked(bool)), this, SLOT(histForw()));
    connect(saveGameButton, SIGNAL(clicked(bool)), this, SLOT(saveGame()));


    // Grid layout - konecne nieco ine nez Vertical Box
    QGridLayout *gameLayout = new QGridLayout;
    //gameLayout->setColumnStretch(0, 1);
    //gameLayout->setColumnStretch(4, 1);
    gameLayout->addWidget(boardArea, 0, 0, 5, 5);
    gameLayout->addWidget(histBackButton, 5, 0, 1, 1);
   // gameLayout->addWidget(histRevtButton, 5, 1, 1, 1);
    gameLayout->addWidget(histForwButton, 5, 1, 1, 1);
    gameLayout->addWidget(saveGameButton, 5, 4, 1, 1);
    setLayout(gameLayout);
}

void GuiWindow::saveGame()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Game..."), "", tr("Othello Save Game Files (*.osf)"));
    if(fileName.isNull())
        return;

    // Aktualne to dookola ponuka ulozenie hry kym sa ju nepodari ulozit, alebo uzivatel neda Cancel
    if(boardArea->game.saveGame(fileName))
        saveGame();
}


void GuiWindow::histBack()
{
    do boardArea->game.setState(boardArea->game.history.prevState());
    while (boardArea->game.onTurnAI() == AI);
    boardArea->repaint();
}


void GuiWindow::histForw()
{
    do boardArea->game.setState(boardArea->game.history.nextState());
    while (boardArea->game.onTurnAI() == AI);
    boardArea->repaint();
}
