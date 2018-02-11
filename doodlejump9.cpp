#include<iostream>
#include"SDL/SDL.h"
#include"SDL/SDL_image.h"
#include<SDL/SDL_gfxPrimitives.h>
#include"SDL/SDL_ttf.h"
#include<cstdlib>
#include<ctime>
#include<math.h>
#include "SDL/SDL_mixer.h"
#include<sstream>
#include <string>
#include <fstream>

using namespace std;

struct boards
{
    double x;
    double y;
    int type;
    int len;
    double vy;
    double vx;
    int w;
    int h;
};

const double vmax = 10;
const double g = -0.25;

struct Doodle
{
    double x = 300;
    double y = 599;
    double vy = vmax;
    int width = 100;
    int height = 100;
};


Doodle doodle;

const int board_n = 15;
boards board[board_n];

//boards type
const int broken = 1;
const int stable = 2;
const int movable = 3;
const int spring = 4;
const int jet = 5;
const int monster1 = 6;
const int monster2 = 7;
const int monster3 = 8;
const int empty = 9;
const int mirror = 10;
const int brokenbroken = 11;
const int completlybroken = 12;
const int spring2 = 13;

int score=20;
double highscore;
double tempscore = 700;

Mix_Music *music = NULL; 
 
Mix_Chunk *scratch = NULL; 
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

void show_score(SDL_Surface * screen, TTF_Font* scorefont)
{
    stringstream scoreString;
    SDL_Surface * shown_score;
    SDL_Color color={0,130,255};
    
    scoreString<<"score : "<<score;
    shown_score = TTF_RenderText_Solid(scorefont,scoreString.str().c_str(),color);
	
	SDL_Rect position;
    position.x = 50;
    position.y = 50;
    
    SDL_BlitSurface(shown_score,NULL,screen,&position);
    SDL_Flip(screen);
}


void show_highscore(SDL_Surface * screen , TTF_Font* scorefont )
{	
    stringstream highscoreString;
    SDL_Surface * shown_highscore;
    SDL_Color color={130,0,130};

 
    highscoreString<<"highscore : "<<highscore;
    shown_highscore = TTF_RenderText_Solid(scorefont,highscoreString.str().c_str(),color);
    
	SDL_Rect position;
    position.x = 50;
    position.y = 100;
    
    SDL_BlitSurface(shown_highscore,NULL,screen,&position);
    SDL_Flip(screen);
}


int showmenu_start(SDL_Surface* screen , TTF_Font* font , TTF_Font* titlefont )	//{ "play","exit"};0,1
{
	int x,y;
	const int menunumber=2;
	const char* menuitem[menunumber]={ "play","exit"};//0,1
	SDL_Surface* menu[menunumber]={NULL};
	bool selected_item[menunumber]={0,0};
	SDL_Color color[3]={{255,255,255},{255,0,0},{80,210,30}};
	
	SDL_Surface* title=TTF_RenderText_Solid(titlefont,"doodle jump",color[2]);
	
	SDL_Rect positiontitle;
	positiontitle.x=140;
	positiontitle.y=50;
	
	menu[0]=TTF_RenderText_Solid(font,menuitem[0],color[0]);
	menu[1]=TTF_RenderText_Solid(font,menuitem[1],color[0]);
	
	SDL_Rect position[menunumber];
	
	position[0].x=140;
	position[0].y=250;
	position[0].w=200;
	position[0].h=150;

	position[1].x=140;
	position[1].y=450;
	position[1].w=200;
	position[1].h=200;


	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 00, 00, 00 ) );


	SDL_Event event;
	
	while(true)	
	{
		
		for(int i=0;i<menunumber;i++)
			SDL_BlitSurface(menu[i],NULL,screen,&position[i]);
		SDL_BlitSurface(title,NULL,screen,&positiontitle);
			
		while(SDL_PollEvent(&event))
		{
			
			switch(event.type)
			{
				
				case(SDL_QUIT):
				{
					SDL_FreeSurface(title);
					for(int i=0;i<menunumber;i++)
						SDL_FreeSurface(menu[i]);
					return 1;
					break;
				}	
				
				case(SDL_MOUSEMOTION):
				{
					
					x=event.motion.x;
					y=event.motion.y;
					
					for(int i=0; i<menunumber;i++)
					{
						
						if( x>=position[i].x && x<=position[i].x+position[i].w && y>=position[i].y && y<=position[i].y+position[i].h )
						{
							
							if(selected_item[i]!=1)
							{
								
								SDL_FreeSurface(menu[i]);
								menu[i]=TTF_RenderText_Solid(font,menuitem[i],color[1]);
								selected_item[i]=1;									
							}
						}
						
						else
						{
							
							if(selected_item[i]==1)
							{
								
								SDL_FreeSurface(menu[i]);
								menu[i]=TTF_RenderText_Solid(font,menuitem[i],color[0]);
								selected_item[i]=0;	
							}
						}
					}
					
					break;
				}
				
				case(SDL_MOUSEBUTTONDOWN):
				{
									
					if(event.button.button=SDL_BUTTON_LEFT)
					{
						
						x=event.button.x;
						y=event.button.y;
						
						for(int i=0;i<menunumber;i++)
						{
							
							if(x>=position[i].x && x<=position[i].x + position[i].w && y>=position[i].y && y<=position[i].y + position[i].h)
							{	
							//	SDL_Delay(500);
								SDL_FreeSurface(title);
								for(int j=0;j<menunumber;j++)
									SDL_FreeSurface( menu[j]);
								return i;
							}	
						}
					}
					
					break;
				}		
			}	
		}
		SDL_Flip(screen);
	}
	
}


