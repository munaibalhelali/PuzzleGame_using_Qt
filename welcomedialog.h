#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QWidget>
#include <QSize>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

    QSize getBoardSize() const;
    bool getImageMode();

    QString getImageName() const;

private slots:
    void on_confirmButton_clicked();

    void on_cancelButton_clicked();



    void on_numbersRadioButton_toggled(bool checked);

    void on_imageRadioButton_toggled(bool checked);

private:
    Ui::WelcomeDialog *ui;
    QSize boardSize;
    QString imageName;
    bool imageMode;
    QList<QSize> availableSizes;
    QStringList availableSizesText;
    QStringList availableImages;

};

#endif // WELCOMEDIALOG_H
