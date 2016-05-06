#include "guiboardarea.hpp"
#include "guiwindow.hpp"

#include <QtWidgets>
#include <QVBoxLayout>

/**
 * @brief Nastavi nemennu velkost okna a nadpis, nastavi rozlozenie hlavneho menu.
 */
GuiWindow::GuiWindow()
{
    this->setFixedSize(width()/2.25, height()/1.75);
    gameInitialized = false;
    setWindowTitle(tr("Othello"));
    mainMenu();
}


/**
 * @brief Nastavi nadpis a tlacidla v hlavnom menu.
 */
void GuiWindow::mainMenu()
{
    // Nastavenie nadpisu "Othello"
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

    // Pripojenie stlacenia tlacidiel na obsluzne funkcie, ktore zmenia menu, resp. vyberu subor na nacitanie
    connect(newGameButton, SIGNAL(clicked(bool)), this, SLOT(newGame()));
    connect(loadGameButton, SIGNAL(clicked(bool)), this, SLOT(loadGame()));

    // Vytvorenie Vertical Box rozlozenia a vlozenie widgetov, aplikovanie rozlozenia
    QVBoxLayout *mainMenuLayout = new QVBoxLayout;
    mainMenuLayout->addWidget(appNameLabel);
    mainMenuLayout->addWidget(newGameButton);
    mainMenuLayout->addWidget(loadGameButton);

    setLayout(mainMenuLayout);
}


/**
 * @brief Skryje a odstrani vsetky widgety aktualneho rozlozenia, aby sa mohlo nastavit nove rozlozenie okna.
 */
void GuiWindow::clearLayout()
{
    QLayout *layout = this->layout();
    QLayoutItem *item;
    QWidget *widget;
    while((item = layout->takeAt(0))){
        if((widget = item->widget()) != 0){
            widget->hide();
            delete widget;
        }
        else
            delete item;
    }
    delete layout;
}


/**
 * @brief Menu novej hry, umoznuje vybrat si velkost pola, typ hry a typ AI.
 */
void GuiWindow::newGame()
{
    // Odstrani stare rozlozenie, tj. hlavne menu
    clearLayout();

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
    aiSelectComboBox->addItem(tr("AI 1 (easy)"), AI_EASY);
    aiSelectComboBox->addItem(tr("AI 2 (difficult)"), AI_DIFFICULT);
    aiSelectComboBox->setMinimumHeight(aiSelectComboBox->height()*0.05);
    aiSelectLabel = new QLabel(tr("AI:"));
    aiSelectLabel->setBuddy(aiSelectComboBox);
    aiSelectLabel->setAlignment(Qt::AlignBottom);

    // Tlacidlo na zacatie hry
    QPushButton *startButton = new QPushButton;
    startButton->setMinimumHeight(startButton->height()*0.1);
    QFont startButtonFont = startButton->font();
    startButtonFont.setPointSize(startButtonFont.pointSize()*1.5);
    startButton->setFont(startButtonFont);
    startButton->setText(tr("Start"));

    // Pripojenie interaktivnych prvkov na obsluzne funkcie
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(game())); // Pripojenie tlacidla startu hry
    connect(gameTypeComboBox, SIGNAL(activated(int)), this, SLOT(hideAISelection())); // Schovanie vyberu typu AI ak je zvolena hra proti hracovi

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

    // Nastavime rozlozenie
    setLayout(newGameMenuLayout);
}


/**
 * @brief Skontroluje, ci je zvolena hra proti hracovi, respektive proti AI a podla toho skryje/zobrazi vyber typu AI.
 */
void GuiWindow::hideAISelection(){
    int pve = gameTypeComboBox->itemData(gameTypeComboBox->currentIndex(), Qt::UserRole).toInt();
    if(pve){
        aiSelectComboBox->show();
        aiSelectLabel->show();
    }
    else{
        aiSelectComboBox->hide();
        aiSelectLabel->hide();
    }
}


/**
 * @brief Nacitavanie hry
 * Vyvola "Open file" dialog, kde si uzivatel moze vybrat subor ulozenej hry, ktory chce nacitat. Ak nie je mozne nacitat pozadovany subor alebo neexistuje, tak sa zrusi vyber suboru na nacitanie. Ak sa podarilo nacitavanie hry, tak sa vytvori nova hra s nacitanou konfiguraciou.
 */
void GuiWindow::loadGame()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Saved Game"), "", tr("Othello Save Game Files (*.osf)"));
    if(fileName.isNull())
        return;

    QByteArray save = boardArea->game.loadGame(fileName);
    if(save == "")
        return;

    game(save);
}


/**
 * @brief Rozlozenie okna pocas hry, in-game interface
 * @param save Bajtove pole ziskane zo suboru ulozenej hry
 * Vytvori vykreslovaciu plochu pre hraciu dosku a pozadovane informacie, vytvori v nej novu hru na zaklade zvolenych nastaveni a nastavi rozlozenie okna pre hru.
 * Ak je save prazdny retazec (implicitna moznost), tak sa tvori hra na zaklade uzivatelom zvolenych nastaveni; inak sa vytvori hra z bajtoveho pola save,
 * kde na prvom bajte je velkost hracej plochy, na druhom bajte je typ hry a AI a dalej je postupnost vykonanych tahov.
 */
