#include <QApplication>
#include "chessboard.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    MenuDialog menu;

    if(menu.exec()!=QDialog::Accepted)
        return 0;

    ChessBoard board(menu.choix());
    board.show();

    return app.exec();
}
