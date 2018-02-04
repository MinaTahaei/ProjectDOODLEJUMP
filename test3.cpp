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

int loadIMG(){

	bool flag = true;
	
	SDL_Surface *screen;
	SDL_Surface *image;	
	SDL_Surface *temp;	//This pointer will temporarily reference our bitmap sprite
	SDL_Rect src, dest;	//These rectangles will describe the source and destination regions of our blit
	
	
	//When this program exits, SDL_Quit must be called
	atexit(SDL_Quit);
	
	screen = SDL_SetVideoMode(800 , 600 , 32 , SDL_DOUBLEBUF);
	if (screen == NULL) {

		flag=false;
		return 1;
	}
	
	
	temp = SDL_LoadBMP("background.bmp");

	if (temp == NULL) 
	{
		flag=false;
		return 1;
	}

 image = SDL_DisplayFormat(temp);
	
 SDL_FreeSurface(temp);

  

 while(flag==true){
 
	//Construct the source rectangle for our blit
	src.x = 0;
	src.y = 0;
	src.w = image->w;	//Use imagew to display the entire width of the image
	src.h = image->h;	//Use imageh to display the entire height of the image
	
	//Construct the destination rectangle for our blit
	dest.x = 150;		//Display the image at the (X,Y) coordinates (100,100)
	dest.y = 150;
	dest.w = image->w;	//Ensure the destination is large enough for the image's entire width/height
	dest.h = image->h;
	
	
	SDL_BlitSurface(image, &src, screen, &dest);
	
	SDL_Flip(screen);
	
	}
  return 0;
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
	  screen = SDL_SetVideoMode(800 , 600 , 32 , SDL_DOUBLEBUF);
	 if (screen == NULL) {

		mahnaz=false;
		return 1;
	  }
	

	while(mahnaz==true)
	{
		loadIMG();

	
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
