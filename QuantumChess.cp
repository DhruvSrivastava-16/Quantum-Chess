#include<iostream>
#include<tuple>
#include<vector>
#include<cstring>
#include<cstdlib>
#include<ctime>

#define SIZE 8

using namespace std;

enum types
{
	king,
	queen,
	lightbishop,
	darkbishop,
	leftknight,
	rightknight,
	leftrook,
	rightrook,
	apawn,
	bpawn,
	cpawn,
	dpawn,
	epawn,
	fpawn,
	gpawn,
	hpawn
};


typedef tuple <int, int> position;

struct piece
{
	position pos;
	char color;
	types type;
	char symbol;
	int promotion;
	double prob;
};

class chess
{
	char turn;	//'w' or 'b'
	piece* board[8][8]{{NULL}};
	vector<piece*> King;
	vector<piece*> Queen;
	vector<piece*> Night;
	vector<piece*> Bishop;
	vector<piece*> Rook;
	vector<piece*> Pawn;
	
	int prom_no=0;
	
	public:
	chess()
	{
		make_king('w',7,4);
        make_king('b',0,4);
        make_queen('w',7,3);
        make_queen('b',0,3);
        
        make_knight('w','l',7,1);
        make_knight('w','r',7,6);
        make_knight('b','l',0,1);
        make_knight('b','r',0,6);
        
        make_bishop('w','d',7,2);
        make_bishop('w','l',7,5);
        make_bishop('b','d',0,2);
        make_bishop('b','l',0,5);
        
        make_rook('w','l',7,0);
        make_rook('w','r',7,7);
        make_rook('b','l',0,0);
        make_rook('b','r',0,7);
        
        for(char x='a';x<='h';x++)
		{
			make_pawn('w',x,6,(x-'a'));
		}
		for(char x='a';x<='h';x++)
		{
			make_pawn('b',x,1,(x-'a'));
		}
		
		turn='w';
	}
	
