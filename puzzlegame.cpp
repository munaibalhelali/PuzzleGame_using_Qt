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
#include <QImage>
#include <QDir>

void removeItems ( QLayout* layout );

QImage createSubImage(QImage* image, const QRect & rect);

PuzzleGame::PuzzleGame(QWidget *parent)
    : QWidget(parent), ui(new Ui::PuzzleGame)
{
    ui->setupUi(this);
    imageMode = false;
    promoteWelcomeDialog();

    emptySymbol = QString("");
    this->setFixedSize(500, 600);
    image.load(":/background/images/img_1.png");
    image = image.scaled(this->size(), Qt::IgnoreAspectRatio);

    mainVLayout = ui->gameVerticalLayout;
    createBoard();

    //connect startOverButton
    connect(ui->startOverButton, &QPushButton::clicked, this, &PuzzleGame::startOver);
    bestScore = 1000000;
    if(imageMode)
        editImage();


}

PuzzleGame::~PuzzleGame()
{
}

void PuzzleGame::printButtonText(int row, int column)
{
    //qDebug()<<"Button text: "<< buttons[row][column]->text();
    QString temp = buttons[row][column]->text();
    buttons[row][column]->setText(buttons[row][column+1]->text());
    buttons[row][column+1]->setText(temp);
}

void PuzzleGame::play(int row, int column)
{
    QPushButton* emptyButton = checkForNullButton(row, column);
    //qDebug()<<"Mode: "<< imageMode;
    QPushButton* button = buttons[row][column];
    if(emptyButton != nullptr && !imageMode){
        QString temp = button->text();
        button->setText(emptyButton->text());
        emptyButton->setText(temp);
        QString count = ui->movesNumberLabel->text();
        ui->movesNumberLabel->setText(QString::number(count.toInt()+1));

    }else if(emptyButton != nullptr && imageMode){
        QString tempName = button->accessibleName();
        QIcon tempIcon = button->icon();

        button->setAccessibleName(emptyButton->accessibleName());
        button->setIcon(emptyButton->icon());

        emptyButton->setAccessibleName(tempName);
        emptyButton->setIcon(tempIcon);

        QString count = ui->movesNumberLabel->text();
        ui->movesNumberLabel->setText(QString::number(count.toInt()+1));
    }else{
        //qDebug()<<"neither hhhhh";
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
    if(imageMode){
        std::vector<int> randomContent = getRandomeBoard();
        for(auto row: buttons)
            for(auto button: row){
                if(randomContent[counter] != 0)
                    button->setAccessibleName(QString::number(randomContent[counter]));
                else
                    button->setAccessibleName(emptySymbol);

                counter++;
            }
        palette.setColor(QPalette::Window, Qt::white);
        this->setPalette(palette);
        editImage();
    }else{
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



}

void PuzzleGame::promoteWelcomeDialog()
{
    WelcomeDialog welcomeDialog;
    auto ret = welcomeDialog.exec();
    if(ret == QDialog::Accepted){
        boardW = welcomeDialog.getBoardSize().width();
        boardH = welcomeDialog.getBoardSize().height();
        imageMode = welcomeDialog.getImageMode();
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
    //qDebug()<<"in createBoard";
    QVBoxLayout* buttonVlayout = new QVBoxLayout();

    std::vector<int> buttonText = getRandomeBoard();

    for(int i = 0; i < boardH; i++){
        QHBoxLayout* hlayout = new QHBoxLayout();
        std::vector<QPushButton*> rowButtons;
        for(int j = 0; j < boardW; j++){
            QPushButton* button = new QPushButton(this);
            button->setFixedSize(buttonW, buttonH);
            QString buttonName = QString::number(buttonText[i*boardW+j]);
            if(buttonName != QString::number(0)){
                if(!imageMode)
                    button->setText(buttonName);
                else
                    button->setAccessibleName(buttonName);
            }else{
                button->setText(QString(""));
                button->setAccessibleName(QString(""));
            }
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
            if(!imageMode){
                if(buttons[rowIdxs[i]][columnIdxs[i]]->text() == emptySymbol){
                    return buttons[rowIdxs[i]][columnIdxs[i]];
                }
            }else{
                //qDebug()<<"accessibleName: "<<buttons[rowIdxs[i]][columnIdxs[i]]->accessibleName() ;
                if(buttons[rowIdxs[i]][columnIdxs[i]]->accessibleName() == emptySymbol){
                    return buttons[rowIdxs[i]][columnIdxs[i]];
                }
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
            if(imageMode ){
                if(button->accessibleName() != reference[counter]){
                    return false;
                }
            }else{
                if(button->text() != reference[counter]){
                return false;
                }
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

void PuzzleGame::editImage()
{

    int x = 0;
    int y = 0;
    int w = image.width()/boardW;
    int h = image.height()/boardH;
    QDir currDir = QDir::current();
    if (currDir.cd("subImages")){
        currDir.removeRecursively();
        currDir.cdUp();
        currDir.mkdir("subImages");
    }else{
        currDir.mkdir("subImages");
    }
    //qDebug()<<currDir.absolutePath();
    QList<QList<QImage>> subImages;
    int counter = 1;
    for(int hi = 0; hi < boardH; hi++){
        QList<QImage> rowImages;
        x = 0;
        for(int wi = 0; wi < boardW; wi++){
            QImage subImg = createSubImage(&image, QRect(x, y, w, h));
            rowImages.append(subImg);
            subImg.save("subImages/luffy_"+QString::number(counter)+".png");
            //qDebug() << subImg.width()<<", "<< subImg.height();
            x += w;
            counter++;
        }
        y += h;
        subImages.append(rowImages);
    }
    //qDebug()<<"subImages: "<<subImages.size()<<", "<<subImages[0].size();
    for(int row = 0; row < buttons.size(); row++)
        for(int column = 0; column < buttons[0].size(); column++){
            QPushButton* button = buttons[row][column];
            QString imgPath = "subImages/luffy_"+button->accessibleName()+".png";
            QPixmap pixmap;
            if(button->accessibleName() == "")
                pixmap.load(":/background/images/white_background.png");
            else
                pixmap.load(imgPath);
            //qDebug()<<"Pixmap size: "<<pixmap.size();
            QIcon icon(pixmap);
            button->setIcon(icon);
            button->setIconSize(pixmap.size());
        }
}

QImage createSubImage(QImage* image, const QRect & rect) {
    size_t offset = rect.x() * image->depth() / 8
                    + rect.y() * image->bytesPerLine();
    return QImage(image->bits() + offset, rect.width(), rect.height(),
                  image->bytesPerLine(), image->format());
}

