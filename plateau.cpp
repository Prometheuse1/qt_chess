#include "plateau.h"
#include "math.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

plateau::plateau()
{
    init_Plateau();
	cp=0;
	tour=0;
}

plateau::~plateau()
{
	
}

int plateau::roi_en_echec(int couleur)
{
	int ki=-1,kj=-1;
	int roi=couleur*6;
 
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(ech[i][j]==roi) 
			{ 
				ki=i; 
				kj=j; 
			}
		}	
	}
	
	if(ki==-1) return 1;
 
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(couleur==1 && ech[i][j]<0)
			{
				if(evaluation(i,j,ki,kj)==1) return 1;
			}
			else if(couleur==-1 && ech[i][j]>0)
			{
				if(evaluation(i,j,ki,kj)==1) return 1;
			}
		}
	}
	return 0;
}

int plateau::a_mouvement_legal(int couleur)
{
	int sauvegarde,piece,toujours_en_echec;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(couleur==1 && ech[i][j]<=0) continue;
			if(couleur==-1 && ech[i][j]>=0) continue;
 
			for(int k=0;k<8;k++)
			{
				for(int l=0;l<8;l++)
				{
					if(evaluation(i,j,k,l)==1)
					{
						// Simulate the move
						sauvegarde=ech[k][l];
						piece=ech[i][j];
						ech[k][l]=piece;
						ech[i][j]=0;
 
						toujours_en_echec=roi_en_echec(couleur);
 
						// Undo the move
						ech[i][j]=piece;
						ech[k][l]=sauvegarde;
 
						if(toujours_en_echec==0) return 1;
					}
				}
			}
		}
	}
	return 0;
}

int plateau::get_Tour(){
	return tour;
}

void plateau::scanner_Plateau_IA()
{
	int rep=0;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<=7;j++)
		{
			if(ech[i][j]<0)
			{
				for(int k=0;k<=7;k++)
				{
					for(int l=0;l<=7;l++)
					{
						if(ech[k][l]>=0)
						{
							switch(ech[i][j])
							{
								case -1  : rep=eval_pion(i,j,k,l);     break;
								case -2  : rep=eval_tour(i,j,k,l);     break;
								case -3  : rep=eval_cavalier(i,j,k,l); break;
								case -4  : rep=eval_fou(i,j,k,l);      break;
								case -5  : rep=eval_dame(i,j,k,l);     break;
								case -6  : rep=eval_roi(i,j,k,l);      break;
								default  :                             break;
							}
							if(rep==1)
							{
								t[cp].ld=i;	
								t[cp].cd=j;	
								t[cp].la=k;	
								t[cp].ca=l;	
								t[cp].poid=0;	

								cp++;
							}							
						}
					}
				}
			}
		}
	}
}

void plateau::evalMovIA()
{
	int val,cpMp=0;
	predict_move tempcpMp;

	for(int i=0;i<cp;i++)
	{
		switch(ech[t[i].la][t[i].ca])
		{
			case 0 : break;
			case 1 : t[i].poid+=5;    break;
			case 2 : t[i].poid+=15;   break;
			case 3 : t[i].poid+=10;   break;
			case 4 : t[i].poid+=20;   break;
			case 5 : t[i].poid+=50;   break;
			case 6 : t[i].poid+=1000; break; // Impossible Echec et mat !!
			default : break;
		}
	}
	for(int i=0;i<cp;i++)
	{
		for(int j=i+1;j<cp;j++)
		{
			if(t[i].poid < t[j].poid)
			{
				predict_move temp;
				temp=t[i];
				t[i]=t[j];
				t[j]=temp;
			}
		}
	}
	
	// si plusieurs poids similaires (meilleur mouv possible)
	for(int j=0;j<cp;j++)
	{
		if(t[0].poid == t[j].poid)
		{
			cpMp++;
		}
		else
		{
			j=cp;
		}
	}
	//..................
	srand(time(NULL));
	val=rand()%cpMp;
	tempcpMp=t[0];
	t[0]=t[val];
	t[val]=tempcpMp;	
}