	void move(string mv)
	{
		if(mv.length()!=6)
		{
			cout<<"invalid"<<endl;
			return;
		}
		position current_pos;
		int err=decode( mv.substr(1,2),current_pos);
		if(err==1)
		{
			cout<<"invalid"<<endl;
			return;
		}
		position next_pos;
		err=decode( mv.substr(3,2),next_pos);
		if(err==1)
		{
			cout<<"invalid"<<endl;
			return;
		}
		piece* p=decode_piece(mv[0],current_pos);
		if(p==NULL)
		{
			cout<<"invalid"<<endl;
			return;
		}
		
		if(mv[5]=='N')
		{
			int v=isvalid(p,current_pos,next_pos);
			if(v==1)
			{
				if(board[get<0>(next_pos)][get<1>(next_pos)]==NULL)
				{	
					if(p->symbol=='P' && (get<0>(next_pos)==0 || get<0>(next_pos)==7) )
					{
						if(p->prob==1)
						{
							promote(p,next_pos);
							for(int i=0;i<Pawn.size();i++)
							{
								if(Pawn[i]->color==p->color && Pawn[i]->type==p->type)
								{
									Pawn.erase(Pawn.begin()+i);
								}
							}
							delete p;
							board[get<0>(current_pos)][get<1>(current_pos)]=NULL;		
						}
						else
						{
							vector<piece*> vec;
							for(int i=0;i<Pawn.size();i++)
							{
								if(Pawn[i]->type==p->type && Pawn[i]->color==p->color)
								{
									vec.push_back(Pawn[i]);
									Pawn.erase(Pawn.begin()+i);
									i--;
								}
							}
							double rans=(1.0*rand())/RAND_MAX;
							double j=0;
							piece* temp;
							for(int i=0;i<vec.size();i++)
							{
								if(rans>=j && rans<=j+vec[i]->prob)
								{
									vec[i]->prob=1;
									break;
								}
								j+=vec[i]->prob;
							}
							for(int i=0;i<vec.size();i++)
							{
								if(vec[i]->prob==1)
								{
									temp=vec[i];
									continue;
								}
								board[get<0>(vec[i]->pos)][get<1>(vec[i]->pos)]=NULL;
								delete vec[i];
							}
							if(temp==p)
							{
								promote(p,next_pos);
								delete p;
								board[get<0>(current_pos)][get<1>(current_pos)]=NULL;
							}
							else
							{
								Pawn.push_back(temp);
							}
							
						}
					}
					else
					{
						get<0>(p->pos)=get<0>(next_pos);
						get<1>(p->pos)=get<1>(next_pos);
						board[get<0>(next_pos)][get<1>(next_pos)]=board[get<0>(current_pos)][get<1>(current_pos)];
						board[get<0>(current_pos)][get<1>(current_pos)]=NULL;
					}
				}
				else if(board[get<0>(next_pos)][get<1>(next_pos)]->prob==1 && board[get<0>(current_pos)][get<1>(current_pos)]->prob==1)
				{
					if(p->symbol=='P' && (get<0>(next_pos)==0 || get<0>(next_pos)==7) )
					{
						char sym=board[get<0>(next_pos)][get<1>(next_pos)]->symbol;
						types t=board[get<0>(next_pos)][get<1>(next_pos)]->type;
						char c=board[get<0>(next_pos)][get<1>(next_pos)]->color;
					
						switch(sym)
						{
					
							case 'K' : 	for(int i=0;i<King.size();i++)
										{
											if(King[i]->type==t && King[i]->color==c)
											{
												King.erase(King.begin()+i);
												break;
											}
										}
										break;
						
							case 'Q' : 	for(int i=0;i<Queen.size();i++)
										{
											if(Queen[i]->type==t && Queen[i]->color==c)
											{
												Queen.erase(Queen.begin()+i);
												break;
											}
										}
										break;
						
							case 'N' : 	for(int i=0;i<Night.size();i++)
										{
											if(Night[i]->type==t && Night[i]->color==c)
											{
												Night.erase(Night.begin()+i);
												break;
											}
										}
										break;
						
							case 'B' : 	for(int i=0;i<Bishop.size();i++)
										{
											if(Bishop[i]->type==t && Bishop[i]->color==c)
											{
												Bishop.erase(Bishop.begin()+i);
												break;
											}
										}
										break;
						
							case 'R' : 	for(int i=0;i<Rook.size();i++)
										{
											if(Rook[i]->type==t && Rook[i]->color==c)
											{
												Rook.erase(Rook.begin()+i);
												break;
											}
										}
										break;
						
							case 'P' : 	for(int i=0;i<Pawn.size();i++)
										{
											if(Pawn[i]->type==t && Pawn[i]->color==c)
											{
												Pawn.erase(Pawn.begin()+i);
												break;
											}
										}
										break;
					
						}
						delete board[get<0>(next_pos)][get<1>(next_pos)];
						promote(p,next_pos);
						for(int i=0;i<Pawn.size();i++)
						{
							if(Pawn[i]->color==p->color && Pawn[i]->type==p->type)
							{
								Pawn.erase(Pawn.begin()+i);
							}
						}
						delete p;
						board[get<0>(current_pos)][get<1>(current_pos)]=NULL;		
					}
					else
					{
						get<0>(p->pos)=get<0>(next_pos);
						get<1>(p->pos)=get<1>(next_pos);
					
						char sym=board[get<0>(next_pos)][get<1>(next_pos)]->symbol;
						types t=board[get<0>(next_pos)][get<1>(next_pos)]->type;
						char c=board[get<0>(next_pos)][get<1>(next_pos)]->color;
					
						switch(sym)
						{
					
							case 'K' : 	for(int i=0;i<King.size();i++)
										{
											if(King[i]->type==t && King[i]->color==c)
											{
												King.erase(King.begin()+i);
												break;
											}
										}
										break;
						
							case 'Q' : 	for(int i=0;i<Queen.size();i++)
										{
											if(Queen[i]->type==t && Queen[i]->color==c)
											{
												Queen.erase(Queen.begin()+i);
												break;
											}
										}
										break;
						
							case 'N' : 	for(int i=0;i<Night.size();i++)
										{
											if(Night[i]->type==t && Night[i]->color==c)
											{
												Night.erase(Night.begin()+i);
												break;
											}
										}
										break;
						
							case 'B' : 	for(int i=0;i<Bishop.size();i++)
										{
											if(Bishop[i]->type==t && Bishop[i]->color==c)
											{
												Bishop.erase(Bishop.begin()+i);
												break;
											}
										}
										break;
						
							case 'R' : 	for(int i=0;i<Rook.size();i++)
										{
											if(Rook[i]->type==t && Rook[i]->color==c)
											{
												Rook.erase(Rook.begin()+i);
												break;
											}
										}
										break;
						
							case 'P' : 	for(int i=0;i<Pawn.size();i++)
										{
											if(Pawn[i]->type==t && Pawn[i]->color==c)
											{
												Pawn.erase(Pawn.begin()+i);
												break;
											}
										}
										break;
					
						}
					
						delete board[get<0>(next_pos)][get<1>(next_pos)];
						board[get<0>(next_pos)][get<1>(next_pos)]=board[get<0>(current_pos)][get<1>(current_pos)];
						board[get<0>(current_pos)][get<1>(current_pos)]=NULL;
					}
				}
				else
				{
					char sym1=board[get<0>(next_pos)][get<1>(next_pos)]->symbol;
					types t1=board[get<0>(next_pos)][get<1>(next_pos)]->type;
					char c1=board[get<0>(next_pos)][get<1>(next_pos)]->color;
					int pp1=board[get<0>(next_pos)][get<1>(next_pos)]->promotion;
					vector<piece*> vec1;
					
					char sym2=board[get<0>(current_pos)][get<1>(current_pos)]->symbol;
					types t2=board[get<0>(current_pos)][get<1>(current_pos)]->type;
					char c2=board[get<0>(current_pos)][get<1>(current_pos)]->color;
					int pp2=board[get<0>(current_pos)][get<1>(current_pos)]->promotion;
					vector<piece*> vec2;
					
					switch(sym1)
					{
					
						case 'K' : 	for(int i=0;i<King.size();i++)
									{
										if(King[i]->type==t1 && King[i]->color==c1)
										{
											vec1.push_back(King[i]);
											King.erase(King.begin()+i);
											i--;
										}
									}
									break;
						
						case 'Q' : 	for(int i=0;i<Queen.size();i++)
									{
										if(Queen[i]->type==t1 && Queen[i]->color==c1 &&                    Queen[i]->promotion==pp1)
										{
											vec1.push_back(Queen[i]);
											Queen.erase(Queen.begin()+i);
											i--;
										}
									}
									break;
						
						case 'N' : 	for(int i=0;i<Night.size();i++)
									{
										if(Night[i]->type==t1 && Night[i]->color==c1)
										{
											vec1.push_back(Night[i]);
											Night.erase(Night.begin()+i);
											i--;
										}
									}
									break;
						
						case 'B' : 	for(int i=0;i<Bishop.size();i++)
									{
										if(Bishop[i]->type==t1 && Bishop[i]->color==c1)
										{
											vec1.push_back(Bishop[i]);
											Bishop.erase(Bishop.begin()+i);
											i--;
										}
									}
									break;
						
						case 'R' : 	for(int i=0;i<Rook.size();i++)
									{
										if(Rook[i]->type==t1 && Rook[i]->color==c1)
										{
											vec1.push_back(Rook[i]);
											Rook.erase(Rook.begin()+i);
											i--;
										}
									}
									break;
						
						case 'P' : 	for(int i=0;i<Pawn.size();i++)
									{
										if(Pawn[i]->type==t1 && Pawn[i]->color==c1)
										{
											vec1.push_back(Pawn[i]);
											Pawn.erase(Pawn.begin()+i);
											i--;
										}
									}
									break;
					
					}
					
					switch(sym2)
					{
					
						case 'K' : 	for(int i=0;i<King.size();i++)
									{
										if(King[i]->type==t2 && King[i]->color==c2)
										{
											vec2.push_back(King[i]);
											King.erase(King.begin()+i);
											i--;
										}
									}
									break;
						
						case 'Q' : 	for(int i=0;i<Queen.size();i++)
									{
										if(Queen[i]->type==t2 && Queen[i]->color==c2 &&                    Queen[i]->promotion==pp2)
										{
											vec2.push_back(Queen[i]);
											Queen.erase(Queen.begin()+i);
											i--;
										}
									}
									break;
						
						case 'N' : 	for(int i=0;i<Night.size();i++)
									{
										if(Night[i]->type==t2 && Night[i]->color==c2)
										{
											vec2.push_back(Night[i]);
											Night.erase(Night.begin()+i);
											i--;
										}
									}
									break;
						
						case 'B' : 	for(int i=0;i<Bishop.size();i++)
									{
										if(Bishop[i]->type==t2 && Bishop[i]->color==c2)
										{
											vec2.push_back(Bishop[i]);
											Bishop.erase(Bishop.begin()+i);
											i--;
										}
									}
									break;
						
						case 'R' : 	for(int i=0;i<Rook.size();i++)
									{
										if(Rook[i]->type==t2 && Rook[i]->color==c2)
										{
											vec2.push_back(Rook[i]);
											Rook.erase(Rook.begin()+i);
											i--;
										}
									}
									break;
						
						case 'P' : 	for(int i=0;i<Pawn.size();i++)
									{
										if(Pawn[i]->type==t2 && Pawn[i]->color==c2)
										{
											vec2.push_back(Pawn[i]);
											Pawn.erase(Pawn.begin()+i);
											i--;
										}
									}
									break;
					
					}
					
					double rans1=(1.0*rand())/RAND_MAX;
					double j1=0;
					for(int i=0;i<vec1.size();i++)
					{
						if(rans1>=j1 && rans1<=j1+vec1[i]->prob)
						{
							vec1[i]->prob=1;
							break;
						}
						j1+=vec1[i]->prob;
					}
					
					double rans2=(1.0*rand())/RAND_MAX;
					double j2=0;
					for(int i=0;i<vec2.size();i++)
					{
						if(rans2>=j2 && rans2<=j2+vec2[i]->prob)
						{
							vec2[i]->prob=1;
							break;
						}
						j2+=vec2[i]->prob;
					}
					
					for(int i=0;i<vec1.size();i++)
					{
						if(vec1[i]->prob==1)
						{
							switch(sym1)
							{
								case 'K' : King.push_back(vec1[i]);
											break;
											
								case 'Q' : Queen.push_back(vec1[i]);
											break;
											
								case 'N' : Night.push_back(vec1[i]);
											break;
											
								case 'B' : Bishop.push_back(vec1[i]);
											break;
											
								case 'R' : Rook.push_back(vec1[i]);
											break;
											
								case 'P' : Pawn.push_back(vec1[i]);
											break;
								
							}
							continue;
						}
						board[get<0>(vec1[i]->pos)][get<1>(vec1[i]->pos)]=NULL;
						delete vec1[i];
						
					}
					
					for(int i=0;i<vec2.size();i++)
					{
						if(vec2[i]->prob==1)
						{
							switch(sym2)
							{
								case 'K' : King.push_back(vec2[i]);
											break;
											
								case 'Q' : Queen.push_back(vec2[i]);
											break;
											
								case 'N' : Night.push_back(vec2[i]);
											break;
											
								case 'B' : Bishop.push_back(vec2[i]);
											break;
											
								case 'R' : Rook.push_back(vec2[i]);
											break;
											
								case 'P' : Pawn.push_back(vec2[i]);
											break;
								
							}
							continue;
						}
						board[get<0>(vec2[i]->pos)][get<1>(vec2[i]->pos)]=NULL;
						delete vec2[i];
						
					}
					if(board[get<0>(current_pos)][get<1>(current_pos)]!=NULL && board[get<0>(next_pos)][get<1>(next_pos)]!=NULL)
					{
						char sym=board[get<0>(next_pos)][get<1>(next_pos)]->symbol;
						types t=board[get<0>(next_pos)][get<1>(next_pos)]->type;
						char c=board[get<0>(next_pos)][get<1>(next_pos)]->color;
					
						switch(sym)
						{
					
							case 'K' : 	for(int i=0;i<King.size();i++)
										{
											if(King[i]->type==t && King[i]->color==c)
											{
												King.erase(King.begin()+i);
												break;
											}
										}
										break;
						
							case 'Q' : 	for(int i=0;i<Queen.size();i++)
										{
											if(Queen[i]->type==t && Queen[i]->color==c)
											{
												Queen.erase(Queen.begin()+i);
												break;
											}
										}
										break;
						
							case 'N' : 	for(int i=0;i<Night.size();i++)
										{
											if(Night[i]->type==t && Night[i]->color==c)
											{
												Night.erase(Night.begin()+i);
												break;
											}
										}
										break;
						
							case 'B' : 	for(int i=0;i<Bishop.size();i++)
										{
											if(Bishop[i]->type==t && Bishop[i]->color==c)
											{
												Bishop.erase(Bishop.begin()+i);
												break;
											}
										}
										break;
						
							case 'R' : 	for(int i=0;i<Rook.size();i++)
										{
											if(Rook[i]->type==t && Rook[i]->color==c)
											{
												Rook.erase(Rook.begin()+i);
												break;
											}
										}
										break;
						
							case 'P' : 	for(int i=0;i<Pawn.size();i++)
										{
											if(Pawn[i]->type==t && Pawn[i]->color==c)
											{
												Pawn.erase(Pawn.begin()+i);
												break;
											}
										}
										break;
					
						}
						delete board[get<0>(next_pos)][get<1>(next_pos)];
						
						if(p->symbol=='P' && (get<0>(next_pos)==0 || get<0>(next_pos)==7) )
						{
							promote(p,next_pos);
							delete p;
							board[get<0>(current_pos)][get<1>(current_pos)]=NULL;
						}
						else
						{
							get<0>(p->pos)=get<0>(next_pos);
							get<1>(p->pos)=get<1>(next_pos);
							board[get<0>(next_pos)][get<1>(next_pos)]=board[get<0>(current_pos)][get<1>(current_pos)];
							board[get<0>(current_pos)][get<1>(current_pos)]=NULL;
						}
					}
					
				}
			}
			else	//For now, add entanglement later
			{
				cout<<"invalid"<<endl;
				return;
			}
		}
		else if(mv[5]=='Q')
		{
			int v=isvalid(p,current_pos,next_pos);
			if(v==0 || board[get<0>(next_pos)][get<1>(next_pos)]!=NULL)
			{
				cout<<"invalid"<<endl;
				return;
			}
			piece *new_piece=copyPiece(p);
			p->prob=p->prob/2;
			new_piece->prob=p->prob;
			get<0>(new_piece->pos)=get<0>(next_pos);
			get<1>(new_piece->pos)=get<1>(next_pos);
			board[get<0>(next_pos)][get<1>(next_pos)]=new_piece;
			
		}
		else
		{
			cout<<"invalid"<<endl;
			return;
		}
		
	
		if(turn=='w')
		{
			turn='b';
		}
		else
		{
			turn='w';
		}	
	}
	
