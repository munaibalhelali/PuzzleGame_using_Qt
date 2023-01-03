#ifndef PUZZLEGAME_H
#define PUZZLEGAME_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class PuzzleGame; }
QT_END_NAMESPACE

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
    void promoteWelcomeDialog();

private:
    Ui::PuzzleGame * ui;
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

    int bestScore;
};
#endif // PUZZLEGAME_H
