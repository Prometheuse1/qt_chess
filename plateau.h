#ifndef PLATEAU_H
#define PLATEAU_H
#include <vector>

using namespace std;

struct predict_move{
	int ld,cd,la,ca,poid;
};

class plateau
{
    friend class ChessBoard;
	public:
		plateau();
		~plateau();
		int roi_en_echec(int couleur);
		int a_mouvement_legal(int couleur);
		void init_Plateau();
		int get_Tour();
		void scanner_Plateau_IA();
		void deplacer_IA();
		void evalMovIA();
		int fin_Partie();
		int get_Piece(int i,int j);
		void mov_Piece(int ld,int cd,int la,int ca);
		int check_arival_space(int ld,int cd,int la,int ca);
		int evaluation(int ld,int cd,int la,int ca);
		int eval_pion(int ld,int cd,int la,int ca);
		int eval_cavalier(int ld,int cd,int la,int ca);
		int eval_tour(int ld,int cd,int la,int ca);
		int eval_fou(int ld,int cd,int la,int ca);
		int eval_dame(int ld,int cd,int la,int ca);
		int eval_roi(int ld,int cd,int la,int ca);
		
	private:
		int ech[8][8];
		int tour;
		predict_move t[1000];
		int cp;
};

#endif