	piece* decode_piece(char p,position cpos)
	{
		if(get<0>(cpos)<0 || get<0>(cpos)>7)
		{
			return NULL;
		}
		if(get<1>(cpos)<0 || get<1>(cpos)>7)
		{
			return NULL;
		}
		
		piece *chk=board[get<0>(cpos)][get<1>(cpos)];
		if(chk==NULL)
		{
			return NULL;
		}
		if(chk->color!=turn)
		{
			return NULL;
		}
		if(chk->symbol!=p)
		{
			return NULL;
		}
		return chk;
	}
	
	int decode(string p,position &z)
	{
		char col=p[0];
		char row=p[1];
		
		
		get<0>(z)= 8 - (row - '0');
		if(get<0>(z)>7 || get<0>(z)<0)
		{
			return 1;
		}
		get<1>(z)=col - 'a';
		if(get<1>(z)>7 || get<1>(z)<0)
		{
			return 1;
		}
		
		return 0;
	}
	
	void promote(piece *p,position npos)	//no underpromotion
	{
		prom_no++;
		make_queen(p->color,get<0>(npos),get<1>(npos),prom_no);
	}
	
	int gameOver()
	{
		int flag_bk=0;
		int flag_wk=0;
		for(int i=0;i<King.size();i++)
		{
			if(King[i]->color=='w')
			{
				flag_wk=1;
			}
			else
			{
				flag_bk=1;
			}
		}
		if(flag_wk==0)
		{
			return 1;
		}
		if(flag_bk==0)
		{
			return 2;
		}
		return 0;
	}
	
