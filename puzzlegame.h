#ifndef PUZZLEGAME_H
#define PUZZLEGAME_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include <QVBoxLayout>

class PuzzleGame : public QWidget
{
    Q_OBJECT

public:
    PuzzleGame(QWidget *parent = nullptr);
    ~PuzzleGame();

private slots:
    void printButtonText(int row, int column);
    void play(int row, int column);
    void startOver(void);
private:
    std::vector<std::vector<QPushButton*>> buttons;
    int boardW;
    int boardH;
    QVBoxLayout * mainVLayout;
    QPalette palette;

    std::vector<int> getRandomeBoard();
    void createBoard(void);
    QPushButton* checkForNullButton(int row, int column);
    bool hasWon(void);
    QString emptySymbol;
};
#endif // PUZZLEGAME_H
