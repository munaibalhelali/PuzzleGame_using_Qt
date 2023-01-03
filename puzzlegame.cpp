#include "puzzlegame.h"
#include <QLayout>
#include <QRect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <opencv4/opencv2/opencv.hpp>
#include <ui_puzzlegame.h>
#include <QApplication>
#include <welcomedialog.h>
#include <QLayoutItem>

void removeItems ( QLayout* layout );

PuzzleGame::PuzzleGame(QWidget *parent)
    : QWidget(parent), ui(new Ui::PuzzleGame)
{
    ui->setupUi(this);

    promoteWelcomeDialog();

    emptySymbol = QString("");
    this->setFixedSize(500, 600);
    mainVLayout = ui->gameVerticalLayout;
    createBoard();

    //connect startOverButton
    connect(ui->startOverButton, &QPushButton::clicked, this, &PuzzleGame::startOver);
    bestScore = 1000000;


}

PuzzleGame::~PuzzleGame()
{
}

void PuzzleGame::printButtonText(int row, int column)
{
    qDebug()<<"Button text: "<< buttons[row][column]->text();
    QString temp = buttons[row][column]->text();
    buttons[row][column]->setText(buttons[row][column+1]->text());
    buttons[row][column+1]->setText(temp);
}

void PuzzleGame::play(int row, int column)
{
    QPushButton* emptyButton = checkForNullButton(row, column);
    if(emptyButton != nullptr){
        QString temp = buttons[row][column]->text();
        buttons[row][column]->setText(emptyButton->text());
        emptyButton->setText(temp);
        QString count = ui->movesNumberLabel->text();
        ui->movesNumberLabel->setText(QString::number(count.toInt()+1));
    }
    if(hasWon()){
        palette.setColor(QPalette::Window, Qt::green);
        this->setPalette(palette);
        QMessageBox msgBox;

        msgBox.setText("Congratulations! You won.");
        msgBox.exec();

    }
}

void PuzzleGame::startOver()
{
    int score = ui->movesNumberLabel->text().toInt();
    bestScore = (bestScore > score) ? score : bestScore;
    ui->bestScoreLabel->setText(QString::number(bestScore));
    ui->movesNumberLabel->setText(QString::number(0));

    int counter = 0;
    std::vector<int> randomContent = getRandomeBoard();
    for(auto row: buttons)
        for(auto button: row){
            if(randomContent[counter] != 0)
                button->setText(QString::number(randomContent[counter]));
            else
                button->setText(emptySymbol);
            counter++;
        }
    palette.setColor(QPalette::Window, Qt::white);
    this->setPalette(palette);



}

void PuzzleGame::promoteWelcomeDialog()
{
    WelcomeDialog welcomeDialog;
    auto ret = welcomeDialog.exec();
    if(ret == QDialog::Accepted){
        boardW = welcomeDialog.getBoardSize().width();
        boardH = welcomeDialog.getBoardSize().height();
    }else{
        boardW = 3;
        boardH = 3;
    }

}


std::vector<int> PuzzleGame::getRandomeBoard(){

        std::vector<int> tempArr;
        int upper = boardW * boardH, lower = 0;
        std::srand ((unsigned)time(nullptr));
        rand();
        int counter=0;
        int arraySize = boardW * boardH;
        while(counter < arraySize){
            int tempStorage= (rand()%(upper-lower))+lower;
            bool duplicate=false;
            for(int y: tempArr){
                if(y==tempStorage){
                    duplicate=true;
                }
            }
            if(!duplicate){
                tempArr.push_back(tempStorage);
                counter++;
            }

        }

        return tempArr;
}

void PuzzleGame::createBoard()
{
    int buttonW = this->width()/boardW;
    int buttonH = buttonW;
    qDebug()<<"in createBoard";
    QVBoxLayout* buttonVlayout = new QVBoxLayout();

    std::vector<int> buttonText = getRandomeBoard();

    for(int i = 0; i < boardH; i++){
        QHBoxLayout* hlayout = new QHBoxLayout();
        std::vector<QPushButton*> rowButtons;
        for(int j = 0; j < boardW; j++){
            QPushButton* button = new QPushButton(this);
            button->setFixedSize(buttonW, buttonH);
            QString buttonName = QString::number(i*boardW+j);
            button->setAccessibleName(buttonName);
            if(buttonText[i*boardW+j] != 0)
                button->setText(QString::number(buttonText[i*boardW+j]));
            else
                button->setText(QString(""));
            connect(button, &QPushButton::clicked, [=]()->void{play(i, j);});
//            button->setStyleSheet("border-image:url(:/background/images/img_1.png)");
            hlayout->addWidget(button);
            rowButtons.push_back(button);
        }
        buttonVlayout->addItem(hlayout);
        buttons.push_back(rowButtons);
    }
    mainVLayout->addItem(buttonVlayout);

}

QPushButton* PuzzleGame::checkForNullButton(int row, int column)
{
    std::vector<int> rowIdxs{row, row, row+1, row-1};
    std::vector<int> columnIdxs{column+1, column-1, column, column};
    for(int i = 0; i< 4; i++){
        if(rowIdxs[i] < boardH && rowIdxs[i] >= 0 && columnIdxs[i] < boardW && columnIdxs[i] >= 0){
            if(buttons[rowIdxs[i]][columnIdxs[i]]->text() == emptySymbol){
                return buttons[rowIdxs[i]][columnIdxs[i]];
            }
        }
    }
    return nullptr;

}

bool PuzzleGame::hasWon()
{
    std::vector<QString> reference;
    for(int i=1; i<boardH*boardW; i++){
        reference.push_back(QString::number(i));
    }
    reference.push_back(emptySymbol);

    int counter = 0;
    for(auto row: buttons){
        for(auto button: row){
            if(button->text() != reference[counter]){
                return false;
            }
            counter++;
        }
    }
    return true;

}

void removeItems ( QLayout* layout )
{
    QLayoutItem* child;
    while ( layout->count() != 0 ) {
        child = layout->takeAt ( 0 );
        if ( child->layout() != 0 ) {
            removeItems ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }

        delete child;
    }
}