	int isvalid(piece* p,position cp,position np)
	{
		if(isAllowed(p->symbol,cp,np)==0)
		{
			return 0;
		}
		if(obstruction(p->symbol,cp,np)==0)
		{
			return 0;
		}
		if(therePiece(np)==0)
		{
			return 0;
		}
		return 1;
		
	}
	
	int isAllowed(char p,position cp,position np)
	{
		int x1=get<0>(cp);
		int x2=get<0>(np);
		int y1=get<1>(cp);
		int y2=get<1>(np);
		
		int x_net=abs(x1-x2);
		int y_net=abs(y1-y2);
		
		if(p=='K')
		{
			if(x_net==1 || y_net==1)
			{
				return 1;
			}
			return 0;
		}
		else if(p=='R')
		{
			if(x_net==0 ^ y_net==0)
			{
				return 1;
			}
			return 0;
		}
		else if(p=='B')
		{
			if(x_net==y_net && (x_net+y_net)>0)
			{
				return 1;
			}
			return 0;
		}
		else if(p=='N')
		{
			if((x_net==2 && y_net==1) || (x_net==1 && y_net==2))
			{
				return 1;
			}
			return 0;
		}
		else if(p=='Q')
		{
			if( (x_net==0 ^ y_net==0) || (x_net==y_net && (x_net+y_net)>0) )
			{
				return 1;
			}
			return 0;
		}
		else if(p=='P')
		{
			if(turn=='w' && y_net==0 && x1-x2==1)
			{
				return 1;
			}
			if(turn=='b' && y_net==0 && x2-x1==1)
			{
				return 1;
			}
			if(turn=='w' && y_net==0 && x1==6 && x2==4)
			{
				return 1;
			}
			if(turn=='b' && y_net==0 && x1==1 && x2==3)
			{
				return 1;
			}
			if(turn=='w' && y_net==1 && x1-x2==1)
			{
				piece *temp=board[x2][y2];
				if(temp!=NULL && temp->color=='b')
				{
					return 1;
				}
			}
			if(turn=='b' && y_net==1 && x2-x1==1)
			{
				piece *temp=board[x2][y2];
				if(temp!=NULL && temp->color=='w')
				{
					return 1;
				}
			}
			return 0;
		}
		
	}
	
