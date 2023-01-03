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
