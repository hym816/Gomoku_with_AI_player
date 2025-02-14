#ifndef AIFIRST_H
#define AIFIRST_H

#include <QWidget>
#include <QtWidgets>
#include <QLabel>
#include <QPushButton>
#include <random>
#include <QApplication>
#include <QWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include <random>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <QDebug>





class aifirst : public QWidget
{
public:
    aifirst(QWidget *parent = nullptr) : QWidget(parent)
    {
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
                chessvalue[i][j] = 0;
        }
        step = 0;
        setFixedSize(600, 750);
        resetBoard();
        // 创建菜单栏
        QMenuBar *menuBar = new QMenuBar(this);
        menuBar->setGeometry(0, 600, width(), 40); // 设置菜单栏的位置和大小

        // 创建菜单
        QMenu *fileMenu = menuBar->addMenu("Versus");

        // 创建动作（菜单项）
        QAction *newGameAction = new QAction("New Game", this);
        QAction *exitAction = new QAction("Exit", this);

        // 将动作添加到菜单中
        fileMenu->addAction(newGameAction);
        fileMenu->addAction(exitAction);

        // 连接动作的信号与槽函数
        connect(newGameAction, &QAction::triggered, this, &aifirst::resetBoard);
        connect(exitAction, &QAction::triggered, this, &aifirst::close);

        resetBoard();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 绘制棋盘
        drawBoard(painter);

        // 绘制棋子
        drawPieces(painter);
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        if (gameOver || currentPlayer == aiPlayer)
            return;

        if (event->button() == Qt::LeftButton)
        {
            int row = event->y() / cellSize;
            int col = event->x() / cellSize;
            if (isValidMove(row, col))
            {
                board[row][col] = currentPlayer;
                currentPlayer = (currentPlayer == WhitePiece) ? BlackPiece : WhitePiece;
                update();

                if (checkWin(row, col))
                {
                    gameOver = true;
                    QString winner = (currentPlayer == WhitePiece) ? "白方" : "黑方";
                    int result = QMessageBox::question(this, "游戏结束", winner + "获胜！\n是否再来一局？",
                                                       QMessageBox::Yes | QMessageBox::No);

                    if (result == QMessageBox::Yes)
                        resetBoard();
                }
                else if (currentPlayer == aiPlayer)
                {
                    aiMakeMove(); // 让AI自动下棋
                }
            }
        }
    }