	int obstruction(char p,position cp,position np)
	{
		int x1=get<0>(cp);
		int x2=get<0>(np);
		int y1=get<1>(cp);
		int y2=get<1>(np);
		
		int x_net=abs(x1-x2);
		int y_net=abs(y1-y2);
		
		if(p=='K')
		{
			return 1;
		}
		else if(p=='N')
		{
			return 1;
		}
		else if(p=='R')
		{
			if(x_net==0)
			{
				int y_max=(y1>y2)?y1:y2;
				int y_min=(y2>y1)?y1:y2;
				
				for(int i=y_min+1;i<y_max;i++)
				{
					if(board[x1][i]!=NULL)
					{
						return 0;
					}
				}
				return 1;
			}
			if(y_net==0)
			{
				int x_max=(x1>x2)?x1:x2;
				int x_min=(x2>x1)?x1:x2;
				
				for(int i=x_min+1;i<x_max;i++)
				{
					if(board[i][y1]!=NULL)
					{
						return 0;
					}
				}
				return 1;
			}
		}
		else if(p=='B')
		{
			int sign_x=(x2>x1)?1:-1;
			int sign_y=(y2>y1)?1:-1;
			for(int i=x1+sign_x, j=y1+sign_y ; i!=x2 ; i+=sign_x, j+=sign_y)
			{
				if(board[i][j]!=NULL)
				{
					return 0;
				}
			}
			return 1;
		}
		else if(p=='Q')
		{
			if(x_net==0 || y_net==0)
			{
				
				if(x_net==0)
				{
					int y_max=(y1>y2)?y1:y2;
					int y_min=(y2>y1)?y1:y2;
			
					for(int i=y_min+1;i<y_max;i++)
					{
						if(board[x1][i]!=NULL)
						{
							return 0;
						}
					}
					return 1;
				}
				if(y_net==0)
				{
					int x_max=(x1>x2)?x1:x2;
					int x_min=(x2>x1)?x1:x2;
			
					for(int i=x_min+1;i<x_max;i++)
					{
						if(board[i][y1]!=NULL)
						{
							return 0;
						}
					}
					return 1;
				}
			
			}
			else
			{
				int sign_x=(x2>x1)?1:-1;
				int sign_y=(y2>y1)?1:-1;
				for(int i=x1+sign_x, j=y1+sign_y ; i!=x2 ; i+=sign_x, j+=sign_y)
				{
					if(board[i][j]!=NULL)
					{
						return 0;
					}
				}
				return 1;
			}
		}
		else if(p=='P')
		{
			if(turn=='w' && x1==6 && x2==4)
			{
				if(board[5][y1]!=NULL)
				{
					return 0;
				}
				return 1;
			}
			if(turn=='b' && x1==1 && x2==3)
			{
				if(board[2][y1]!=NULL)
				{
					return 0;
				}
				return 1;
			}
		}
	}
	