int showmenu_pause(SDL_Surface* screen , TTF_Font* font , TTF_Font* titlefont )	//{ "continue", "play again" , "exit"};0,1,2
{
	int x,y;
	const int menunumber=3;
	const char* menuitem[menunumber]={ "continue", "play again" , "exit"};//0,1,2
	SDL_Surface* menu[menunumber]={NULL};
	bool selected_item[menunumber]={0,0,0};
	SDL_Color color[3]={{255,255,255},{255,0,0},{255,160,0}};
	
	SDL_Surface* title=TTF_RenderText_Solid(titlefont,"pause",color[2]);
	
	SDL_Rect positiontitle;
	positiontitle.x=140;
	positiontitle.y=50;
	
	menu[0]=TTF_RenderText_Solid(font,menuitem[0],color[0]);
	menu[1]=TTF_RenderText_Solid(font,menuitem[1],color[0]);
	menu[2]=TTF_RenderText_Solid(font,menuitem[2],color[0]);
	
	SDL_Rect position[menunumber];
	
	position[0].x=140;
	position[0].y=200;
	position[0].w=200;
	position[0].h=100;

	position[1].x=140;
	position[1].y=350;
	position[1].w=100;
	position[1].h=100;

	position[2].x=140;
	position[2].y=500;
	position[2].w=100;
	position[2].h=150;
	
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 00, 00, 00 ) );
	
	SDL_Event event;
	
	while(1)	
	{
		for(int i=0;i<menunumber;i++)
			SDL_BlitSurface(menu[i],NULL,screen,&position[i]);

		SDL_BlitSurface(title,NULL,screen,&positiontitle);

		while(SDL_PollEvent(&event))
		{
			
			switch(event.type)
			{
				
				case(SDL_QUIT):
				{
					SDL_FreeSurface(title);					
					for(int i=0;i<menunumber;i++)
						SDL_FreeSurface(menu[i]);
					return 2;
					break;
				}
					
				case(SDL_MOUSEMOTION):
				{
					
					x=event.motion.x;
					y=event.motion.y;
					
					for(int i=0; i<menunumber;i++)
					{
						
						if( x>=position[i].x && x<=position[i].x+position[i].w && y>=position[i].y && y<=position[i].y+position[i].h )
						{
							
							if(selected_item[i]!=1)// !selected_item[i]
							{								
								SDL_FreeSurface(menu[i]);
								menu[i]=TTF_RenderText_Solid(font,menuitem[i],color[1]);
								selected_item[i]=1;									
							}	
						}
						
						else
						{
							
							if(selected_item[i]==1)// selected_item[i]
							{
								SDL_FreeSurface(menu[i]);
								menu[i]=TTF_RenderText_Solid(font,menuitem[i],color[0]);
								selected_item[i]=0;	
							}
						}
					}
					break;
				}
				
				case(SDL_MOUSEBUTTONDOWN):
				{	
								
					if(event.button.button=SDL_BUTTON_LEFT)
					{
						
						x=event.button.x;
						y=event.button.y;
						
						for(int i=0;i<menunumber;i++)
						{
							
							if(x>=position[i].x && x<=position[i].x + position[i].w && y>=position[i].y && y<=position[i].y + position[i].h)
							{	
								
							//	SDL_Delay(500);
								SDL_FreeSurface(title);	
								for(int j=0;j<menunumber;j++)
									SDL_FreeSurface( menu[j]);								
								return i;
							}	
						}
					}
					break;
				}
				
				case(SDL_KEYDOWN):
				{
					
					if(event.key.keysym.sym==SDLK_ESCAPE)
					{
						
					//	SDL_Delay(500);
						SDL_FreeSurface(title);	
						for(int i=0;i<menunumber;i++)
							SDL_FreeSurface( menu[i]);
						return 0;
					}
					break;
				}	
					
			}
				
		}
		SDL_Flip(screen);
	//	SDL_Delay(20);
	}
}


