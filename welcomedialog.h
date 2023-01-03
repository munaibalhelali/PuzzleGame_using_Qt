#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include <QSize>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

    QSize getBoardSize() const;

private slots:
    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::WelcomeDialog *ui;
    QSize boardSize;
};

#endif // WELCOMEDIALOG_H