	int therePiece(position np)
	{
		piece* temp=board[get<0>(np)][get<1>(np)];
		if(temp==NULL)
		{
			return 1;
		}
		if(temp->color!=turn)
		{
			return 1;
		}
		return 0;
	}
	
	piece* copyPiece(piece* p)
	{
		piece* temp=new piece;
		temp->color=p->color;
		temp->type=p->type;
		temp->symbol=p->symbol;
		if(temp->symbol=='K')
		{
			King.push_back(temp);
		}
		else if(temp->symbol=='Q')
		{
			Queen.push_back(temp);
		}
		else if(temp->symbol=='N')
		{
			Night.push_back(temp);
		}
		else if(temp->symbol=='B')
		{
			Bishop.push_back(temp);
		}
		else if(temp->symbol=='R')
		{
			Rook.push_back(temp);
		}
		else if(temp->symbol=='P')
		{
			Pawn.push_back(temp);
		}
		
		return temp;
	}
	
	void make_king(char colr,int x,int y,int prom=0,double p=1)
    {
		piece *temp=new piece;
		temp->pos=make_tuple(x,y);
		temp->color=colr;
		temp->type=king;
		temp->symbol='K';
		temp->promotion=prom;
		temp->prob=p;
		King.push_back(temp);
		board[x][y]=temp;
	}
    
