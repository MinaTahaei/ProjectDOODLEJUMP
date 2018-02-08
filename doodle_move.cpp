void doodle_move() :
{
	int vmax = //??;
	int vy = max;
	int g = -//?? ;
	int jump_num = 0;
	int jumpmax =//??;
	bool jumping = true;

	while ( jumping )
	{
		if (vy == max)
		{
			g = -g;
			while( jump_num < jumpmax )
			{
				y -= vy +//(g/2);
				vy += g;
				jump_num ++;
				collision(//);
			}
		}
		if (vy == 0)
		{
			g = -g;
			while (jump_num < jumpmax)
			{
				y += vy +//(g/2);
				vy += g;
				jump_num++;
				collision(//);
			}
		}
	}
}