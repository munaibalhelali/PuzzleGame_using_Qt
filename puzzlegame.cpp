#include "puzzlegame.h"
#include "hintdialog.h"
#include <welcomedialog.h>

#include <ui_puzzlegame.h>

#include <QLayout>
#include <QRect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QLayoutItem>
#include <QImage>
#include <QDir>


void removeItems ( QLayout* layout );

QImage createSubImage(QImage* image, const QRect & rect);

PuzzleGame::PuzzleGame(QWidget *parent)
    : QDialog(parent), ui(new Ui::PuzzleGame)
{
    ui->setupUi(this);

    imageMode = false;
    imageName = "luffy.png";
    boardH = 3;
    boardW = 3;

    emptySymbol = "";
    emptyButtonIconName = "empty_button_chess.png";

    this->setFixedSize(800, 900);

    mainVLayout = ui->gameVerticalLayout;

    //connect startOverButton
    connect(ui->startOverButton, &QPushButton::clicked, this, &PuzzleGame::startOver);
    bestScore = 1000000;

    //disable hint in text mode
    ui->hintButton->setEnabled(imageMode);


}

PuzzleGame::~PuzzleGame()
{
}

void PuzzleGame::setupUi()
{
    createBoard();
    if(imageMode){
        image.load(":/background/images/"+imageName);
        image = image.scaled(this->size(), Qt::IgnoreAspectRatio);
        editImage();
    }
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
    auto ret = QMessageBox::question(this, "Restart game?", "Are you sure?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if(ret == QMessageBox::No){
        return;
    }
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

    std::vector<int> buttonText = getRandomeBoard();
//    std::vector<int> buttonText= {1, 2, 3, 4, 5, 6, 7, 0, 8};

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

            //connect each button to the play slot
            connect(button, &QPushButton::clicked, [=]()->void{play(i, j);});

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


    int counter = 1;
    for(int hi = 0; hi < boardH; hi++){
        x = 0;
        for(int wi = 0; wi < boardW; wi++){
            QImage subImg = createSubImage(&image, QRect(x, y, w, h));
            subImg.save("subImages/sub_"+QString::number(counter)+".png");
            x += w;
            counter++;
        }
        y += h;
    }

    for(int row = 0; row < buttons.size(); row++)
        for(int column = 0; column < buttons[0].size(); column++){
            QPushButton* button = buttons[row][column];
            QString imgPath = "subImages/sub_"+button->accessibleName()+".png";
            QPixmap pixmap;
            if(button->accessibleName() == ""){
                pixmap.load(":/background/images/"+emptyButtonIconName);
                pixmap = pixmap.scaled(button->width(), button->height());
            }else
                pixmap.load(imgPath);

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


void PuzzleGame::on_hintButton_clicked()
{
    HintDialog* hint = new HintDialog(this, imageName);
    hint->exec();
}

bool PuzzleGame::getImageMode() const
{
    return imageMode;
}

void PuzzleGame::setImageMode(bool value)
{
    imageMode = value;
    ui->hintButton->setEnabled(imageMode);

}

void PuzzleGame::setBoardSize(QSize boardSize)
{
    boardW = boardSize.width();
    boardH = boardSize.height();
}

QString PuzzleGame::getImageName() const
{
    return imageName;
}

void PuzzleGame::setImageName(const QString &value)
{
    imageName = value;
}

void PuzzleGame::on_closeButton_clicked()
{
   int ret = QMessageBox::warning(this, "End game", "Are you sure?", QMessageBox::Yes, QMessageBox::Cancel);
   if(ret == QMessageBox::Yes){
       accept();
   }
}