void plateau::deplacer_IA()
{
	cp=0;
	scanner_Plateau_IA();

    int valid_cp = 0;
    predict_move valid_t[1000];

    for(int i=0;i<cp;i++)
    {
        int ld=t[i].ld,cd=t[i].cd;
        int la=t[i].la,ca=t[i].ca;

        // Simulate
        int sauvegarde=ech[la][ca];
        int piece=ech[ld][cd];
        ech[la][ca]=piece;
        ech[ld][cd]=0;

        int still_in_check=roi_en_echec(-1);

        ech[ld][cd] = piece;
        ech[la][ca] = sauvegarde;

        if(still_in_check==0)
        {
            valid_t[valid_cp++]=t[i];
        }
    }

    // Replace the move list with only legal moves
    for(int i = 0; i < valid_cp; i++)
    {
        t[i] = valid_t[i];
    }

    cp = valid_cp;
	evalMovIA();
	if(cp==0) return;
	
	ech[t[0].la][t[0].ca]=ech[t[0].ld][t[0].cd];
	ech[t[0].ld][t[0].cd]=0;	
	tour++;		
}

int plateau::check_arival_space(int ld,int cd,int la,int ca)
{
	if((ech[ld][cd]>0 && ech[la][ca]<0) ||	(ech[ld][cd]<0 && ech[la][ca]>0) || ech[la][ca]==0)
	{
		return 1;
	}
	
	return 0;
}

int plateau::eval_pion(int ld,int cd,int la,int ca)
{
	if(ech[ld][cd]==1)
	{

		if(la==ld-1 && ca==cd && ech[la][ca]==0)
            return 1;

        if(ld==6 && la==ld-2 && ca==cd && ech[ld-1][cd]==0 && ech[la][ca]==0)
        	return 1;

        if(la==ld-1 && (ca==cd+1 || ca==cd-1) && ech[la][ca]<0)
        	return 1;
	}
	
	if(ech[ld][cd]==-1)
	{
		if(la==ld+1 && ca==cd && ech[la][ca]==0)
            return 1;

        if(ld==1 && la==ld+2 && ca==cd && ech[ld+1][cd]==0 && ech[la][ca]==0)
        	return 1;
        
        if(la==ld+1 && (ca==cd+1 || ca==cd-1) && ech[la][ca]>0)
        	return 1;
	}
	
	return 0;
}

int plateau::eval_cavalier(int ld,int cd,int la,int ca)
{
	if( ((ld+2==la)&&(cd+1==ca)) || ((ld+2==la)&&(cd-1==ca)) || ((ld+1==la)&&(cd+2==ca)) || ((ld-1==la)&&(cd+2==ca)) 
	|| ((ld-2==la)&&(cd+1==ca)) || ((ld-2==la)&&(cd-1==ca)) || ((ld+1==la)&&(cd-2==ca)) || ((ld-1==la)&&(cd-2==ca)) )
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else
	{
		return 0;
	}
}

int plateau::eval_tour(int ld,int cd,int la,int ca)
{
	bool chemin_vide=false;
	int i;
	if(ld==la)
	{
		chemin_vide=true;
		if(cd<ca)
		{
			i=cd+1;
			while(i<ca && chemin_vide==true)
			{
				if(ech[ld][i]!=0)
				{
					chemin_vide=false;
				}
				i++;
			}
		}
		else if(cd>ca)
		{
			i=cd-1;
			while(i>ca && chemin_vide==true)
			{
				if(ech[ld][i]!=0)
				{
					chemin_vide=false;
				}
				i--;
			}
		}	
	}
	if(cd==ca)
	{
		chemin_vide=true;
		if(ld<la)
		{
			i=ld+1;
			while(i<la && chemin_vide==true)
			{
				if(ech[i][cd]!=0)
				{
					chemin_vide=false;
				}
				i++;
			}
		}
		if(ld>la)
		{
			i=ld-1;
			while(i>la && chemin_vide==true)
			{
				if(ech[i][cd]!=0)
				{
					chemin_vide=false;
				}
				i--;
			}
		}
	}
	
	if(chemin_vide==true)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else 
	{
		return 0;
	}
}

