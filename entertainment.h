#ifndef ENTERTAINMENT_H
#define ENTERTAINMENT_H



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

#include <QWidget>

#include "settings.h"






class GomokuBoard_2 : public QWidget
{
public:
    int lastindex1 = 10000;    int lastindex2 = 10000;
    int winCnt = 5;
    GomokuBoard_2(QWidget *parent = nullptr) : QWidget(parent)
    {
        setFixedSize(600, 750);
        resetBoard();
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &GomokuBoard_2::handleTimeout);



        progressBar = new QProgressBar(this);
        progressBar->setGeometry(20, height() - 20, width() - 40, 20);
        progressBar->setRange(0, 30);
        progressBar->setFormat("%v 秒"); // 设置显示格式为秒数
        progressBar->setValue(countdown);

        // 创建一个小的颜色块，用于显示应该下的棋子颜色
        currentPlayerColorBlock = new QFrame(this);
        currentPlayerColorBlock->setGeometry(20, height() - 40, 20, 20); // 在进度条的左侧

        // 初始化颜色块的颜色为黑色（黑方先下）
        currentPlayerColorBlock->setStyleSheet("background-color: black;");


     //   // 创建一个小的颜色块，用于显示应该下的棋子颜色
     //   currentPositionBlock = new QFrame(this);
     //   currentPositionBlock->setGeometry(10000, 10000, 10, 10); // 在进度条的左侧
     //
     //   // 初始化颜色块的颜色为黑色（黑方先下）
     //   currentPositionBlock->setStyleSheet("background-color: red;");


        //谁下
        turn = new QLabel(this);
        turn->setText("黑方先下棋");
        QGridLayout a1;
        a1.addWidget(turn);
        turn->setGeometry(50,705,100,30);



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
        connect(newGameAction, &QAction::triggered, this, &GomokuBoard_2::resetBoard);
        connect(exitAction, &QAction::triggered, this, &GomokuBoard_2::close);


    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        drawBoard(painter);
        drawPieces(painter);

        if (hoveredRow >= 0 && hoveredCol >= 0)
        {
            painter.setBrush(Qt::white);
            painter.drawEllipse(CellPadding + hoveredCol * cellSize - 3,
                                CellPadding + hoveredRow * cellSize - 3,
                                6, 6);
        }
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        if (gameOver)
            return;

        if (event->button() == Qt::LeftButton)
        {
            int row = event->y() / cellSize;
            int col = event->x() / cellSize;

            step++;

            if (isValidMove(row, col))
            {
                timer->stop();  // 停止之前的倒计时
                timer->start(1000);  // 重新启动倒计时
                lastMoveRow = row;
                lastMoveCol = col;
                countdown = 30;  // 重置倒计时时间
                progressBar->setValue(countdown);  // 重置进度条

                board[row][col] = currentPlayer;
                currentPlayer = (currentPlayer == BlackPiece) ? WhitePiece : BlackPiece;
                update();

                if (checkWin(row, col))
                {
                    timer->stop();  // 停止倒计时
                    gameOver = true;
                    QString winner = (currentPlayer == BlackPiece) ? "白方" : "黑方";
                    int result = QMessageBox::question(this, "游戏结束", winner + "获胜！\n是否再来一局？",
                                                       QMessageBox::Yes | QMessageBox::No);

                    if (result == QMessageBox::Yes)
                        resetBoard();
                }
            }
        }
    }


private:
    static const int BoardSize = 15;
    static const int CellPadding = 20;
    static const int CellSize = 40;
    enum { EmptyPiece, BlackPiece, WhitePiece };

    int board[BoardSize][BoardSize];
    int currentPlayer = BlackPiece;
    int cellSize = CellSize;
    bool gameOver = false;

    void resetBoard()
    {
        for (int i = 0; i < BoardSize; ++i)
            for (int j = 0; j < BoardSize; ++j)
                board[i][j] = EmptyPiece;

        currentPlayer = BlackPiece;
        gameOver = false;
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
        for (int i = 0; i < BoardSize; ++i)
        {
            for (int j = 0; j < BoardSize; ++j)
            {
                if (board[i][j] == BlackPiece)
                    drawPiece(painter, i, j, Qt::black);
                else if (board[i][j] == WhitePiece)
                    drawPiece(painter, i, j, Qt::white);
            }
        }

        // 绘制上一步棋的红点标注
        if (lastMoveRow >= 0 && lastMoveCol >= 0)
        {
            painter.setBrush(Qt::red);
            painter.drawEllipse(CellPadding + lastMoveCol * cellSize - 5,
                                CellPadding + lastMoveRow * cellSize - 5,
                                10, 10);
        }
    }


    void drawPiece(QPainter &painter, int row, int col, const QColor &color)
    {
//        if (step % 2 == 0) {
//            painter.setBrush(Qt::black);
//            painter.drawEllipse(lastindex1, lastindex2, cellSize - 5, cellSize - 5);

//        } else if (step % 2 == 1) {
//            painter.setBrush(Qt::white);
//            painter.drawEllipse(lastindex1, lastindex2, cellSize - 5, cellSize - 5);
//        }
//      qDebug() << step++;


        painter.setBrush(color);
        painter.drawEllipse(CellPadding + col * cellSize - cellSize / 2 + 2.5,
                            CellPadding + row * cellSize - cellSize / 2 + 2.5,
                            cellSize - 5, cellSize - 5);

        lastindex1 = CellPadding + col * cellSize - cellSize / 2 + 2.5;
        lastindex2 = CellPadding + row * cellSize - cellSize / 2 + 2.5;
        // 创建一个小的颜色块，用于显示应该下的棋子颜色
//        painter.setBrush(Qt::red);
//        painter.drawEllipse(CellPadding + col * cellSize - cellSize / 2 + 15,
//                            CellPadding + row * cellSize - cellSize / 2 + 15,
//                            cellSize - 30, cellSize - 30);



    }
