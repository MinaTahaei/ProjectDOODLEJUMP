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
    int x;
    int y;
    int type;
    int len;
    int vy;
    int vx = 0;
};

const int board_n = 15;
boards board1[board_n];

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

void init_board()
{
    for( int i=0 ; i<board_n ; i++)
    {
        board[i].x = rand()%800;
        board[i].y = (600/board_n)*i;
        board[i].len = 600/board_n;
        board[i].vy = 0;


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
                }
                else 
                {
                    if( x<65 )
                    {
                        int a = rand()%3;
                        switch (a)
                        case 0:
                            board[i].type = monster1;
                            break;
                        case 1:
                            board[i].type = monster2;
                            break;
                        case 2:
                            board[i].type = monster3;
                            break;
                    }
                    else
                    {
                        board[i].type = stable;
                    }
                }
            }
        }
    }
}

void move_board ()
{
    for( int i=board_n-1 ; i>=0 ; i-- )
    {
        board[i].y+=board[i].vy;
//		board[i].x+=board[i].vx;
        int h = 0;
        for( int j=0 ; j<i ; j++)
        {
            h+=board[j].y;
        }
        if( board[i].y>=h )
        {
            board[i+1].x = board[i].x;
            board[i+1].y = board[i].y;
            board[i+1].type = board[i].type;
            board[i+1].len = board[i].len;
            board[i+1].v = board[i].v;

            board[i].type = empty;

            if ( i==0 )
            {
                board[i].x = rand()%800;
                board[i].y++;
                board[i].len = 600/board_n;
                board[i].v = 0;
                int x=rand()%100;
                if( x<10 )
                {
                    if( board[i+1].type!= broken || board[i+2].type!= broken )
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
                    if( x<40 )
                    {
                        board[i].type = movable;
                        board[i].vx = 5;
                    }
                    else
                    {
                        board[i].type = stable;
                    }
                }
            }
        }
    }

}


int loadIMG( SDL_Surface *screen , string filename , int image_x , int image_y ){

	bool flag = true;
	
	
	SDL_Surface *image;	
	SDL_Surface *temp;	//This pointer will temporarily reference our bitmap sprite
	SDL_Rect src, dest;	//These rectangles will describe the source and destination regions of our blit
	
	
	//When this program exits, SDL_Quit must be called
	atexit(SDL_Quit);
	
	if (screen == NULL) {

		flag=false;
		return 1;
	}
	
	
	temp = SDL_LoadBMP(filename.c_str());

	if (temp == NULL) 
	{
		flag=false;
		return 1;
	}

 image = SDL_DisplayFormat(temp);
	
 SDL_FreeSurface(temp);

  

// while(flag==true){
 
	//Construct the source rectangle for our blit
	src.x = 0;
	src.y = 0;
	src.w = image->w;	//Use imagew to display the entire width of the image
	src.h = image->h;	//Use imageh to display the entire height of the image
	
	//Construct the destination rectangle for our blit
	dest.x = image_x;		//Display the image at the (X,Y) coordinates (100,100)
	dest.y = image_y;
	dest.w = image->w;	//Ensure the destination is large enough for the image's entire width/height
	dest.h = image->h;
	
	
	SDL_BlitSurface(image, &src, screen, &dest);
	
	SDL_Flip(screen);
	
//	}
  return 0;
}

void print_board()
{
    for ( int i=0 ; i<board_n ; i++)
    {
        
        SDL_Surface* board;
        switch ( board[i].type )
        {
            case stable :
                //load stable board's image
                break;
            case broken :
                //load broken board's image
                break;
            case movable :
                //load movable board's image
                break;
            case spring :
                //load spring board's image
                break;
            case jet:
                //load stable board and the jet
                break;
            case monster1:
                //load stable board and the monster
                break;
            case monster2:
                //load stable board and the monster
                break;
            case monster3:
                //load stable board and the monster
                break;
        }
    }
}
 
int main(int argc, char *argv[])
{
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
	 if (screen == NULL) {

		mahnaz=false;
		return 1;
	  }
	

	while(mahnaz==true)
	{
		loadIMG( screen , "background1.bmp" , 0 , 0);

	
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