int plateau::eval_fou(int ld,int cd,int la,int ca)
{
    bool chemin_vide=false;
    int i,j;
    if(abs(la-ld)==abs(ca-cd) && ld!=la && cd!=ca )
    {
		chemin_vide=true;
        if(la>ld && ca>cd)
        {  
			i=ld+1;
            j=cd+1;
            while(i<la && chemin_vide==true)
            {
                if(ech[i][j]!=0) 
				{
					chemin_vide=false;
				}
                i++; 
				j++;
            }
        }
        if(la>ld && ca<cd)
        {
            i=ld+1;
            j=cd-1;
            while(i<la && chemin_vide==true)
            {
                if(ech[i][j]!=0)
				{
					chemin_vide=false;
				}
                i++; 
				j--;
            }
        }
        if(la<ld && ca>cd)
        {
            i=ld-1;
            j=cd+1;
            while(i>la && chemin_vide==true)
            {
                if(ech[i][j]!=0) 
				{
					chemin_vide=false;
				}
                i--; 
				j++;
            }
        }
        if(la<ld && ca<cd)
        {
            i=ld-1;
            j=cd-1;
            while(i>la && chemin_vide==true)
            {
                if(ech[i][j]!=0) 
				{
					chemin_vide=false;
				}
                i--; 
				j--;
            }
        }
    }
   	if(chemin_vide==true)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else 
	{
		return 0;
	}
}

int plateau::eval_dame(int ld,int cd,int la,int ca)
{
	if(eval_tour(ld,cd,la,ca)==1)
	{
		return 1;
	}
	if(eval_fou(ld,cd,la,ca)==1)
	{
		return 1;
	}
	return 0;
}

int plateau::eval_roi(int ld,int cd,int la,int ca)
{
	if(abs(la-ld)<2 && abs(ca-cd)<2)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	return 0;
}

int plateau::evaluation(int ld,int cd,int la,int ca)
{
	switch(ech[ld][cd])
	{
		case  1: return eval_pion(ld,cd,la,ca);		break;
		case -1: return eval_pion(ld,cd,la,ca);		break;
		case  2: return eval_tour(ld,cd,la,ca); 	break;
		case -2: return eval_tour(ld,cd,la,ca);		break;
		case  3: return eval_cavalier(ld,cd,la,ca); break;
		case -3: return eval_cavalier(ld,cd,la,ca);	break;
		case  4: return eval_fou(ld,cd,la,ca);		break;
		case -4: return eval_fou(ld,cd,la,ca);		break;
		case  5: return eval_dame(ld,cd,la,ca);		break;
		case -5: return eval_dame(ld,cd,la,ca);		break;
		case  6: return eval_roi(ld,cd,la,ca);		break;
		case -6: return eval_roi(ld,cd,la,ca);		break;
	}
}


void plateau::mov_Piece(int ld,int cd,int la,int ca)
{
	int sauvegarde,piece,couleur;
	if(evaluation(ld,cd,la,ca)==1)
	{
		sauvegarde=ech[la][ca];
        piece=ech[ld][cd];
        ech[la][ca]=piece;
        ech[ld][cd]=0;

        if(tour%2==0)
		{
			couleur=1;
		}
		else
		{
			couleur=-1;
		}
	
        if(roi_en_echec(couleur)==1)
        {
            ech[ld][cd]=piece;
            ech[la][ca]=sauvegarde;
            cout<<"Mouvement invalide, votre roi est en echec !"<<endl;
        }
        else
        {
            tour++;
        }
	}
}


int plateau::get_Piece(int i,int j)
{
	return ech[i][j];
}

void plateau::init_Plateau()
{
    for(int i=2;i<=5;i++)
    {
        for(int j=0;j<=7;j++)
        {
            ech[i][j]=0; // Position Vide
        }
    }

    for(int j=0;j<=7;j++)
    {
        ech[1][j]=-1; // Pion Blanc
        ech[6][j]=1;// Pion Noir
    }

    ech[0][0]=-2; // Tour
    ech[0][1]=-3; // Cavalier
    ech[0][2]=-4; // Fou
    ech[0][3]=-5; // Reine/Dame
    ech[0][4]=-6; // Roi
    ech[0][5]=-4; // Fou
    ech[0][6]=-3; // Cavalier
    ech[0][7]=-2; // Tour

    ech[7][0]=2; // Tour
    ech[7][1]=3; // Cavalier
    ech[7][2]=4; // Fou
    ech[7][3]=5; // Reine/Dame
    ech[7][4]=6; // Roi
    ech[7][5]=4; // Fou
    ech[7][6]=3; // Cavalier
    ech[7][7]=2; // Tour
}
