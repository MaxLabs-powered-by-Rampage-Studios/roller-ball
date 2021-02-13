#include "Iw2D.h"

class play
{
	public:
		void drawball(int,CIwFVec2,CIwFVec2);
		void drawbar(CIwFVec2,CIwFVec2);
		void create();
		void destroy();
	private:
		CIw2DImage *ball[5];
		CIw2DImage *bar;
};

void play::destroy()
{
	int i;
	for(i=0;i<5;i++)
		delete ball[i];
	delete bar;
}

void play::create()
{
	ball[0] = Iw2DCreateImage("ball_matt.png");
	ball[1] = Iw2DCreateImage("ball_steel.png");
	ball[2] = Iw2DCreateImage("ball_titanium.png");
	ball[3] = Iw2DCreateImage("ball_plastic.png");
	ball[4] = Iw2DCreateImage("ball_rubber.png");

	bar = Iw2DCreateImage("plank.png");
}

void play::drawball(int c, CIwFVec2 b_pos, CIwFVec2 b_size)
{
	switch(c)
	{
		case 0:
			Iw2DDrawImage(ball[0],b_pos,b_size);
			break;
		case 1:
			Iw2DDrawImage(ball[1],b_pos,b_size);
			break;
		case 2:
			Iw2DDrawImage(ball[2],b_pos,b_size);
			break;
		case 3:
			Iw2DDrawImage(ball[3],b_pos,b_size);
			break;
		case 4:
			Iw2DDrawImage(ball[4],b_pos,b_size);
			break;
	}
}

void play::drawbar(CIwFVec2 nb_pos, CIwFVec2 nb_size)
{
	Iw2DDrawImage(bar,nb_pos,nb_size);
}