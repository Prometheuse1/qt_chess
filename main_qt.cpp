#include <QApplication>
#include "chessboard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ── Show mode-selection menu ──────────────
    MenuDialog menu;
    if(menu.exec() != QDialog::Accepted)
        return 0;

    // ── Launch board ──────────────────────────
    ChessBoard board(menu.choix());
    board.show();

    return app.exec();
}
