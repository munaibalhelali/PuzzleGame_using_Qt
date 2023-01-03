#include "welcomedialog.h"
#include "ui_welcomedialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_confirmButton_clicked()
{
    int index = ui->boardSizeComboBox->currentIndex();
    QList<QSize> availableSizes;
    availableSizes << QSize(3,3) << QSize(4,4) << QSize(5,5) << QSize(6,6) << QSize(7,7) << QSize(8,8) << QSize(9,9) << QSize(10,10);
    boardSize = availableSizes[index];
    accept();
}

void WelcomeDialog::on_cancelButton_clicked()
{
    reject();
}

QSize WelcomeDialog::getBoardSize() const
{
    return boardSize;
}

