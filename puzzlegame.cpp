#include "puzzlegame.h"
#include <QLayout>
#include <QRect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <opencv4/opencv2/opencv.hpp>

PuzzleGame::PuzzleGame(QWidget *parent)
    : QWidget(parent)
{
    boardW = 3;
    boardH = 3;
    emptySymbol = QString("");
    this->setFixedSize(500, 600);
    mainVLayout = new QVBoxLayout(this);
    createBoard();

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

    QVBoxLayout* buttonVlayout = new QVBoxLayout();

//    std::vector<int> buttonText = getRandomeBoard();
    std::vector<int> buttonText{1, 2, 3, 4, 5, 6, 7, 0, 8};

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

            QPixmap pixmap;
            pixmap.load("images/apple_logo.png");
            QPalette palette;
            palette.setBrush(QPalette::Window, QBrush(pixmap));
//            button->setFlat(true);
            button->setAutoFillBackground(true);
            button->setPalette(palette);

            hlayout->addWidget(button);
            rowButtons.push_back(button);
        }
        buttonVlayout->addItem(hlayout);
        buttons.push_back(rowButtons);
    }
    QPushButton* startOverButton = new QPushButton(this);
    startOverButton->setText("Start over");
    connect(startOverButton, &QPushButton::clicked, this, &PuzzleGame::startOver);
    mainVLayout->addItem(buttonVlayout);
    mainVLayout->addWidget(startOverButton);
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