    void make_queen(char colr,int x,int y,int prom=0,double p=1)
    {
		piece *temp=new piece;
		temp->pos=make_tuple(x,y);
		temp->color=colr;
		temp->type=queen;
		temp->symbol='Q';
		temp->promotion=prom;
		temp->prob=p;
		Queen.push_back(temp);
		board[x][y]=temp;
	}
    
    
    void make_rook(char colr,char detail,int x,int y,int prom=0,double p=1)
    {
    	
			piece *temp=new piece;
    		temp->pos=make_tuple(x,y);
			temp->color=colr;
			if(detail=='l')
				temp->type=leftrook;
			else if(detail=='r')
			{
				temp->type=rightrook;
			}
			temp->symbol='R';
			temp->promotion=prom;
			temp->prob=p;
			Rook.push_back(temp);
			board[x][y]=temp;
    }
    void make_knight(char colr,char detail,int x,int y,int prom=0,double p=1)
    {
    	
			piece *temp=new piece;
    		temp->pos=make_tuple(x,y);
			temp->color=colr;
			if(detail=='l')
				temp->type=leftknight;
			else if(detail=='r')
			{
				temp->type=rightknight;
			}
			temp->symbol='N';
			temp->promotion=prom;
			temp->prob=p;
			Night.push_back(temp);
			board[x][y]=temp;
    }
    
