#ifndef HINTDIALOG_H
#define HINTDIALOG_H

#include <QDialog>

namespace Ui {
class HintDialog;
}

class HintDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HintDialog(QWidget *parent = nullptr);
    explicit HintDialog(QWidget *parent = nullptr, QString imageName = "");
    ~HintDialog();

    void showHint(QString imageName);

private slots:
    void on_okButton_clicked();

private:
    Ui::HintDialog *ui;
};

#endif // HINTDIALOG_H
