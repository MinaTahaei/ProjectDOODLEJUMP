#include<iostream>
#include"SDL/SDL.h"
#include"SDL/SDL_image.h"
#include<SDL/SDL_gfxPrimitives.h>
#include"SDL/SDL_ttf.h"
#include<cstdlib>
#include<ctime>
#include<math.h>
#include<sstream>
#include <string>
using namespace std;

struct boards
{
    float x;
    float y;
    int type;
    int len;
    float vy;
    float vx;
    // added:
    int w;
    int h;
};

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

void init_board()
{
    for( int i=0 ; i<board_n ; i++)
    {
        board[i].x = rand()%600;
        board[i].y = (700/board_n)*i;
        board[i].len = 700/board_n;
        board[i].vy = 0.5;
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
    }
}

void move_board ()
{
	//	brokenbroken-->completlybroken
	//	completlybroken-->empty
	//	score calculating
    for( int i=board_n ; i>=0 ; i-- )
    {
        board[i].y+=board[i].vy;
        board[i].x+=board[i].vx;

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
            board[i+1].vy = board[i].vy;
            board[i+1].vx = board[i].vx;

            board[i].type = empty;

            if ( i==0 )
            {
                board[i].x = rand()%600;
                board[i].y = 0;
                board[i].len +=2;
                board[i].vy = 0.5;
                int x=rand()%100;
                

                if( x<20 )
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
                    if( x<40 )
                    {
                        board[i].type = movable;
                        board[i].vx = rand()%5+1;
                    }
                    else
                    {
                        if ( x<60 )
                        {
                            board[i].type = spring;
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

           }//check kon badish monster nabashe age bood khali kone
            
        }
    }

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
//doodle_move_function

void print_board(SDL_Surface *screen,SDL_Surface *stableS,SDL_Surface *brokenS,SDL_Surface *movableS,SDL_Surface *monster1S,SDL_Surface *monster2S,SDL_Surface *monster3S,SDL_Surface *jetS,SDL_Surface *mirrorS,SDL_Surface *springS)
{
    for ( int i=0 ; i<board_n ; i++)
    {
        
//		SDL_Surface* boards;
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
            case jet:
                loadIMG(screen, jetS,board[i].x, board[i].y-100);
                loadIMG(screen, stableS, board[i].x, board[i].y);
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
                if(board[i].x>300)
                {
                    loadIMG(screen, mirrorS,450, board[i].y-50);
                    loadIMG(screen, stableS,100, board[i].y);
                }
                else
                {
                    loadIMG(screen, mirrorS,50, board[i].y-50);
                    loadIMG(screen, stableS,500, board[i].y);
                }
                break;
        }
    }
}
 
int main(int argc, char *argv[])
{     
    srand(time(0));
	SDL_Event occur;
	bool mahnaz = true;
	SDL_Init(SDL_INIT_EVERYTHING);

	if (SDL_Init(SDL_INIT_EVERYTHING)!= 0) 
	{
	 	mahnaz = false;
        return 1;

	} 

    SDL_Surface *screen;
	screen = SDL_SetVideoMode(700 , 700 , 32 , SDL_DOUBLEBUF);

	if (screen == NULL) 
    {
		mahnaz=false;
		return 1;
	}

    init_board();


    SDL_Surface *backgroundS = init_surface("background.bmp");
    SDL_Surface *brokenS = init_surface("broken.bmp");
    SDL_Surface *stableS = init_surface("stable.bmp");
    SDL_Surface *movableS = init_surface("movable.bmp");
    SDL_Surface *monster1S = init_surface("monster1.bmp");
    SDL_Surface *monster2S = init_surface("monster2.bmp");
    SDL_Surface *monster3S = init_surface("monster3.bmp");
    SDL_Surface *doodleS = init_surface("doodle.bmp");
    SDL_Surface *jetS = init_surface("jet.bmp");
    SDL_Surface *mirrorS = init_surface("mirror.bmp");
    SDL_Surface *springS = init_surface("spring.bmp");
	 
	while(mahnaz==true)
	{
        loadIMG(screen , backgroundS , 0 , 0);

        print_board(screen,stableS,brokenS,movableS,monster1S,monster2S,monster3S,jetS,mirrorS,springS);

        SDL_Flip(screen);

        move_board();
	    
        SDL_PollEvent(&occur);
  
     	if(occur.type==SDL_QUIT)
    	{

         	mahnaz=false;
        	SDL_FreeSurface(screen);
         	return 1;

    	}

	}
   

    SDL_QUIT;
	return 0;
 }
