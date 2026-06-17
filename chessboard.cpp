#include "chessboard.h"
#include <QTimer>
#include <cmath>

MenuDialog::MenuDialog(QWidget *parent):QDialog(parent)
{
    setWindowTitle("Jeu d'Échecs");
    setFixedSize(460,240);
    setStyleSheet("background:#1a1a2e;color:#e0e0e0;");

    auto *vl=new QVBoxLayout(this);
    vl->setSpacing(16);
    vl->setContentsMargins(30,30,30,30);

    auto *title=new QLabel("JEU D'ÉCHECS",this);
    title->setAlignment(Qt::AlignCenter);
    QFont tf("Georgia",18,QFont::Bold);
    title->setFont(tf);
    title->setStyleSheet("color:#f0c040;letter-spacing:2px;");
    vl->addWidget(title);

    auto *sub=new QLabel("Choisissez un mode de jeu",this);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color:#aaa;font-size:12px;");
    vl->addWidget(sub);

    auto *hl=new QHBoxLayout();

    auto btnStyle=[](const QString &accent){
        return QString(
                   "QPushButton {"
                   "  background:%1;"
                   "  color:#fff;"
                   "  border:none;"
                   "  border-radius:6px;"
                   "  padding:10px 20px;"
                   "  font-size:13px;"
                   "  font-weight:bold;"
                   "}"
                   "QPushButton:hover { background:%1; opacity:0.85; }"
                   ).arg(accent);
    };

    auto *btn1=new QPushButton("Joueur vs Joueur",this);
    btn1->setStyleSheet(btnStyle("#3a7bd5"));
    auto *btn2=new QPushButton("Joueur vs IA",this);
    btn2->setStyleSheet(btnStyle("#2d9c5e"));

    hl->addWidget(btn1);
    hl->addWidget(btn2);
    vl->addLayout(hl);

    connect(btn1, &QPushButton::clicked,this,[this]{m_choix=1;accept();});
    connect(btn2, &QPushButton::clicked,this,[this]{m_choix=2;accept();});
}

ChessBoard::ChessBoard(int gameMode,QWidget *parent)
    : QWidget(parent),
    m_mode(gameMode),
    m_selected(false),
    m_selRow(-1),m_selCol(-1),
    m_gameOver(false)
{
    setFixedSize(MARGIN+CELL*8+MARGIN,MARGIN+CELL*8+MARGIN+50);
    setWindowTitle("Jeu d'Échecs");
    setStyleSheet("background:#1a1a2e;");
    m_statusMsg="Tour des Blancs";
}

QPoint ChessBoard::cellToPixel(int row,int col)const
{
    return {MARGIN+col*CELL,MARGIN+row*CELL};
}

void ChessBoard::pixelToCell(QPoint pt,int &row,int &col)const
{
    col=(pt.x()-MARGIN)/CELL;
    row=(pt.y()-MARGIN)/CELL;
}

QString ChessBoard::pieceText(int val) const
{
    switch(val){
        case  1: return "♙";
        case  2: return "♖";
        case  3: return "♘";
        case  4: return "♗";
        case  5: return "♕";
        case  6: return "♔";
        case -1: return "♟";
        case -2: return "♜";
        case -3: return "♞";
        case -4: return "♝";
        case -5: return "♛";
        case -6: return "♚";
        default: return "";
    }
}

QColor ChessBoard::pieceColor(int val)const
{
    return (val>0)?QColor(255,255,240):QColor(30,30,30);
}

void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    drawBoard(p);
    drawHighlight(p);
    drawPieces(p);
    drawAxisLabels(p);
    drawStatus(p);
}

void ChessBoard::drawBoard(QPainter &p)
{
    for (int r=0;r<8;r++){
        for (int c= 0;c<8;c++){
            QColor light("#c8a97e");
            QColor dark("#6b3f1f");
            QColor fill=((r+c)%2==0)?light:dark;
            QPoint tl=cellToPixel(r,c);
            p.fillRect(tl.x(),tl.y(),CELL,CELL,fill);
        }
    }
}

void ChessBoard::drawHighlight(QPainter &p)
{
    if (!m_selected) return;

    QPoint tl=cellToPixel(m_selRow, m_selCol);
    p.fillRect(tl.x(), tl.y(), CELL, CELL, QColor(255, 220, 50, 160));
    int couleur=(m_plateau.get_Tour()%2==0)?1:-1;

    // Show legal destinations
    p.setBrush(QColor(80, 200, 120, 255));
    p.setPen(Qt::NoPen);
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            // We use a const_cast trick: evaluation is logically const
            plateau &ref=const_cast<plateau&>(m_plateau);
            if(ref.evaluation(m_selRow,m_selCol,r,c)==1){
                int saved=m_plateau.ech[r][c];
                int piece=m_plateau.ech[m_selRow][m_selCol];
                m_plateau.ech[r][c]=piece;
                m_plateau.ech[m_selRow][m_selCol]=0;

                bool stillInCheck=(m_plateau.roi_en_echec(couleur)==1);

                m_plateau.ech[m_selRow][m_selCol]=piece;
                m_plateau.ech[r][c]=saved;

                if (stillInCheck) continue;

                QPoint dl=cellToPixel(r,c);

                if (saved!=0)  // there's an enemy piece to capture
                {
                    p.fillRect(dl.x(), dl.y(), CELL, CELL, QColor(200, 40, 40, 180));
                }
                else
                {
                    p.drawEllipse(dl.x()+CELL/2-12,dl.y()+CELL/2-12,24,24);
                }
            }
        }
    }
}

