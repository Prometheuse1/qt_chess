#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include "plateau.h"

class MenuDialog:public QDialog
{
    Q_OBJECT
public:
    explicit MenuDialog(QWidget *parent=nullptr);
    int choix() const {return m_choix;}

private:
    int m_choix=1;
};

class ChessBoard:public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(int gameMode,QWidget *parent=nullptr);

protected:
    void paintEvent(QPaintEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;

private:

    plateau  m_plateau;
    int      m_mode;
    bool     m_selected;
    int      m_selRow, m_selCol;
    bool     m_gameOver;
    QString  m_statusMsg;
    static constexpr int CELL=80;
    static constexpr int MARGIN=30;

    QPoint cellToPixel(int row,int col)const;
    void pixelToCell(QPoint pt,int &row,int &col)const;
    void drawBoard(QPainter &p);
    void drawPieces(QPainter &p);
    void drawHighlight(QPainter &p);
    void drawStatus(QPainter &p);
    void drawAxisLabels(QPainter &p);
    QString pieceText(int val) const;   // Unicode chess symbol
    QColor  pieceColor(int val) const;
    void afterMove();
    void checkFinPartie();
};

#endif