void GuiWindow::game(QByteArray save)
{
    uchar boardSize, pve, ai;
    if(save.isEmpty()){ // Nova hra, ziskame zvolene nastavenia
        boardSize = boardSizeComboBox->itemData(boardSizeComboBox->currentIndex(), Qt::UserRole).toInt();
        pve = gameTypeComboBox->itemData(gameTypeComboBox->currentIndex(), Qt::UserRole).toInt();
        ai = aiSelectComboBox->itemData(aiSelectComboBox->currentIndex(), Qt::UserRole).toInt();
    }
    else{ // Nacitana hra, ziskame nastavenia z bajtoveho pola save
        boardSize = save.at(0);
        pve = save.at(1);
        ai = save.at(1) - 1;
    }

    // Cistenie rozlozenia musi byt vykonane po ziskavani informacii, lebo by sa inak vycistili aj ComboBoxy s nastaveniami
    clearLayout();

    // Vytvorenie renderovacej plochy a pripadne replikovanie vykonanych tahov z ulozenej hry
    boardArea = new GuiBoardArea(boardSize, HUMAN, pve? AI: HUMAN, static_cast<TAI>(ai));
    if(!save.isEmpty()){ // Vykonavanie nacitanych tahov z ulozenej hry
        boardArea->game.setOpponentType(HUMAN);
        for(int i = 2; i < save.length(); i++)
            boardArea->game.execTurnHuman(static_cast<uchar>(save.at(i))/16-1, static_cast<uchar>(save.at(i))%16-1);
        boardArea->game.setOpponentType(pve? AI: HUMAN);
    }

    // Tlacidlo na ulozenie hry
    QPushButton *saveGameButton = new QPushButton;
    QFont saveGameButtonFont = saveGameButton->font();
    saveGameButtonFont.setPointSize(saveGameButtonFont.pointSize()*1.5);
    saveGameButton->setFont(saveGameButtonFont);
    saveGameButton->setText(tr("Save Game"));

    // Tlacidlo pre navrat v historii
    QPushButton *histBackButton = new QPushButton;
    QFont histBackButtonFont = histBackButton->font();
    histBackButtonFont.setPointSize(histBackButtonFont.pointSize()*1.5);
    histBackButton->setFont(histBackButtonFont);
    histBackButton->setText(tr("◄"));

    // Napis "History" medzi tlacidlami
    QLabel *histLabel = new QLabel(tr("History"));
    QFont histLabelFont = histLabel->font();
    //histLabelFont.setWeight(QFont::Bold);
    histLabelFont.setPointSize(histLabelFont.pointSize()*2);
    histLabelFont.setFamily("Rockwell");
    histLabel->setFont(histLabelFont);
    histLabel->setAlignment(Qt::AlignCenter);

    // Tlacidlo pre posun vpred v historii hry
    QPushButton *histForwButton = new QPushButton;
    QFont histForwButtonFont = histForwButton->font();
    histForwButtonFont.setPointSize(histForwButtonFont.pointSize()*1.5);
    histForwButton->setFont(histForwButtonFont);
    histForwButton->setText(tr("►"));

    // Spojenie tlacidiel s obsluznymi metodami
    connect(histBackButton, SIGNAL(clicked(bool)), this, SLOT(histBack()));
    connect(histForwButton, SIGNAL(clicked(bool)), this, SLOT(histForw()));
    connect(saveGameButton, SIGNAL(clicked(bool)), this, SLOT(saveGame()));

    // Zvacsenie okna
    this->setFixedSize(width()*2.25, height()*1.75);

    // Mriezkove rozlozenie okna, kam sa ulozi vykreslovacia plocha a elementy UI
    QGridLayout *gameLayout = new QGridLayout;
    gameLayout->addWidget(boardArea, 0, 0, 7, 7);
    gameLayout->addWidget(histBackButton, 7, 0, 1, 1);
    gameLayout->addWidget(histLabel, 7, 1, 1, 1);
    gameLayout->addWidget(histForwButton, 7, 2, 1, 1);
    gameLayout->addWidget(saveGameButton, 7, 5, 1, 2);
    gameInitialized = true;
    setLayout(gameLayout);
}


/**
 * @brief Ulozenie hry
 * Vyvola "Save file" dialog, kde si uzivatel moze vybrat umiestnenie a nazov suboru, do ktoreho sa hra ulozi. Ak nie je mozne ulozit hru do pozadovaneho suboru, tak sa opat ponukne specifikovanie suboru do ktoreho sa ma hra ulozit.
 */
void GuiWindow::saveGame()
{
    if(boardArea->game.isEnd())
        exit(0);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Game..."), "", tr("Othello Save Game Files (*.osf)")); // ;;All Files(*.*)
    if(fileName.isNull())
        return;

    // Aktualne to dookola ponuka ulozenie hry kym sa ju nepodari ulozit, alebo uzivatel neda Cancel
    if(!fileName.endsWith(".osf"))
      fileName += ".osf";
    if(boardArea->game.saveGame(fileName))
        saveGame();
}


/**
 * @brief Navrat spat v historii
 * Vrati hraciu plochu o tah hraca spat v historii (preskakuje tahy AI)
 */
void GuiWindow::histBack()
{
    if(boardArea->game.isEnd())
        exit(0);
    do boardArea->game.setState(boardArea->game.history.prevState());
    while (boardArea->game.onTurnAI() == AI);
    boardArea->repaint();
}


/**
 * @brief Posun vpred v historii
 * Posunie hraciu plochu o tah hraca vpred v historii (preskakuje tahy AI)
 */
void GuiWindow::histForw()
{
    if(boardArea->game.isEnd())
        exit(0);
    do boardArea->game.setState(boardArea->game.history.nextState());
    while (boardArea->game.onTurnAI() == AI);
    boardArea->repaint();
}


/**
 * @brief Udalost kliknutia na UI
 * Okno reaguje na kliknutie mysi specifickym sposobom, ak nastal koniec hry, cim sa okno zavrie.
 */
void GuiWindow::mousePressEvent(QMouseEvent *)
{
    if(!gameInitialized)
        return;
    if(boardArea->game.isEnd())
        exit(0);
}
