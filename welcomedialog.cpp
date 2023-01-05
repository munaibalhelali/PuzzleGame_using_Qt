#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "puzzlegame.h"

#include <QApplication>
#include <QDebug>

WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);
    ui->numbersRadioButton->setChecked(true);

    availableSizes << QSize(3,3) << QSize(4,4) << QSize(5,5) << QSize(6,6) << QSize(7,7) << QSize(8,8) << QSize(9,9) << QSize(10,10);
    availableSizesText << "3x3" << "4x4" << "5x5" << "6x6" << "7x7" << "8x8" << "9x9" << "10x10";
    ui->boardSizeComboBox->addItems(availableSizesText);

    availableImages << "luffy.png" << "nature.jpeg" << "lion.jpeg";
    ui->imageComboBox->addItems(availableImages);

    ui->imageComboBox->setEnabled(ui->imageRadioButton->isChecked());
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_confirmButton_clicked()
{
    int boardSizeIndex = ui->boardSizeComboBox->currentIndex();
    boardSize = availableSizes[boardSizeIndex];
    if(ui->imageRadioButton->isChecked())
        imageMode = true;
    else if(ui->numbersRadioButton->isChecked())
        imageMode = false;
    imageName = ui->imageComboBox->currentText();

    PuzzleGame* puzzleGame = new PuzzleGame(this);
    puzzleGame->setImageMode(imageMode);
    puzzleGame->setImageName(imageName);
    puzzleGame->setBoardSize(boardSize);
    puzzleGame->setupUi();
    puzzleGame->exec();

}

void WelcomeDialog::on_cancelButton_clicked()
{
    QApplication::quit();
}

QString WelcomeDialog::getImageName() const
{
    return imageName;
}

QSize WelcomeDialog::getBoardSize() const
{
    return boardSize;
}

bool WelcomeDialog::getImageMode()
{
    return imageMode;
}



void WelcomeDialog::on_numbersRadioButton_toggled(bool checked)
{
    ui->imageComboBox->setEnabled(false);
}

void WelcomeDialog::on_imageRadioButton_toggled(bool checked)
{
    ui->imageComboBox->setEnabled(true);
}