///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool isValidMove(int row, int col)
    {
        return row >= 0 && row < BoardSize && col >= 0 && col < BoardSize && board[row][col] == EmptyPiece;
    }

    bool checkWin(int row, int col)
    {

        {
            // 横向判断
            int count = 1;
            for (int i = col + 1; i < BoardSize && board[row][i] == board[row][col]; ++i)
                ++count;
            for (int i = col - 1; i >= 0 && board[row][i] == board[row][col]; --i)
                ++count;
            if (count >= winCnt)
                return true;

            // 纵向判断
            count = 1;
            for (int i = row + 1; i < BoardSize && board[i][col] == board[row][col]; ++i)
                ++count;
            for (int i = row - 1; i >= 0 && board[i][col] == board[row][col]; --i)
                ++count;
            if (count >= winCnt)
                return true;

            // 主对角线判断
            count = 1;
            for (int i = row + 1, j = col + 1; i < BoardSize && j < BoardSize && board[i][j] == board[row][col]; ++i, ++j)
                ++count;
            for (int i = row - 1, j = col - 1; i >= 0 && j >= 0 && board[i][j] == board[row][col]; --i, --j)
                ++count;
            if (count >= winCnt)
                return true;

            // 副对角线判断
            count = 1;
            for (int i = row + 1, j = col - 1; i < BoardSize && j >= 0 && board[i][j] == board[row][col]; ++i, --j)
                ++count;
            for (int i = row - 1, j = col + 1; i >= 0 && j < BoardSize && board[i][j] == board[row][col]; --i, ++j)
                ++count;
            if (count >= winCnt)
                return true;

            return false;
        }
    }


    void mouseMoveEvent(QMouseEvent *event) override
    {
        int mouseX = event->x();
        int mouseY = event->y();

        int closestRow = qRound((mouseY - CellPadding) / static_cast<double>(cellSize));
        int closestCol = qRound((mouseX - CellPadding) / static_cast<double>(cellSize));

        if (isValidMove(closestRow, closestCol))
        {
            setToolTip("点击以下棋"); // 设置提示
        }
        else
        {
            setToolTip(""); // 清空提示
        }
    }



private:
    QTimer *timer;
    int countdown = 30;  // 初始倒计时时间，单位：秒
    QProgressBar *progressBar;
    QFrame *currentPlayerColorBlock;
    QLabel *turn;
    QFrame *currentPositionBlock;
    int step = 0;
    int lastMoveRow = -1;
    int lastMoveCol = -1;
    int hoveredRow = -1;
    int hoveredCol = -1;

private slots:
    void handleTimeout()
    {
        countdown--;
        progressBar->setValue(countdown);

        if (countdown <= 0)
        {
            timer->stop();
            gameOver = true;
            QString winner = (currentPlayer == BlackPiece) ? "白方" : "黑方";
            int result = QMessageBox::question(this, "游戏结束", winner + "获胜！\n是否再来一局？",
                                               QMessageBox::Yes | QMessageBox::No);

            if (result == QMessageBox::Yes)
                resetBoard();
        }

        // 更新颜色块的颜色来指示哪个玩家应该下棋
        currentPlayerColorBlock->setStyleSheet(
            (currentPlayer == BlackPiece) ? "background-color: black;" : "background-color: white;");
        turn->setText(
            (currentPlayer == BlackPiece) ? "轮到黑棋了" : "轮到白棋了");
    }

};


#endif // ENTERTAINMENT_H
