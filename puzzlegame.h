#ifndef PUZZLEGAME_H
#define PUZZLEGAME_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <vector>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class PuzzleGame; }
QT_END_NAMESPACE

class PuzzleGame : public QDialog
{
    Q_OBJECT

public:
    PuzzleGame(QWidget *parent = nullptr);
    ~PuzzleGame();

    void setupUi();

    QString getImageName() const;
    void setImageName(const QString &value);

    bool getImageMode() const;
    void setImageMode(bool value);

    void setBoardSize(QSize boardSize);

private slots:
    void printButtonText(int row, int column);
    void play(int row, int column);
    void startOver(void);

    void on_hintButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::PuzzleGame * ui;
    std::vector<std::vector<QPushButton*>> buttons;
    int boardW;
    int boardH;
    QVBoxLayout * mainVLayout;
    QPalette palette;
    QString emptySymbol;
    QString emptyButtonIconName;
    QImage image;
    QString imageName;
    bool imageMode;
    int bestScore;

    //private functions
    std::vector<int> getRandomeBoard();
    void createBoard(void);
    QPushButton* checkForNullButton(int row, int column);
    bool hasWon(void);
    void editImage();



};
#endif // PUZZLEGAME_H