int showmenu_gameover(SDL_Surface* screen , TTF_Font* font , TTF_Font* titlefont , TTF_Font* scorefont )	//{ "play again","exit"};0,1
{
	
	
	
	int x,y;
	const int menunumber=2;
	const char* menuitem[menunumber]={ "play again","exit"};//0,1
	SDL_Surface* menu[menunumber]={NULL};
	bool selected_item[menunumber]={0,0};
	SDL_Color color[3]={{255,255,255},{0,130,255},{255,0,0}};
	
	SDL_Surface* title=TTF_RenderText_Solid(titlefont,"game over!!",color[2]);

	SDL_Rect positiontitle;
	positiontitle.x=140;
	positiontitle.y=150;
	
	menu[0]=TTF_RenderText_Solid(font,menuitem[0],color[0]);
	menu[1]=TTF_RenderText_Solid(font,menuitem[1],color[0]);
	
	SDL_Rect position[menunumber];

	
	position[0].x=140;
	position[0].y=300;
	position[0].w=200;
	position[0].h=100;

	position[1].x=140;
	position[1].y=450;
	position[1].w=200;
	position[1].h=100;

	
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 00, 00, 00 ) );
	

	show_score(screen, scorefont);
	show_highscore(screen, scorefont);

	SDL_Event event;
	
	while(1)	
	{
		
		for(int i=0;i<menunumber;i++)
			SDL_BlitSurface(menu[i],NULL,screen,&position[i]);
		SDL_BlitSurface(title,NULL,screen,&positiontitle);

		while(SDL_PollEvent(&event))
		{
			
			switch(event.type)
			{
				
				case(SDL_QUIT):
				{
					SDL_FreeSurface(title);	
					for(int i=0;i<menunumber;i++)
						SDL_FreeSurface(menu[i]);
					
					return 1;
					break;
				}	
				
				case(SDL_MOUSEMOTION):
				{
					
					x=event.motion.x;
					y=event.motion.y;
					
					for(int i=0; i<menunumber;i++)
					{
						
						if( x>=position[i].x && x<=position[i].x+position[i].w && y>=position[i].y && y<=position[i].y+position[i].h )
						{
							
							if(selected_item[i]!=1)
							{	
								SDL_FreeSurface(menu[i]);
								menu[i]=TTF_RenderText_Solid(font,menuitem[i],color[1]);
								selected_item[i]=1;									
							}
								
						}
						
						else
						{
							
							if(selected_item[i]==1)
							{	
								SDL_FreeSurface(menu[i]);
								menu[i]=TTF_RenderText_Solid(font,menuitem[i],color[0]);
								selected_item[i]=0;	
							}
						}
					}
					break;
				}
				
				case(SDL_MOUSEBUTTONDOWN):
				{			
						
					if(event.button.button=SDL_BUTTON_LEFT)
					{
						
						x=event.button.x;
						y=event.button.y;
						
						for(int i=0;i<menunumber;i++)
						{
							
							if(x>=position[i].x && x<=position[i].x + position[i].w && y>=position[i].y && y<=position[i].y + position[i].h)
							{	
				//				SDL_Delay(500);
								SDL_FreeSurface(title);	
								for(int j=0;j<menunumber;j++)
									SDL_FreeSurface( menu[j]);
								return i;
							}	
						}
					}
					break;
				}		
			}	
		}
		SDL_Flip(screen);
	}	
}