private:
    static const int BoardSize = 15;
    static const int CellPadding = 20;
    static const int CellSize = 40;
    enum { EmptyPiece, BlackPiece, WhitePiece  };

    int board[BoardSize][BoardSize];
    int currentPlayer = WhitePiece;
    int cellSize = CellSize;
    bool gameOver = false;
    int aiPlayer = BlackPiece;

    int chessvalue[15][15] = {0};

    int step = 0;

    int row, col;

    int getsteps() {
        return step;
    }


    void resetBoard()
    {
        qDebug() << "RESET";
        step = 0;
        for (int i = 0; i < BoardSize; ++i)
            for (int j = 0; j < BoardSize; ++j)
                board[i][j] = EmptyPiece;

        currentPlayer = WhitePiece;
        gameOver = false;
        aiMakeMove();
        update();
    }

    void drawBoard(QPainter &painter)
    {
        painter.setPen(Qt::black);

        for (int i = 0; i < BoardSize; ++i)
        {
            painter.drawLine(CellPadding, CellPadding + i * cellSize,
                             CellPadding + (BoardSize - 1) * cellSize, CellPadding + i * cellSize);
            painter.drawLine(CellPadding + i * cellSize, CellPadding,
                             CellPadding + i * cellSize, CellPadding + (BoardSize - 1) * cellSize);
        }
    }

    void drawPieces(QPainter &painter)
    {
 qDebug() << "drawpieces called";
        for (int i = 0; i < BoardSize; ++i)
        {
            for (int j = 0; j < BoardSize; ++j)
            {
                if (step == 0|| step == 1) {board[6][6] = 0;}
                if (board[i][j] == BlackPiece){
                    drawPiece(painter, i, j, Qt::black);
                    qDebug() << i << " " << j;
                }

                else if (board[i][j] == WhitePiece)
                    drawPiece(painter, i, j, Qt::white);
                else continue;
            }
        }
    }

    void drawPiece(QPainter &painter, int row, int col, const QColor &color)
    {
        painter.setBrush(color);
        painter.drawEllipse(CellPadding + col * cellSize - cellSize / 2,
                            CellPadding + row * cellSize - cellSize / 2,
                            cellSize, cellSize);
    }

    bool isValidMove(int row, int col)
    {
        return row >= 0 && row < BoardSize && col >= 0 && col < BoardSize && board[row][col] == EmptyPiece;
    }

    bool checkWin(int row, int col)
    {
        step ++;
        {

            {
                // 横向判断
                int count = 1;
                for (int i = col + 1; i < BoardSize && board[row][i] == board[row][col]; ++i)
                    ++count;
                for (int i = col - 1; i >= 0 && board[row][i] == board[row][col]; --i)
                    ++count;
                if (count >= 5)
                    return true;

                // 纵向判断
                count = 1;
                for (int i = row + 1; i < BoardSize && board[i][col] == board[row][col]; ++i)
                    ++count;
                for (int i = row - 1; i >= 0 && board[i][col] == board[row][col]; --i)
                    ++count;
                if (count >= 5)
                    return true;

                // 主对角线判断
                count = 1;
                for (int i = row + 1, j = col + 1; i < BoardSize && j < BoardSize && board[i][j] == board[row][col]; ++i, ++j)
                    ++count;
                for (int i = row - 1, j = col - 1; i >= 0 && j >= 0 && board[i][j] == board[row][col]; --i, --j)
                    ++count;
                if (count >= 5)
                    return true;

                // 副对角线判断
                count = 1;
                for (int i = row + 1, j = col - 1; i < BoardSize && j >= 0 && board[i][j] == board[row][col]; ++i, --j)
                    ++count;
                for (int i = row - 1, j = col + 1; i >= 0 && j < BoardSize && board[i][j] == board[row][col]; --i, ++j)
                    ++count;
                if (count >= 5)
                    return true;

                return false;
            }
        }
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////












    //以下是困难难度


    bool hfive()
    {

        for (int i = 0; i <= 10; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
                    return true;
                if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
                    return true;
                if (board[i][j] == 1 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1)
                    return true;
                if (board[i][j + 4] == 1 && board[i + 1][j + 3] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 1] == 1 && board[i + 4][j] == 1)
                    return true;
            }
        }
        for (int i = 11; i < 15; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
                    return true;
            }
        }
        for (int i = 0; i <= 10; i++)
        {
            for (int j = 11; j < 15; j++)
            {
                if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
                    return true;
            }
        }
        return false;
    }


    int hlivefour()//4子连一起且两边空的
    {
        int hlf = 0;//计分
        int counterhlf = 0;//记录这个棋型出现了几次
        for (int i = 0; i <= 9; i++)
        {
            for (int j = 0; j <= 9; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                {
                    hlf += 5000;//分值这么高，是因为出现活4必胜
                    counterhlf++;
                }
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                {
                    hlf += 5000;
                    counterhlf++;
                }
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 0)
                {
                    hlf += 5000;
                    counterhlf++;
                }
                if (board[i][j + 5] == 0 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 1 && board[i + 3][j + 2] == 1 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 0)
                {
                    hlf += 5000;
                    counterhlf++;
                }
            }
        }
        for (int i = 10; i < 15; i++)
        {
            for (int j = 0; j <= 9; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                {
                    hlf += 5000;
                    counterhlf++;
                }
            }
        }
        for (int i = 0; i <= 9; i++)
        {
            for (int j = 10; j < 15; j++)
            {
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                {
                    hlf += 5000;
                    counterhlf++;
                }
            }
        }
        if (counterhlf >= 2)//因为一旦出现双活四，是必胜，所以需要额外加分，提高优先级
            hlf += 10000;
        return hlf;
    }

    int hhalffour()
    {
        int hhf = 0;
        int counterhhf = 0;
        for (int i = 0; i <= 9; i++)
        {
            for (int j = 0; j <= 9; j++)
            {
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                {
                    hhf += 190;
                    counterhhf++;
                }
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 2)
                {
                    hhf += 190;
                    counterhhf++;
                }

                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                {
                    hhf += 190;
                    counterhhf++;
                }
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 2)
                {
                    hhf += 190;
                    counterhhf++;
                }

                if (board[i][j] == 2 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 0)
                {
                    hhf += 190;
                    counterhhf++;
                }
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 2)
                {
                    hhf += 190;
                    counterhhf++;
                }

                if (board[i][j + 5] == 2 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 1 && board[i + 3][j + 2] == 1 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 0)
                {
                    hhf += 190;
                    counterhhf++;
                }
                if (board[i][j + 5] == 0 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 1 && board[i + 3][j + 2] == 1 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 2)
                {
                    hhf += 190;
                    counterhhf++;
                }
            }
        }
        for (int i = 10; i < 15; i++)
        {
            for (int j = 0; j <= 9; j++)
            {
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                {
                    hhf += 190;
                    counterhhf++;
                }
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 2)
                {
                    hhf += 190;
                    counterhhf++;
                }

            }
        }
        for (int i = 0; i <= 9; i++)
        {
            for (int j = 10; j < 15; j++)
            {
                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                {
                    hhf += 190;
                    counterhhf++;
                }
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 2)
                {
                    hhf += 190;
                    counterhhf++;
                }
            }
        }
        //以下是考虑间隔
        for (int i = 0; i <= 10; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (board[i][j] == 1 && board[i][j + 1] == 0 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
                {
                    hhf += 120;//之所以间隔分数会少于没有间隔的，是因为对方堵中间的间隔后，剩下的棋型不太好。还有个人在测试中发现两个有间隔的半3（一个有间隔的半3是65分）比一个有间隔的半4好，所以定120分
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }

                if (board[i][j] == 1 && board[i + 1][j] == 0 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }

                if (board[i][j] == 1 && board[i + 1][j + 1] == 0 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 0 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }

                if (board[i][j + 4] == 1 && board[i + 1][j + 3] == 0 && board[i + 2][j + 2] == 1 && board[i + 3][j + 1] == 1 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j + 4] == 1 && board[i + 1][j + 3] == 1 && board[i + 2][j + 2] == 0 && board[i + 3][j + 1] == 1 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j + 4] == 1 && board[i + 1][j + 3] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 1] == 0 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
            }
        }

        for (int i = 11; i < 15; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (board[i][j] == 1 && board[i][j + 1] == 0 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
            }
        }


        for (int i = 0; i <= 10; i++)
        {
            for (int j = 11; j < 15; j++)
            {
                if (board[i][j] == 1 && board[i + 1][j] == 0 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
                if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1)
                {
                    hhf += 120;
                    counterhhf++;
                }
            }
        }
        //以下是考虑有一侧是边界无法落子
        for (int i = 0; i < 15; i++)
        {
            int j = 0;
            if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 0)
            {
                hhf += 190;
                counterhhf++;
            }
            j = 10;
            if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
            {
                hhf += 190;
                counterhhf++;
            }
        }
        for (int j = 0; j < 15; j++)
        {
            int i = 0;
            if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 0)
            {
                hhf += 190;
                counterhhf++;
            }
            i = 10;
            if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
            {
                hhf += 190;
                counterhhf++;
            }
        }
        if (counterhhf >= 2)//一旦出现双半4，也是必胜的，所以提高优先级
            hhf += 1500;
        return hhf;
    }

    int hlivethree()
    {
        int hlt = 0;
        int counterhlt = 0;
        for (int i = 0; i <= 10; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 0)
                {
                    hlt += 99;//之所以是99，是因为个人感觉四个活2（一个活2是25分）比一个活3好
                    counterhlt++;
                }
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 0)
                {
                    hlt += 99;
                    counterhlt++;
                }
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 0)
                {
                    hlt += 99;
                    counterhlt++;
                }
                if (board[i][j + 4] == 0 && board[i + 1][j + 3] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 1] == 1 && board[i + 4][j] == 0)
                {
                    hlt += 99;
                    counterhlt++;
                }
            }
        }

        for (int i = 11; i < 15; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 0)
                {
                    hlt += 99;
                    counterhlt++;
                }
            }
        }
        for (int j = 11; j < 15; j++)
        {
            for (int i = 0; i <= 10; i++)
            {
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 0)
                {
                    hlt += 99;
                    counterhlt++;
                }
            }
        }

        //以下是考虑间隔
        for (int i = 0; i <= 9; i++)
        {
            for (int j = 0; j <= 9; j++)
            {

                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }

                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }

                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 0 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }

                if (board[i][j + 5] == 0 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 0 && board[i + 3][j + 2] == 1 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
                if (board[i][j + 5] == 0 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 1 && board[i + 3][j + 2] == 0 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
            }
        }
        for (int i = 10; i < 15; i++)
        {
            for (int j = 0; j <= 9; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
            }
        }
        for (int j = 10; j < 15; j++)
        {
            for (int i = 0; i <= 9; i++)
            {
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                {
                    hlt += 65;
                    counterhlt++;
                }
            }
        }
        if (counterhlt >= 2)//出现双活3也是几乎必胜的，所以也要提高优先级
            hlt += 1000;
        return hlt;
    }

    int hhalfthree()
    {
        int hht = 0;
        for (int i = 0; i <= 9; i++)
        {
            for (int j = 0; j <= 9; j++)
            {
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 0&&board[i][j+5]==0)
                    hht += 50;
                if (board[i][j] == 0 && board[i][j + 1] == 0 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1&&board[i][j + 5] == 2)
                    hht += 50;

                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 0&&board[i+5][j]==0)
                    hht += 50;
                if (board[i][j] == 0 && board[i + 1][j] == 0 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 2)
                    hht += 50;

                if (board[i][j] == 2 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 0&&board[i+5][j+5]==0)
                    hht += 50;
                if (board[i][j] == 0 && board[i + 1][j + 1] == 0 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 2)
                    hht += 50;

                if (board[i][j + 5] == 2 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 1 && board[i + 3][j + 2] == 1 && board[i + 4][j+1] == 0&&board[i+5][j]==0)
                    hht += 50;
                if (board[i][j + 5] == 0 && board[i + 1][j + 4] == 0 && board[i + 2][j + 3] == 1 && board[i + 3][j + 2] == 1 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 2)
                    hht += 50;

                //以下是考虑间隔
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                    hht += 35;
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1 && board[i][j + 5] == 2)
                    hht += 35;
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 2)
                    hht += 35;
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                    hht += 35;

                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                    hht += 35;
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 2)
                    hht += 35;
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1 && board[i + 5][j] == 2)
                    hht += 35;
                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                    hht += 35;

                if (board[i][j] == 2 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 0 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 0)
                    hht += 35;
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 0 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 2)
                    hht += 35;
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 2)
                    hht += 35;
                if (board[i][j] == 2 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 1 && board[i + 5][j + 5] == 0)
                    hht += 35;

                if (board[i][j + 5] == 2 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 0 && board[i + 3][j + 2] == 1 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 0)
                    hht += 35;
                if (board[i][j + 5] == 0 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 0 && board[i + 3][j + 2] == 1 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 2)
                    hht += 35;
                if (board[i][j + 5] == 0 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 1 && board[i + 3][j + 2] == 0 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 2)
                    hht += 35;
                if (board[i][j + 5] == 2 && board[i + 1][j + 4] == 1 && board[i + 2][j + 3] == 1 && board[i + 3][j + 2] == 0 && board[i + 4][j + 1] == 1 && board[i + 5][j] == 0)
                    hht += 35;
            }
        }


        for (int i = 10; i < 15; i++)
        {
            for (int j = 0; j <= 9; j++)
            {
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                    hht += 35;
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1 && board[i][j + 5] == 2)
                    hht += 35;
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 2)
                    hht += 35;
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 0)
                    hht += 35;

                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 0 && board[i][j + 5] == 0)
                    hht += 50;
                if (board[i][j] == 0 && board[i][j + 1] == 0 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1 && board[i][j + 5] == 2)
                    hht += 50;
            }
        }
        for (int j = 10; j < 15; j++)
        {
            for (int i = 0; i <= 9; i++)
            {
                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                    hht += 35;
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 2)
                    hht += 35;
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1 && board[i + 5][j] == 2)
                    hht += 35;
                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1 && board[i + 5][j] == 0)
                    hht += 35;

                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 0 && board[i + 5][j] == 0)
                    hht += 50;
                if (board[i][j] == 0 && board[i + 1][j] == 0 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1 && board[i + 5][j] == 2)
                    hht += 50;
            }
        }
        for (int i = 0; i < 15; i++)
        {
            int j = 0;
            if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0&&board[i][j + 4] == 0)
                hht += 50;
            if (board[i][j] == 1 && board[i][j + 1] == 0 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 0)
                hht += 35;
            if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 0)
                hht += 35;

            j = 10;
            if (board[i][j] == 0 && board[i][j + 1] == 0 && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
                hht += 50;

            if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && board[i][j + 4] == 1)
                hht += 35;
            if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 1)
                hht += 35;
        }

        for (int j = 0; j < 15; j++)
        {
            int i = 0;
            if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 0)
                hht += 50;
            if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 0)
                hht += 35;
            if (board[i][j] == 1 && board[i + 1][j] == 0 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 0)
                hht += 35;

            i = 10;
            if (board[i][j] == 0 && board[i + 1][j] == 0 && board[i + 2][j] == 1 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
                hht += 50;

            if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 1)
                hht += 35;
            if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0 && board[i + 4][j] == 1)
                hht += 35;
        }
        return hht;
    }

    int hlivetwo()
    {
        int hltw = 0;
        for (int i = 0; i <= 11; i++)
        {
            for (int j = 0; j <= 11; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0)
                    hltw += 25;
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0)
                    hltw += 25;
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 0)
                    hltw += 25;
                if (board[i][j + 3] == 0 && board[i + 1][j + 2] == 1 && board[i + 2][j + 1] == 1 && board[i + 3][j] == 0)
                    hltw += 25;
            }
        }
        for (int i = 0; i <= 10; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 0)
                    hltw += 20;
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 0)
                    hltw += 20;
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 0 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 0)
                    hltw += 20;
                if (board[i][j + 4] == 0 && board[i + 1][j + 3] == 1 && board[i + 2][j + 2] == 0 && board[i + 3][j + 1] == 1 && board[i + 4][j] == 0)
                    hltw += 20;
            }
        }
        for (int i = 12; i < 15; i++)
        {
            for (int j = 0; j <= 11; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0)
                    hltw += 25;
            }
        }
        for (int j = 12; j < 15; j++)
        {
            for (int i = 0; i <= 11; i++)
            {
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0)
                    hltw += 25;
            }
        }
        for (int i = 11; i < 15; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && board[i][j + 3] == 1 && board[i][j + 4] == 0)
                    hltw += 20;
            }
        }
        for (int j = 11; j < 15; j++)
        {
            for (int i = 0; i <= 10; i++)
            {
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 1 && board[i + 4][j] == 0)
                    hltw += 20;
            }
        }
        return hltw;
    }

    int hhalftwo()
    {
        int hhtw = 0;
        for (int i = 0; i <= 11; i++)
        {
            for (int j = 0; j <= 11; j++)
            {
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0)
                    hhtw += 10;
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 2)
                    hhtw += 10;

                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0)
                    hhtw += 10;
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 2)
                    hhtw += 10;

                if (board[i][j] == 2 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 0)
                    hhtw += 10;
                if (board[i][j] == 0 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 2)
                    hhtw += 10;

                if (board[i][j + 3] == 2 && board[i + 1][j + 2] == 1 && board[i + 2][j + 1] == 1 && board[i + 3][j] == 0)
                    hhtw += 10;
                if (board[i][j + 3] == 0 && board[i + 1][j + 2] == 1 && board[i + 2][j + 1] == 1 && board[i + 3][j] == 2)
                    hhtw += 10;
            }
        }
        for (int i = 12; i < 15; i++)
        {
            for (int j = 0; j <= 11; j++)
            {
                if (board[i][j] == 2 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0)
                    hhtw += 10;
                if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 2)
                    hhtw += 10;
            }
        }
        for (int j = 12; j < 15; j++)
        {
            for (int i = 0; i < 15; i++)
            {
                if (board[i][j] == 2 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0)
                    hhtw += 10;
                if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 2)
                    hhtw += 10;
            }
        }
        for (int i = 0; i < 15; i++)
        {
            int j = 0;
            if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 0)
                hhtw += 10;

            j = 12;
            if (board[i][j] == 0 && board[i][j + 1] == 1 && board[i][j + 2] == 1)
                hhtw += 10;
        }

        for (int j = 0; j < 15; j++)
        {
            int i = 0;
            if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 0)
                hhtw += 10;

            i = 12;
            if (board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1)
                hhtw += 10;
        }
        return hhtw;
    }
    void exchange()
    {
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (board[i][j] == 2)
                {
                    board[i][j] = 3;
                }
            }
        }
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (board[i][j] == 1)
                {
                    board[i][j] = 2;
                }

            }
        }
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (board[i][j] == 3)
                {
                    board[i][j] = 1;
                }
            }
        }
    }

    void calculate()
    {
        int m;
        //首先对自己的棋型赋分
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = 1;//先假设走这里
                    if (hfive())//判断是否能成5
                    {
                        for (int i = 0; i < 15; i++)
                        {
                            for (int j = 0; j < 15; j++)
                            {
                                chessvalue[i][j] = 0;
                            }
                        }


                        QPainter painter(this);
                        drawPiece(painter, i, j, Qt::black);
                        qDebug() << "line 1067";
                        return;
                    }
                    else//不能成5再计算分值
                    {
                        chessvalue[i][j] = hlivefour() + (hhalffour() + hlivethree() + hhalfthree() + hlivetwo() + hhalftwo())*2;//除了活4，其它棋型分值翻倍,这是为了增强攻击性
                        if ((hhalffour() + hlivethree()) >= 185)//出现一个活3一个半4也是必胜的，所以提高优先级
                            chessvalue[i][j] += 1200;
                        board[i][j] = 0;
                    }
                }
            }
        }

        //以下是判断对方棋型
        exchange();
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = 1;//假设对方走这里
                    if (hfive())
                    {
                        board[i][j] = 0;
                        exchange();
                        board[i][j] = 1;
                        for (int i = 0; i < 15; i++)
                        {
                            for (int j = 0; j < 15; j++)
                            {
                                chessvalue[i][j] = 0;
                            }
                        }


                        QPainter painter(this);
                        drawPiece(painter, i, j, Qt::black);
                        qDebug() << "line 1106";
                        return;
                    }
                    else
                    {
                        chessvalue[i][j] += hlivefour() + hhalffour() + hlivethree() + hhalfthree() + hlivetwo() + hhalftwo();
                        if ((hhalffour() + hlivethree()) >= 185)
                            chessvalue[i][j] += 1200;
                        board[i][j] = 0;
                    }
                }
            }
        }
        exchange();
        m = chessvalue[0][0];
        int x = 0, y = 0;
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (chessvalue[i][j] > m)
                {
                    m = chessvalue[i][j];
                    x = i;
                    y = j;
                    row = x;
                    col = y;
                }
            }
        }//找到两次分值相加最高的点
        if (step != 0 && step != 1 && step != 2) {
            qDebug() << "Calculate called";
        board[x][y] = 1;
        QPainter painter(this);
        drawPiece(painter, x, y, Qt::black);
        qDebug() << "line 1140";
        }

        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                chessvalue[i][j] = 0;
            }
        }
    }












    /////////////////////////////////////////////////////////////////////////////////////////////////////



    void aiMakeMove()
    {
        if (gameOver)
            return;
        qDebug() << "STEP1 = " << step;
        if (step == 0 || step == 1) {
            board[7][7] = aiPlayer;
            board[6][6] = 0;
            currentPlayer = (currentPlayer == WhitePiece) ? BlackPiece : WhitePiece;
            QPainter painter(this);
            drawPiece(painter, 7, 7, Qt::black);
            drawPieces(painter);
            qDebug() << "line 1176";
            //return;
        } //else {

            calculate();
            board[row][col] = aiPlayer;
            currentPlayer = (currentPlayer == WhitePiece) ? BlackPiece : WhitePiece;

            update();

            if (checkWin(row, col))
            {
                gameOver = true;
                QString winner = (currentPlayer == BlackPiece) ? "白方" : "黑方";
                int result = QMessageBox::question(this, "游戏结束", winner + "获胜！\n是否再来一局？",
                                                   QMessageBox::Yes | QMessageBox::No);

                if (result == QMessageBox::Yes)
                    resetBoard();
            }
        //}
        qDebug() << step;
    }







};


#endif // AIFIRST_H