    void make_bishop(char colr,char detail,int x,int y,int prom=0,double p=1)
    {
    	
			piece *temp=new piece;
    		temp->pos=make_tuple(x,y);
			temp->color=colr;
			if(detail=='l')
				temp->type=lightbishop;
			else if(detail=='d')
			{
				temp->type=darkbishop;
			}
			temp->symbol='B';
			temp->promotion=prom;
			temp->prob=p;
			Bishop.push_back(temp);
			board[x][y]=temp;
    }
    
    
    
    void make_pawn(char colr,char detail,int x,int y,int prom=0,double p=1)
    {
    	piece *temp=new piece;
		temp->pos=make_tuple(x,y);
		temp->color=colr;
		temp->symbol='P';
		temp->promotion=prom;
		temp->prob=p;
		switch(detail)
		{
			case 'a':	temp->type=apawn;
						break;
			case 'b':	temp->type=bpawn;
						break;
						
			case 'c':	temp->type=cpawn;
						break;
						
			case 'd':	temp->type=dpawn;
						break;
						
			case 'e':	temp->type=epawn;
						break;
						
			case 'f':	temp->type=fpawn;
						break;
						
			case 'g':	temp->type=gpawn;
						break;
						
			case 'h':	temp->type=hpawn;
						break;
						
		}
		Pawn.push_back(temp);
		board[x][y]=temp;
	}
  

	
	void print()
	{
		cout<<"Turn : "<<turn<<"\n"<<endl;
		for(int i=0;i<SIZE;i++)
		{
			for(int j=0;j<SIZE;j++)
			{
                if(board[i][j] == NULL)
                    cout<<"\t\t"<<board[i][j];
                else
                    cout<<"\t\t"<<board[i][j]->color<<board[i][j]->symbol<<board[i][j]->prob;
			}
			cout<<"\n\n\n"<<endl;
		}
		
		int gO=gameOver();

		if(gO==1)
		{
			cout<<"\n\t\tBlack Wins\n"<<endl;
			exit(0);
		}
		else if(gO==2)
		{
			cout<<"\n\t\tWhite Wins\n"<<endl;
			exit(0);
		}
	}
};

int main()
{
	srand(time(NULL));
	chess c;
	string mv;
	
	while(1)
	{
		c.print();
		cout<<"move : ";
		cin>>mv;
		c.move(mv);
	}
	return 0;
}


/*
	promotion, castling, en passent are left out for now
*/