void ChessBoard::drawPieces(QPainter &p)
{
    QFont f("Segoe UI Symbol", 38);
    QPoint tl;
    p.setFont(f);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int val = m_plateau.get_Piece(r, c);
            if (val==0) continue;

            tl=cellToPixel(r,c);
            QRect cell(tl.x(),tl.y(),CELL,CELL);

            // Shadow
            p.setPen(QColor(0,0,0,100));
            p.drawText(cell.adjusted(2,2,2,2),Qt::AlignCenter,pieceText(val));

            // Piece
            p.setPen(pieceColor(val));
            p.drawText(cell, Qt::AlignCenter, pieceText(val));
        }
    }
}

void ChessBoard::drawAxisLabels(QPainter &p)
{
    QFont f("Consolas",11,QFont::Bold);
    p.setFont(f);
    p.setPen(QColor(180,150,100));

    const char cols[]="abcdefgh";
    for(int c=0;c<8;c++){
        int x=MARGIN+c*CELL+CELL/2-6;
        p.drawText(x,MARGIN-6,QString(cols[c]));
        p.drawText(x,MARGIN+8*CELL+18,QString(cols[c]));
    }
    for(int r=0;r<8;r++){
        int y=MARGIN+r*CELL+CELL/2+5;
        p.drawText(6,y,QString::number(8-r));
        p.drawText(MARGIN+8*CELL+6,y,QString::number(8-r));
    }
}

void ChessBoard::drawStatus(QPainter &p)
{
    int y=MARGIN+8*CELL+30;
    QFont f("Georgia",13,QFont::Bold);
    p.setFont(f);
    p.setPen(QColor(240,200,60));
    p.drawText(QRect(0,y,width(),36),Qt::AlignCenter,m_statusMsg);
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    int row,col,piece;
    if(m_gameOver) return;
    if(m_mode==2 && m_plateau.get_Tour()%2!=0) return;


    pixelToCell(event->pos(),row,col);
    if(row<0 || row>7 || col<0 || col>7) return;

    piece=m_plateau.get_Piece(row, col);

    if(!m_selected){
        if(piece==0) return;
        if(m_plateau.get_Tour()%2==0 && piece<0) return;
        if(m_plateau.get_Tour()%2==1 && piece>0) return;
        m_selected=true;
        m_selRow=row;
        m_selCol=col;
    }

    else{
        if(row==m_selRow && col==m_selCol){
            m_selected=false;
            update();
            return;
        }

        if(piece!=0){
            bool sameColor=(m_plateau.get_Tour()%2==0 && piece>0) || (m_plateau.get_Tour()%2==1 && piece<0);
            if(sameColor){
                m_selRow=row;
                m_selCol=col;
                update();
                return;
            }
        }

        m_plateau.mov_Piece(m_selRow,m_selCol,row,col);
        m_selected=false;
        afterMove();
    }
    update();
}

void ChessBoard::afterMove()
{
    checkFinPartie();

    if(!m_gameOver && m_mode==2 && m_plateau.get_Tour()%2!=0){
        m_statusMsg="L'IA réfléchit…";
        update();
        QTimer::singleShot(1000,this,[this](){
            m_plateau.deplacer_IA();
            checkFinPartie();
            update();
        });
    }
}

void ChessBoard::checkFinPartie()
{
    int couleur=(m_plateau.get_Tour()%2==0)?1:-1;
    bool enEchec=(m_plateau.roi_en_echec(couleur)==1);
    bool aLegal=(m_plateau.a_mouvement_legal(couleur)==1);

    if(enEchec && !aLegal){
        QString winner=(couleur==1)?"Noirs":"Blancs";
        m_statusMsg=QString("Échec et mat! Les %1 gagnent!").arg(winner);
        m_gameOver=true;
        update();
        QMessageBox msg(this);
        msg.setWindowTitle("Fin de partie");
        msg.setText(QString("Échec et mat!\nLes %1 gagnent!").arg(winner));
        msg.setStyleSheet(
            "QMessageBox {background:#1a1a2e;}"
            "QLabel      {color:white; font-size:16px; font-family:Georgia;}"
            "QPushButton {background:#3a7bd5; color:white; border-radius:4px;"
            "padding:6px 20px; font-size:13px;}"
            );
        msg.setFixedSize(600,300);
        msg.exec();
        return;
    }

    if(!enEchec && !aLegal){
        m_statusMsg="Pat! Match nul!";
        m_gameOver=true;
        update();
        QMessageBox msg(this);
        msg.setWindowTitle("Fin de partie");
        msg.setText("Pat! Match nul!");
        msg.setStyleSheet(
            "QMessageBox {background:#1a1a2e;}"
            "QLabel      {color:white; font-size:16px; font-family:Georgia;}"
            "QPushButton {background:#3a7bd5; color:white; border-radius:4px;"
            "padding:6px 20px;font-size:13px;}"
            );
        msg.setFixedSize(600, 300);
        msg.exec();
        return;
    }

    if(enEchec){
        m_statusMsg=QString("Échec au Roi %1! À vous de jouer.").arg((m_plateau.get_Tour()%2==0)?"Blanc":"Noir");
    }
    else{
        m_statusMsg=QString("Tour des %1").arg((m_plateau.get_Tour()%2==0)?"Blancs":"Noirs");
    }
    update();
}
