#include <fstream>
//at game over 
void show_highscore()
{
	ifstream readFile;
	readFile.open( "address: doodle/highscore.txt" );
	if( readFile.is_open() )
	{
		while ( !readFile.eof() )
			readFile >> highscore;	
	}
	readFile.close( );
	ofstream writeFile ( "address: doodle/highscore.txt" );
	if ( writeFile.is_open() )
	{
		if (score > highscore)
			highscore = score;
		writeFile << highscore
	}
	writeFile.close();
}