void play_music()
{
	music = Mix_LoadMUS( "piano.mp3" );
	    //Free the music
    Mix_FreeMusic( music );
}

int loadIMG( SDL_Surface *screen , SDL_Surface *image , int image_x , int image_y )
{
    SDL_Rect src, dest;
    //Construct the source rectangle for our blit
    src.x = 0;
    src.y = 0;
    src.w = image->w;   //Use imagew to display the entire width of the image
    src.h = image->h;   //Use imageh to display the entire height of the image
    
    //Construct the destination rectangle for our blit
    dest.x = image_x;       //Display the image at the (X,Y) coordinates (100,100)
    dest.y = image_y;
    dest.w = image->w;  //Ensure the destination is large enough for the image's entire width/height
    dest.h = image->h;
    
    
    SDL_BlitSurface(image, &src, screen, &dest);
    
    
    

  return 0;
}


void print_board(SDL_Surface *screen,SDL_Surface *stableS,SDL_Surface *brokenS,SDL_Surface *movableS,SDL_Surface *monster1S,SDL_Surface *monster2S,SDL_Surface *monster3S,SDL_Surface *jetS,SDL_Surface *mirrorS,SDL_Surface *springS,SDL_Surface *spring2S,SDL_Surface *brokenbrokenS,SDL_Surface *completlybrokenS)
{
    for ( int i=0 ; i<board_n ; i++)
    {
        

        switch ( board[i].type )
        {
            case stable :
                loadIMG(screen, stableS, board[i].x, board[i].y);
                break;
            case broken :
                loadIMG(screen, brokenS, board[i].x, board[i].y);
                break;
            case movable :
                loadIMG(screen,movableS, board[i].x, board[i].y);
                break;
            case spring :
                loadIMG(screen, springS,board[i].x, board[i].y-17);
                break;
            case spring2: 
            	loadIMG(screen, spring2S,board[i].x, board[i].y-34);
            	break;
            case jet:
                loadIMG(screen, jetS ,board[i].x, board[i].y-30);
                loadIMG(screen, stableS, board[i].x , board[i].y);
                break;
            case monster1:
                loadIMG(screen, monster1S,board[i].x, board[i].y-100);
                loadIMG(screen, stableS, board[i].x, board[i].y);
                break;
            case monster2:
                loadIMG(screen, monster2S,board[i].x, board[i].y-100);
                loadIMG(screen, stableS, board[i].x, board[i].y);
                break;
            case monster3:
                 loadIMG(screen, monster3S,board[i].x, board[i].y-100);
                 loadIMG(screen, stableS, board[i].x, board[i].y);
                break;
            case mirror:
                if(board[i].x<300)
                {
                    loadIMG(screen, mirrorS,450, board[i].y-50);
                    loadIMG(screen, stableS,board[i].x, board[i].y);
                }
                else
                {
                    loadIMG(screen, mirrorS,50, board[i].y-50);
                    loadIMG(screen, stableS , board[i].x, board[i].y);
                }
                break;
            case brokenbroken:
                loadIMG(screen,brokenbrokenS,board[i].x,board[i].y);
                break;
            case completlybroken:
                loadIMG(screen,completlybrokenS,board[i].x,board[i].y);
                break;

        }
    }
}

