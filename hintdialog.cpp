#include "hintdialog.h"
#include "ui_hintdialog.h"

HintDialog::HintDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HintDialog)
{
    ui->setupUi(this);
}

HintDialog::HintDialog(QWidget *parent, QString imageName) :
    QDialog(parent),
    ui(new Ui::HintDialog)
{
    ui->setupUi(this);
    showHint(imageName);
}

HintDialog::~HintDialog()
{
    delete ui;
}

void HintDialog::showHint(QString imageName)
{
    QPixmap pixmap(":/background/images/"+imageName);
    pixmap = pixmap.scaled(QSize(500, 600));
    ui->hintLabel->setPixmap(pixmap);
}

void HintDialog::on_okButton_clicked()
{
    accept();
}
