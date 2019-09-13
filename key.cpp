/*
	0-----------1
	-			-
	-	GAME	-
	-	SCREEN	-
	-			-
	2-----------3
	
	0 , 1 , 2 , 3 --> int x , y ;
	0-2 (y) == 1-3 (y) : SCREEN_HEIGHT
	0-1 (x) == 2-3 (x) : SCREEN_WIDTH
	
*/	
struct offset
	{
		int x , y;
	};
	
void move ( SDL_Surface * screen )
{
	Uint8 * keystates = SDL_GetKeyState ( NULL );
	SDL_Event move_event;
	
	while ( SDL_PollEvent ( & move_event ) )
	{
		if ( move_event.type == SDL_KEYDOWN )
		{
			if ( keystates [ SDLK_RIGHT ] )
//				doodle.x+=5;
				break;
			if ( keystates [ SDLK_LEFT ] )
//				doodle.x-=5;
				break;
		}
		if ( doodle.x == dot[0].x )
			doodle.x = dot[1].x
		if ( doodle.x == dot[1].x )
			doodle.x = dot[0].x
	}
}

int main()
{
	struct offset dot[4];
	//  SDL_QUIT:
	running = false;
}