void init_board()
{
	score=0;
	doodle.vy = vmax;
	doodle.x = 300;
	doodle.y = 599;

    for( int i=0 ; i<board_n ; i++)
    {
        board[i].x = rand()%560+20;
        board[i].y = (700/board_n)*i;
        board[i].len = 700/board_n;
        board[i].vx = 0;


        int x=rand()%100;
        if( x<20 )
        {
            if( board[i-1].type!= broken || board[i-2].type!= broken )
            {
                board[i].type = broken;
            }
            else
            {
                board[i].type = stable;
            }
        }
        else
        {
            if (x<40)
            {
                board[i].type = spring;
            }
             else
            {
                board[i].type = stable;
            }

        }
        if(i == board_n-1 || i == board_n-2)
        {
        	board[i].type = stable;
        }
    }
}

void move_board (double v)
{
    for( int i=board_n ; i>=0 ; i-- )
    {
        board[i].y+=v;
        board[i].x+=board[i].vx;

        if (board[i].type==completlybroken)
        {
            board[i].type = empty;
        }

        
        if ( board[i].x<20 || board[i].x>580 )
        {
             board[i].vx*=-1;  
        }

        int h = 0;
        for( int j=0 ; j<=i ; j++)
        {
            h+=board[j].len;
        }
        if( board[i].y>=h )
        {
            board[i+1].x = board[i].x;
            board[i+1].y = board[i].y;
            board[i+1].type = board[i].type;
            board[i+1].len = board[i].len;
            board[i+1].vx = board[i].vx;

            //board[i].type = empty;

            if ( i==0 )
            {
                board[i].x = rand()%560+20;
                board[i].y = 0;

                if(board[i].len<60)
                    board[i].len +=1;


                int x=rand()%100;
                if( x<15 )
                {
                    if( board[i+1].type!= broken || board[i+2].type!= broken )
                    {
                        board[i].type = broken;
                        board[i].vx = 0;
                    }
                    else
                    {
                        board[i].type = stable;
                        board[i].vx = 0;
                    }
                }
                else
                {
                    if(x<20)
                    {
                        board[i].type = mirror;
                        board[i].vx = 0;
                    }
                    else
                    {
                        if( x<40 )
                        {
                            board[i].type = movable;
                            board[i].vx = rand()%5+1;
                        }
                        else
                        {
                            if ( x<55 )
                            {
                                board[i].type = spring;
                                board[i].vx = 0;
                            }
                            else
                            {
                                if(x<60)
                                {
                                    board[i].type = jet;
                                    board[i].vx = 0;
                                }
                                else 
                                {
                                    if( x<65 )
                                    {
                                        int a = rand()%3;
                                        switch (a)
                                        {
                                        case 0 :
                                            board[i].type = monster1;
                                            board[i].vx = 0;
                                            break;
                                        case 1 :
                                            board[i].type = monster2;
                                            board[i].vx = 0;
                                            break;
                                        case 2 :
                                            board[i].type = monster3;
                                            board[i].vx = 0;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        board[i].type = stable;
                                        board[i].vx = 0;
                                    }
                                }
                            }
                        }
                    }
                }
           }
        }
        if (board[i].type==brokenbroken)
        {
            board[i].type = completlybroken;
        }
    }
}

void jetmove(SDL_Surface *screen,SDL_Surface *backgroundS,SDL_Surface *doodlejetS,SDL_Surface *stableS,SDL_Surface *brokenS,SDL_Surface *movableS,SDL_Surface *monster1S,SDL_Surface *monster2S,SDL_Surface *monster3S,SDL_Surface *jetS,SDL_Surface *mirrorS,SDL_Surface *springS,SDL_Surface *spring2S,SDL_Surface *brokenbrokenS,SDL_Surface *completlybrokenS)
{
    for(int i=0;i<100;i++)
    {
    	score += 10;
        loadIMG(screen,backgroundS,0,0);
        print_board(screen,stableS,brokenS,movableS,monster1S,monster2S,monster3S,jetS,mirrorS,springS,spring2S,brokenbrokenS,completlybrokenS);
        loadIMG(screen,doodlejetS,200,156);
        SDL_Flip(screen);
        move_board(10);
    }
}

bool collision_check(int i,int depth=0)
{
    if(doodle.x + doodle.width/4 < board[i].x + board[i].w && doodle.x + doodle.width*3/4 > board[i].x && doodle.y + doodle.height > board[i].y - board[i].h &&  doodle.y + doodle.height < board[i].y + depth )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool collision(SDL_Surface *screen,SDL_Surface *backgroundS,SDL_Surface *doodlejetS,SDL_Surface *stableS,SDL_Surface *brokenS,SDL_Surface *movableS,SDL_Surface *monster1S,SDL_Surface *monster2S,SDL_Surface *monster3S,SDL_Surface *jetS,SDL_Surface *mirrorS,SDL_Surface *springS,SDL_Surface *spring2S,SDL_Surface *brokenbrokenS,SDL_Surface *completlybrokenS)
{
    for (int i = 0; i < board_n; i++)
    {
        switch (board[i].type)
        {
            case broken:
                board[i].w = 100;
                board[i].h = 0;
                if(doodle.vy<0)
                {
                    if ( collision_check(i,23))
                    {    
                    	music = Mix_LoadMUS( "beep.mp3" );
                        board[i].type = brokenbroken;
                    }
                }
                break;
            case movable:
            case stable:   
                if(doodle.vy<0)
                {
                    board[i].w = 100;
                    board[i].h = 0;
                    if ( collision_check(i,23))
                    {    
                    	music = Mix_LoadMUS( "beep.mp3" );
                        doodle.vy = vmax;
                    }
                }
                break;
            case spring:   
                if (doodle.vy<0)
                {
                    board[i].w = 100;
                    board[i].h = 0;
                    if (collision_check(i,23))
                    {   
                    	music = Mix_LoadMUS( "beep.mp3" );
                        doodle.vy = 2*vmax;
                        board[i].type = spring2;
                    }
                }
                break;
            case jet:
            //count the stable   
                board[i].w = 25;
                board[i].h = 35;
                if (collision_check(i,100))
                {
                    jetmove(screen,backgroundS,doodlejetS,stableS,brokenS,movableS,monster1S,monster2S,monster3S,jetS,mirrorS,springS,spring2S,brokenbrokenS,completlybrokenS);
                    doodle.vy = vmax/2;
                    doodle.x = 300;
                    doodle.y = 200;
                }
                board[i].w = 100;
                if (collision_check(i,23))
                {
                    jetmove(screen,backgroundS,doodlejetS,stableS,brokenS,movableS,monster1S,monster2S,monster3S,jetS,mirrorS,springS,spring2S,brokenbrokenS,completlybrokenS);
                    music = Mix_LoadMUS( "beep.mp3" );
                    doodle.vy = vmax/2;
                    doodle.x = 300;
                    doodle.y = 200;
                }
                    break;

            case monster1:
            case monster2:
            case monster3:
                board[i].w = 100;
                board[i].h = 100;
                if (collision_check(i,123))
                {
                    if (doodle.vy >= 0)
                    {
                        return false;
                    }
                    if (doodle.vy < 0);
                    { 
                    	music = Mix_LoadMUS( "beep.mp3" );
                        board[i].type = stable;
                        doodle.vy = vmax;
                    }
                    
                }
                break;
            case mirror:
                board[i].w = 100;
                board[i].h = 0;
                if (collision_check(i,23))
                {    
                	music = Mix_LoadMUS( "beep.mp3" );
                    doodle.vy = vmax;
                }
                if (board[i].x <300)
                {
                    if(doodle.x + doodle.width/4 < 525 && doodle.x + doodle.width*3/4 > 450 && doodle.y + doodle.height > board[i].y - 50 &&  doodle.y + doodle.height < board[i].y + 150 )
                    {
                   
                        return false;
                    }
                }
                else
                {
                    if(doodle.x + doodle.width/4 < 125 && doodle.x + doodle.width*3/4 > 50 && doodle.y + doodle.height > board[i].y - 50 &&  doodle.y + doodle.height < board[i].y + 150 )
                    {

                        return false;
                    }
                }
                break;
        }
    }
    if(doodle.y >600 )
        return false;
    return true;
}


bool doodle_move(SDL_Surface *screen,SDL_Surface *backgroundS,SDL_Surface *doodlejetS,SDL_Surface *stableS,SDL_Surface *brokenS,SDL_Surface *movableS,SDL_Surface *monster1S,SDL_Surface *monster2S,SDL_Surface *monster3S,SDL_Surface *jetS,SDL_Surface *mirrorS,SDL_Surface *springS,SDL_Surface *spring2S,SDL_Surface *brokenbrokenS,SDL_Surface *completlybrokenS)
{
        //  jumping up
        if (doodle.y < 100 && doodle.vy > 0)
        {
            move_board(doodle.vy);
        } 

        //  jumping down
        else 
        {
            move_board(0);
            doodle.y -= doodle.vy;
        }
        if (collision(screen,backgroundS,doodlejetS,stableS,brokenS,movableS,monster1S,monster2S,monster3S,jetS,mirrorS,springS,spring2S,brokenbrokenS,completlybrokenS)==false)
        {
            return false;
        }
        if(doodle.vy>0 && (tempscore > doodle.y ||doodle.y<100))
        {
        	if (doodle.y < tempscore)
        	{
        		tempscore = doodle.y;
        	}
        	score +=doodle.vy;
        	cout<<"doodle.vy : "<<doodle.vy<<"\t"<<"doodle.y : "<<doodle.y<<"\t"<<"score : "<<score<<endl;
        }

        doodle.vy += g;

        return true;
}


SDL_Surface *init_surface(string filename)
{
    bool flag = true;
    
    
    SDL_Surface *image; 
    SDL_Surface *temp;  //This pointer will temporarily reference our bitmap sprite
    SDL_Rect src, dest; //These rectangles will describe the source and destination regions of our blit
    
    
    //When this program exits, SDL_Quit must be called
    atexit(SDL_Quit);
    
    
    
    temp = SDL_LoadBMP(filename.c_str());

    if (temp == NULL) 
    {
        flag=false;
        return NULL;
    }
 
 image = SDL_DisplayFormat(temp);

 Uint32 colorkey = SDL_MapRGB( image->format, 0xFF, 0xFF, 0xFF );
 SDL_SetColorKey( image , SDL_SRCCOLORKEY, colorkey );
    
 SDL_FreeSurface(temp);

return image;
}


 
int main(int argc, char *argv[])
{     
    srand(time(0));
	
	bool running , quit = false , again = false ;
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	
	TTF_Font* font=TTF_OpenFont("zachary.ttf",50);
	TTF_Font* titlefont=TTF_OpenFont("zachary.ttf",80);
	TTF_Font* scorefont=TTF_OpenFont("zachary.ttf",30);


	if (SDL_Init(SDL_INIT_EVERYTHING)!= 0) 
	{
	 	running = false;
        return 1;

	} 
/*
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
	 return false; 
	}
*/
    SDL_Surface *screen;
	screen = SDL_SetVideoMode(700 , 700 , 32 , SDL_DOUBLEBUF);

	if (screen == NULL) 
    {
		running=false;
		return 1;
	}


    SDL_Surface *backgroundS = init_surface("background.bmp");
    SDL_Surface *brokenS = init_surface("broken.bmp");
    SDL_Surface *stableS = init_surface("stable.bmp");
    SDL_Surface *movableS = init_surface("movable.bmp");
    SDL_Surface *monster1S = init_surface("monster1.bmp");
    SDL_Surface *monster2S = init_surface("monster2.bmp");
    SDL_Surface *monster3S = init_surface("monster3.bmp");
    SDL_Surface *doodleRS = init_surface("doodle.bmp");
    SDL_Surface *doodleLS = init_surface("doodleL.bmp");
    SDL_Surface *doodleS = doodleRS;
    SDL_Surface *jetS = init_surface("jet.bmp");
    SDL_Surface *mirrorS = init_surface("mirror.bmp");
    SDL_Surface *springS = init_surface("spring.bmp");
    SDL_Surface *spring2S = init_surface("spring2.bmp");
    SDL_Surface *brokenbrokenS = init_surface("brokenbroken.bmp");
    SDL_Surface *completlybrokenS = init_surface("completelybroken.bmp");
    SDL_Surface *doodlejetS = init_surface("doodlejet.bmp");

    Uint8 * keystates = SDL_GetKeyState ( NULL );


	int i=showmenu_start (screen,font,titlefont);
	
	if(i==0)
	 	running=true;

	 	
	else if(i==1)
	{
		quit=true; 
		SDL_FreeSurface(screen);
		return 1; 
	}




    while(!quit)
    {

	    init_board();
	    
		ifstream readFile;
		readFile.open( "highscore.txt" );
		if( readFile.is_open() )
		{
			while ( !readFile.eof() )
				readFile >> highscore;	
		}
		readFile.close( );
		
		if(again == true)
		{
			running = true;
			again = false;
		}
	   
	    SDL_Event occur;
		 
		while(running == true)
		{

	        if ( !SDL_PollEvent ( &occur ) )
	        {
	            if ( occur.type == SDL_KEYDOWN )
	            {
					if ( keystates [ SDLK_ESCAPE ] )
					{
						i=showmenu_pause(screen,font,titlefont);
						
	/*					if(i==0)	//continue
						{
							
						}
	*/					if(i==1)	//play agian
						{
							// play again and set everything again
							init_board();
							running=false;
							again =true;
						}
						if(i==2)	//exit
						{
							quit=true;
							running=false; 
							SDL_FreeSurface(screen);
							return 1; 
						}
					}
								
	                if ( keystates [ SDLK_RIGHT ] )
	                {
	                    doodle.x+=5;
	                    doodleS = doodleRS;
	                }
	                if ( keystates [ SDLK_LEFT ] )
	                {
	                    doodle.x-=5;
	                    doodleS = doodleLS;
	                }
	                if(doodle.x>600)
	                    doodle.x=0;
	                if(doodle.x<0)
	                    doodle.x=600;
	            }
	        }

	        loadIMG(screen , backgroundS , 0 , 0);
//	        play_music();

	        print_board(screen,stableS,brokenS,movableS,monster1S,monster2S,monster3S,jetS,mirrorS,springS,spring2S,brokenbrokenS,completlybrokenS);

	        loadIMG(screen , doodleS , doodle.x , doodle.y);
	       
	        show_score(screen, scorefont);

	        SDL_Flip(screen);


			ofstream writeFile ( "highscore.txt" );
			if ( writeFile.is_open() )
			{
				if (score > highscore)
					highscore = score;
				writeFile << highscore;
			}
			writeFile.close();

	        if(doodle_move(screen,backgroundS,doodlejetS,stableS,brokenS,movableS,monster1S,monster2S,monster3S,jetS,mirrorS,springS,spring2S,brokenbrokenS,completlybrokenS)==false)
	        {
				i=showmenu_gameover(screen,font,titlefont,scorefont);

	        	if(i==0)	//play again
				{
				// play again and set everything again
					init_board();
					running=false;
					again =true;

				}
				if(i==1)	//exit
				{
					quit=true;
					running=false; 
					SDL_FreeSurface(screen);
					return 1; 
				}
	        }

/*	    if( music == NULL )
	    {
	        return false;
	    }
*/

		    
	        //SDL_PollEvent(&occur);
	  
	     	if(occur.type==SDL_QUIT)
	    	{
	    		quit=true;
	         	running=false;
	        	SDL_FreeSurface(screen);
	         	return 1;

	    	}

		}
   }
	TTF_CloseFont(font);
	TTF_CloseFont(titlefont);
	TTF_CloseFont(scorefont);

	TTF_Quit();
//	SDL_Quit();
    SDL_QUIT;
	return 0;
 }
