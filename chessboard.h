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

// ──────────────────────────────────────────────
//  Menu dialog shown at startup
// ──────────────────────────────────────────────
class MenuDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MenuDialog(QWidget *parent = nullptr);
    int choix() const { return m_choix; }

private:
    int m_choix = 1;
};

// ──────────────────────────────────────────────
//  The 8×8 board widget
// ──────────────────────────────────────────────
class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(int gameMode, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    // ── game state ──────────────────────────
    plateau  m_plateau;
    int      m_mode;          // 1 = JvJ, 2 = JvIA
    bool     m_selected;      // a piece is selected
    int      m_selRow, m_selCol;
    bool     m_gameOver;
    QString  m_statusMsg;

    // ── geometry ────────────────────────────
    static constexpr int CELL = 80;   // px per square
    static constexpr int MARGIN = 30; // axis labels margin

    QPoint cellToPixel(int row, int col) const;
    void   pixelToCell(QPoint pt, int &row, int &col) const;

    // ── rendering helpers ───────────────────
    void drawBoard(QPainter &p);
    void drawPieces(QPainter &p);
    void drawHighlight(QPainter &p);
    void drawStatus(QPainter &p);
    void drawAxisLabels(QPainter &p);

    QString pieceText(int val) const;   // Unicode chess symbol
    QColor  pieceColor(int val) const;

    // ── game logic helpers ──────────────────
    void afterMove();
    void checkFinPartie();
};

#endif // CHESSBOARD_H
