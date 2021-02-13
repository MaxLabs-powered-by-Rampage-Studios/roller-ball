#include "s3e.h"
#include "Iw2D.h"
#include "game.h"
#include "Play.h"
#include "IwResManager.h"
#include "s3eOSExec.h"

CIw2DFont *font,*dfont,*hfont;
CIw2DImage *_name, *_bg,*_about,*_setting,*_option, *_about_content,*_share;
CIw2DImage *_resume,*_play,*_scores,*_newgame, *_score;
CIw2DImage *_music_off, *_music_a, *_music_b, *_music_c, *_title_music;
CIw2DImage *_title_high;
CIw2DImage *_control, *_control_acc, *_control_touch;
CIw2DImage *_b_matt, *_b_steel, *_b_titanium, *_b_plastic, *_b_rubber, *_title_ball, *_locked;

CIwColour col[32], s_col[32];

struct _save
{
	int a, p , r , b , t , l;
	float s, ts, tsb, hs[3];
	CIwFVec2 bp[6];
	CIwFVec2 m_p;
} *_store;

play start;
char str[100];
int page, over, i, j, z, resume, trans, ballsel, life, option, track, click, acc;
int m_tran;
CIwFVec2 barpos[6], b_size;
bool iscaptured;

int32 accX, lSmoothFactor, deltaX;

float speed, time_scale, time_scale_ball, b_speed, bh_speed, x, score, highscore[3];

CGame::CGame()
	: m_Position((float)Iw2DGetSurfaceWidth()/2,0)
, m_Size(Iw2DGetSurfaceHeight()*0.07f,Iw2DGetSurfaceHeight()*0.07f)
{
	
	IwResManagerInit();

	IwGetResManager()->LoadGroup("Iw2DStrings.group");

	if(Iw2DGetSurfaceWidth()<=480)
	{
		font = Iw2DCreateFontResource("segoeprint16");
		dfont = Iw2DCreateFontResource("segoeprint26_black");
		hfont = Iw2DCreateFontResource("segoeprint26_aqua");
	}
	else if(Iw2DGetSurfaceWidth()<=800)
	{
		font = Iw2DCreateFontResource("segoeprint26");
		dfont = Iw2DCreateFontResource("segoeprint36_black");
		hfont = Iw2DCreateFontResource("segoeprint36_aqua");
	}
	else
	{
		font = Iw2DCreateFontResource("segoeprint36");
		dfont = Iw2DCreateFontResource("segoeprint36_black");
		hfont = Iw2DCreateFontResource("segoeprint36_aqua");
	}

	_name = Iw2DCreateImage("name.png");
	_bg = Iw2DCreateImage("background.png");

	Iw2DSurfaceClear(0xff000000);
	Iw2DSetColour(0xffffffff);
	Iw2DDrawImage(_bg,CIwFVec2(0.0f,0.0f),CIwFVec2((float)Iw2DGetSurfaceWidth(),(float)Iw2DGetSurfaceHeight()));
	Iw2DDrawImage(_name,CIwFVec2((float)(Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.90f)/2),(float)(Iw2DGetSurfaceHeight()/6-((float)Iw2DGetSurfaceHeight()*(0.90f/3))/2)),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.90f,(float)Iw2DGetSurfaceWidth()*(0.90f/3.12f)));
	Iw2DSurfaceShow();

	_share = Iw2DCreateImage("share.png");
	_about = Iw2DCreateImage("about.png");
	_about_content = Iw2DCreateImage("aboutwindow.png");
	_setting = Iw2DCreateImage("setting.png");
	_option = Iw2DCreateImage("optionswindow.png");

	_resume = Iw2DCreateImage("resume.png");
	_play = Iw2DCreateImage("play.png");
	_scores = Iw2DCreateImage("scores.png");
	_score = Iw2DCreateImage("score.png");
	_newgame = Iw2DCreateImage("newgame.png");

	
	_music_off = Iw2DCreateImage("music_off.png");
	_music_a = Iw2DCreateImage("track_a.png");
	_music_b = Iw2DCreateImage("track_b.png");
	_music_c = Iw2DCreateImage("track_c.png");
	_title_music = Iw2DCreateImage("title_music.png");

	_title_high = Iw2DCreateImage("highscore.png");

	_title_ball = Iw2DCreateImage("title_ball.png");
	_b_matt = Iw2DCreateImage("b_matt.png");
	_b_steel = Iw2DCreateImage("b_steel.png");
	_b_titanium = Iw2DCreateImage("b_titanium.png");
	_b_plastic = Iw2DCreateImage("b_plastic.png");
	_b_rubber = Iw2DCreateImage("b_rubber.png");
	_locked = Iw2DCreateImage("locked.png");

	_control = Iw2DCreateImage("control.png");
	_control_acc = Iw2DCreateImage("control_acc.png");
	_control_touch = Iw2DCreateImage("control_touch.png");

	start.create();

	col[0].Set(0x00ffffff);	col[8].Set(0x40ffffff);		col[16].Set(0x80ffffff);	col[24].Set(0xc0ffffff);
	col[1].Set(0x07ffffff);	col[9].Set(0x47ffffff);		col[17].Set(0x87ffffff);	col[25].Set(0xc7ffffff);
	col[2].Set(0x10ffffff);	col[10].Set(0x50ffffff);	col[18].Set(0x90ffffff);	col[26].Set(0xd0ffffff);
	col[3].Set(0x17ffffff);	col[11].Set(0x57ffffff);	col[19].Set(0x97ffffff);	col[27].Set(0xd7ffffff);
	col[4].Set(0x20ffffff);	col[12].Set(0x60ffffff);	col[20].Set(0xa0ffffff);	col[28].Set(0xe0ffffff);
	col[5].Set(0x27ffffff);	col[13].Set(0x67ffffff);	col[21].Set(0xa7ffffff);	col[29].Set(0xe7ffffff);
	col[6].Set(0x30ffffff);	col[14].Set(0x70ffffff);	col[22].Set(0xb0ffffff);	col[30].Set(0xf0ffffff);
	col[7].Set(0x37ffffff);	col[15].Set(0x77ffffff);	col[23].Set(0xb7ffffff);	col[31].Set(0xffffffff);

	s_col[0].Set(0x000000ff);	s_col[8].Set(0x400000ff);	s_col[16].Set(0x800000ff);	s_col[24].Set(0xc00000ff);
	s_col[1].Set(0x070000ff);	s_col[9].Set(0x470000ff);	s_col[17].Set(0x870000ff);	s_col[25].Set(0xc70000ff);
	s_col[2].Set(0x100000ff);	s_col[10].Set(0x500000ff);	s_col[18].Set(0x900000ff);	s_col[26].Set(0xd00000ff);
	s_col[3].Set(0x170000ff);	s_col[11].Set(0x570000ff);	s_col[19].Set(0x970000ff);	s_col[27].Set(0xd70000ff);
	s_col[4].Set(0x200000ff);	s_col[12].Set(0x600000ff);	s_col[20].Set(0xa00000ff);	s_col[28].Set(0xe00000ff);
	s_col[5].Set(0x270000ff);	s_col[13].Set(0x670000ff);	s_col[21].Set(0xa70000ff);	s_col[29].Set(0xe70000ff);
	s_col[6].Set(0x300000ff);	s_col[14].Set(0x700000ff);	s_col[22].Set(0xb00000ff);	s_col[30].Set(0xf00000ff);
	s_col[7].Set(0x370000ff);	s_col[15].Set(0x770000ff);	s_col[23].Set(0xb70000ff);	s_col[31].Set(0xff0000ff);

	_store = new struct _save;

	accX = 0;
	lSmoothFactor = IW_GEOM_ONE / 4;

	if(s3eSecureStorageGet(_store,sizeof(struct _save)) == S3E_RESULT_SUCCESS)
	{
		for(i=0;i<6;i++)
		{
			barpos[i].y=_store->bp[i].y;
			barpos[i].x=_store->bp[i].x;
		}
		ballsel =_store->b;
		resume = _store->r;
		page = _store->p;
		track = _store->t;

		switch (track)
		{
			case 1:
				s3eAudioPlay("bk_music.mp3",0);
				break;
			case 2:
				s3eAudioPlay("bk_music1.mp3",0);
				break;
			case 3:
				s3eAudioPlay("bk_music2.mp3",0);
				break;
		}

		m_Position.x = _store->m_p.x;
		m_Position.y = _store->m_p.y;
		
		life = _store->l;
		score = _store->s;
		for(i=0;i<3;i++)
		{
			highscore[i] = _store->hs[i];
		}

		time_scale = _store->ts;
		time_scale_ball = _store->tsb;

		acc = _store->a;
		
		b_size.x=Iw2DGetSurfaceWidth()*0.6f;
		b_size.y=Iw2DGetSurfaceHeight()*0.03f;

		x = 0;
		trans = 0;
		over = 0;
		option = 0;
		click = 0;
		
		speed = 0.002f;
		b_speed = 0.01f;
		bh_speed = 0.5f;
		iscaptured=false;
		
		for(m_tran=1;(Iw2DGetSurfaceWidth()*0.5f)-m_tran>=0;m_tran+=8){};
		j=1;
	}
	else
	{
		for(i=0;i<3;i++)
		{
			highscore[i] = 1000;
		}
		page=0;
		track=1;
		ballsel = 0;
		click = 0;
		acc = 0;
		s3eAudioPlay("bk_music.mp3",0);
		reset();
	}
}


CGame::~CGame()
{
	delete font;
	delete dfont;
	delete hfont;

	delete _name;
	delete _bg;
	delete _share;
	delete _about;
	delete _about_content;
	delete _setting;
	delete _option;

	delete _resume;
	delete _play;
	delete _scores;
	delete _score;
	delete _newgame;

	delete _music_off;
	delete _music_a;
	delete _music_b;
	delete _music_c;
	delete _title_music;

	delete _title_high;

	delete _title_ball;
	delete _b_matt;
	delete _b_steel;
	delete _b_titanium;
	delete _b_plastic;
	delete _b_rubber;
	delete _locked;

	delete _store;

	delete _control;
	delete _control_acc;
	delete _control_touch;

	start.destroy();

	IwGetResManager()->DestroyGroup("Iw2DStrings");
	IwResManagerTerminate();
}


void CGame::Update()
{
	if(trans==0)
	{
		if(page==1)
		{
			if(acc==1)
			{
				play_Page_Update_Acc();
			}
			else
			{
				play_Page_Update();
			}
		}
		else
		{
			if(j==1||j==32)
			{
				main_Page_Update();
			}
		}	
	}
}


void CGame::Render()
{
    Iw2DSurfaceClear(0xff000000);

	Iw2DSetColour(0xffffffff);
	Iw2DDrawImage(_bg,CIwFVec2(0.0f,0.0f),CIwFVec2((float)Iw2DGetSurfaceWidth(),(float)Iw2DGetSurfaceHeight()));

	if(page==0)
	{
		if(trans == 1)
		{
			trans_Page();
		}
		else
		{
			main_Page();
		}
	}
	else if(page==1)
	{
		if(trans==1)
		{
			trans_Page();
		}
		else
		{
			play_Page();
		}
	}
    Iw2DSurfaceShow();
}


void CGame::main_Page()
{
	Iw2DDrawImage(_name,CIwFVec2((float)(Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.90f)/2),(float)(Iw2DGetSurfaceHeight()/6-((float)Iw2DGetSurfaceHeight()*(0.90f/3))/2)),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.90f,(float)Iw2DGetSurfaceWidth()*(0.90f/3.12f)));
	Iw2DDrawImage(_scores,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-(float)(Iw2DGetSurfaceWidth()*0.3f)/2,(float)Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)-20),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.3f,(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)));

	Iw2DDrawImage(_setting,CIwFVec2(5,(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f)-5),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.17f,(float)Iw2DGetSurfaceWidth()*0.17f));
	Iw2DDrawImage(_about,CIwFVec2((float)(Iw2DGetSurfaceWidth()-(float)Iw2DGetSurfaceWidth()*0.17f-5),(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f-5)),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.17f,(float)Iw2DGetSurfaceWidth()*0.17f));
	
	if(resume==0)
	{
		Iw2DDrawImage(_play,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.3f)/2,(float)Iw2DGetSurfaceHeight()*0.45f),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.3f,(float)Iw2DGetSurfaceWidth()*(0.3f/1.39f)));
	}
	else
	{
		Iw2DDrawImage(_newgame,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.50f)/2,(float)Iw2DGetSurfaceHeight()/2-(float)Iw2DGetSurfaceWidth()*(0.50f/3.32f)-10),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.50f,(float)Iw2DGetSurfaceWidth()*(0.50f/3.32f)));
		Iw2DDrawImage(_resume,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.40f)/2,(float)Iw2DGetSurfaceHeight()/2+30),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.40f,(float)Iw2DGetSurfaceWidth()*(0.40f/3.24f)));
	}

	if(option!=0)
	{
		if(j<32)
		{
			Iw2DSetColour(col[j-1]);
			Iw2DDrawImage(_option,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/2,Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*1.25f));
			switch (option)
			{
				case 1:
					Iw2DDrawImage(_title_music,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.15f),CIwFVec2(Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*0.3f));
					Iw2DDrawImage(_title_ball,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.35f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.4f),CIwFVec2(Iw2DGetSurfaceWidth()*0.35f,Iw2DGetSurfaceWidth()*0.35f*0.55f));
					Iw2DDrawImage(_control,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.65f),CIwFVec2(Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*0.3f));
					break;
				case 2:
					Iw2DDrawImage(_about_content,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*0.7f*1.38f/2),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*1.38f));
					Iw2DDrawImage(_share,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.7f/2-Iw2DGetSurfaceWidth()*0.17f*0.35f,Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2+Iw2DGetSurfaceWidth()*0.17f*0.2f),CIwFVec2(Iw2DGetSurfaceWidth()*0.17f,Iw2DGetSurfaceWidth()*0.17f));
					break;
				case 3:
					Iw2DSetFont(dfont);
					Iw2DDrawImage(_title_high,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.1f),CIwFVec2(Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*0.3f));
					sprintf(str,"I>		%.0f",highscore[0]);
					Iw2DDrawString(str,CIwFVec2(0,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.3f),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*0.4f*0.3f),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
					sprintf(str,"II>	%.0f",highscore[1]);
					Iw2DDrawString(str,CIwFVec2(0,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.5f),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*0.4f*0.3f),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
					sprintf(str,"III>	%.0f",highscore[2]);
					Iw2DDrawString(str,CIwFVec2(0,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.7f),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*0.4f*0.3f),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
					break;
			}
			j++;
		}
		else
		{
			option_Win();
		}
	}
	else if(j!=1)
	{
		if(j>1)
		{
			j--;
			Iw2DSetColour(col[j-1]);
			Iw2DDrawImage(_option,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/2,Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*1.25f));
		}
	}
} 

void CGame::play_Page()
{
	start.drawball(ballsel,m_Position,m_Size);
	for(i=0;i<6;i++)
	{
		start.drawbar(barpos[i],b_size);
	}
	switch (life)
	{
		case 1:
			start.drawball(0,CIwFVec2((float)Iw2DGetSurfaceWidth()-Iw2DGetSurfaceWidth()*0.06f-10,10),CIwFVec2(Iw2DGetSurfaceWidth()*0.06f,Iw2DGetSurfaceWidth()*0.06f));
			break;
		case 2:
			start.drawball(0,CIwFVec2((float)Iw2DGetSurfaceWidth()-Iw2DGetSurfaceWidth()*0.06f-10,10),CIwFVec2(Iw2DGetSurfaceWidth()*0.06f,Iw2DGetSurfaceWidth()*0.06f));
			start.drawball(0,CIwFVec2((float)Iw2DGetSurfaceWidth()-2*(Iw2DGetSurfaceWidth()*0.06f)-20,10),CIwFVec2(Iw2DGetSurfaceWidth()*0.06f,Iw2DGetSurfaceWidth()*0.06f));
			break;
		case 3:
			start.drawball(0,CIwFVec2((float)Iw2DGetSurfaceWidth()-Iw2DGetSurfaceWidth()*0.06f-10,10),CIwFVec2(Iw2DGetSurfaceWidth()*0.06f,Iw2DGetSurfaceWidth()*0.06f));
			start.drawball(0,CIwFVec2((float)Iw2DGetSurfaceWidth()-2*(Iw2DGetSurfaceWidth()*0.06f)-20,10),CIwFVec2(Iw2DGetSurfaceWidth()*0.06f,Iw2DGetSurfaceWidth()*0.06f));
			start.drawball(0,CIwFVec2((float)Iw2DGetSurfaceWidth()-3*(Iw2DGetSurfaceWidth()*0.06f)-30,10),CIwFVec2(Iw2DGetSurfaceWidth()*0.06f,Iw2DGetSurfaceWidth()*0.06f));
			break;
	}
	Iw2DDrawImage(_score,CIwFVec2(0,0),CIwFVec2(Iw2DGetSurfaceWidth()*0.26f,Iw2DGetSurfaceWidth()*(0.26f/2.47f)));
	Iw2DSetFont(font);
	sprintf(str,"%.0f",score);
	Iw2DDrawString(str,CIwFVec2(Iw2DGetSurfaceWidth()*0.30f,0),CIwFVec2(200,Iw2DGetSurfaceWidth()*(0.30f/3.2f)),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
	if (over == 1)
	{
		Iw2DDrawImage(_option,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/2,Iw2DGetSurfaceHeight()*0.5f-Iw2DGetSurfaceWidth()*1.25f/2),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*1.25f));
		if(score>highscore[0])
		{
			Iw2DSetFont(dfont);
			Iw2DDrawString("GAME OVER",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2-200),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawString("Score",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2-100),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			sprintf(str,"%.0f",score);
			Iw2DDrawString(str,CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawString("New High Score!!!",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2+100),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		}
		else if(score>highscore[1])
		{
			Iw2DSetFont(dfont);
			Iw2DDrawString("GAME OVER",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2-200),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawString("Score",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2-100),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			sprintf(str,"%.0f",score);
			Iw2DDrawString(str,CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawString("One of Best Score!!!",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2+100),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		}
		else if(score>highscore[2])
		{
			Iw2DSetFont(dfont);
			Iw2DDrawString("GAME OVER",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2-200),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawString("Score",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2-100),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			sprintf(str,"%.0f",score);
			Iw2DDrawString(str,CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawString("Good Score!!!",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2+100),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		}
		else
		{
			Iw2DSetFont(dfont);
			Iw2DDrawString("GAME OVER",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2-150),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawString("Score",CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2-50),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			sprintf(str,"%.0f",score);
			Iw2DDrawString(str,CIwFVec2(0,(float)Iw2DGetSurfaceHeight()/2+50),CIwFVec2((float)Iw2DGetSurfaceWidth(),100),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		}
	}
}

void CGame::trans_Page()
{
	if(page==1)
	{
		if(j<m_tran)
		{
			Iw2DDrawImage(_name,CIwFVec2((float)(Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.90f)/2),(float)(Iw2DGetSurfaceHeight()/6-((float)Iw2DGetSurfaceHeight()*(0.90f/3))/2)-j),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.90f,(float)Iw2DGetSurfaceWidth()*(0.90f/3.12f)));
			Iw2DDrawImage(_scores,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-(float)(Iw2DGetSurfaceWidth()*0.3f)/2,(float)Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)-20+j),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.3f,(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)));
			Iw2DDrawImage(_setting,CIwFVec2(5,(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f)-5+j),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.17f,(float)Iw2DGetSurfaceWidth()*0.17f));
			Iw2DDrawImage(_about,CIwFVec2((float)(Iw2DGetSurfaceWidth()-(float)Iw2DGetSurfaceWidth()*0.17f-5),(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f-5)+j),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.17f,(float)Iw2DGetSurfaceWidth()*0.17f));
			if(resume==0)
			{
				Iw2DDrawImage(_play,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.3f-j*0.65f)/2,(float)Iw2DGetSurfaceHeight()*0.45f+j*0.65f*0.72f),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.3f-j*0.65f,(float)Iw2DGetSurfaceWidth()*(0.3f/1.39f)-j*0.65f*0.72f));
			}
			else if(resume == -1)
			{
				Iw2DDrawImage(_newgame,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.5f-j)/2,(float)Iw2DGetSurfaceHeight()/2-((float)Iw2DGetSurfaceWidth()*(0.5f/3.32f)-j*0.3f)-10),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.5f-j,(float)Iw2DGetSurfaceWidth()*(0.5f/3.32f)-j*0.3f));
			}
			else
			{
				Iw2DDrawImage(_resume,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.40f-j*0.85f)/2,(float)Iw2DGetSurfaceHeight()/2+30+j*0.85f*0.31f),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.40f-j*0.85f,(float)Iw2DGetSurfaceWidth()*(0.40f/3.24f)-j*0.85f*0.31f));
			}
			j+=16;
		}
		else
		{
			j=1;
			resume = 0;
			trans=0;
			if(acc == 1)
			{
				s3eAccelerometerStart();
			}
		}
	}
	else if(page == 0)
	{
		if(j>=1)
		{
			Iw2DDrawImage(_name,CIwFVec2((float)(Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.90f)/2),(float)(Iw2DGetSurfaceHeight()/6-((float)Iw2DGetSurfaceHeight()*(0.90f/3))/2)-j),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.90f,(float)Iw2DGetSurfaceWidth()*(0.90f/3.12f)));
			Iw2DDrawImage(_scores,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-(float)(Iw2DGetSurfaceWidth()*0.3f)/2,(float)Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)-20+j),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.3f,(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)));
			Iw2DDrawImage(_setting,CIwFVec2(5,(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f)-5+j),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.17f,(float)Iw2DGetSurfaceWidth()*0.17f));
			Iw2DDrawImage(_about,CIwFVec2((float)(Iw2DGetSurfaceWidth()-(float)Iw2DGetSurfaceWidth()*0.17f-5),(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f-5)+j),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.17f,(float)Iw2DGetSurfaceWidth()*0.17f));
			if(resume==0)
			{
				Iw2DDrawImage(_play,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.3f-j*0.65f)/2,(float)Iw2DGetSurfaceHeight()*0.45f+j*0.65f*0.72f),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.3f-j*0.65f,(float)Iw2DGetSurfaceWidth()*(0.3f/1.39f)-j*0.65f*0.72f));
			}
			else if(resume == -1)
			{
				Iw2DDrawImage(_newgame,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.5f-j)/2,(float)Iw2DGetSurfaceHeight()/2-((float)Iw2DGetSurfaceWidth()*(0.5f/3.32f)-j*0.3f)-10),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.5f-j,(float)Iw2DGetSurfaceWidth()*(0.5f/3.32f)-j*0.3f));
			}
			else
			{
				Iw2DDrawImage(_resume,CIwFVec2((float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.40f-j*0.85f)/2,(float)Iw2DGetSurfaceHeight()/2+30+j*0.31f),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.40f-j*0.85f,(float)Iw2DGetSurfaceWidth()*(0.40f/3.24f)-j*0.31f));
			}
			j-=16;
		}
		else
		{
			j=1;
			trans=0;
		}
	}
}

void CGame::option_Win()
{
	Iw2DDrawImage(_option,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/2,Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*1.25f));
	switch (option)
	{
		case 1:
			Iw2DDrawImage(_title_music,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.15f),CIwFVec2(Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*0.3f));
			Iw2DDrawImage(_title_ball,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.35f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.4f),CIwFVec2(Iw2DGetSurfaceWidth()*0.35f,Iw2DGetSurfaceWidth()*0.35f*0.55f));
			Iw2DDrawImage(_control,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.65f),CIwFVec2(Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*0.3f));
			break;
		case 2:
			Iw2DDrawImage(_about_content,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*0.7f*1.38f/2),CIwFVec2((float)Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*1.38f));
			Iw2DDrawImage(_share,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.7f/2-Iw2DGetSurfaceWidth()*0.17f*0.35f,Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2+Iw2DGetSurfaceWidth()*0.17f*0.2f),CIwFVec2(Iw2DGetSurfaceWidth()*0.17f,Iw2DGetSurfaceWidth()*0.17f));
			break;
		case 4:
			Iw2DDrawImage(_title_music,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.1f),CIwFVec2(Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*0.3f));
			switch (track)
			{
				case 1:
					Iw2DSetColour(0xff0000ff);
					Iw2DDrawImage(_music_a,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.45f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DSetColour(0xffffffff);
					Iw2DDrawImage(_music_off,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.29f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DDrawImage(_music_b,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.61f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DDrawImage(_music_c,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.77f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					break;
				case 2:
					Iw2DSetColour(0xff0000ff);
					Iw2DDrawImage(_music_b,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.61f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DSetColour(0xffffffff);
					Iw2DDrawImage(_music_off,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.29f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DDrawImage(_music_a,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.45f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DDrawImage(_music_c,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.77f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					break;
				case 3:
					Iw2DSetColour(0xff0000ff);
					Iw2DDrawImage(_music_c,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.77f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DSetColour(0xffffffff);
					Iw2DDrawImage(_music_off,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.29f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DDrawImage(_music_a,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.45f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DDrawImage(_music_b,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.61f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					break;
				case 0:
					Iw2DSetColour(0xff0000ff);
					Iw2DDrawImage(_music_off,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.29f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DSetColour(0xffffffff);
					Iw2DDrawImage(_music_a,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.45f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DDrawImage(_music_b,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.61f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					Iw2DDrawImage(_music_c,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.77f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
					break;
			}

			if(x<0)
			{
				x++;
			}
			break;
		case 5:
			Iw2DDrawImage(_title_ball,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.35f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.1f),CIwFVec2(Iw2DGetSurfaceWidth()*0.35f,Iw2DGetSurfaceWidth()*0.35f*0.55f));
					
			if(highscore[0]>=120000)
			{
				switch (ballsel)
				{
					case 0:
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+3,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+4,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_rubber,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 1:
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+3,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_rubber,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 2:
						start.drawball(ballsel-2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_rubber,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 3:
						start.drawball(ballsel-3,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_rubber,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 4:
						start.drawball(ballsel-4,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-3,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_rubber,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
				}
			}
			else if(highscore[0]>=75000)
			{
				switch (ballsel)
				{
					case 0:
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+3,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 1:
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 2:
						start.drawball(ballsel-2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 3:
						start.drawball(ballsel-3,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_plastic,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
				}
			}
			else if(highscore[0]>=50000)
			{
				switch (ballsel)
				{
					case 0:
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 1:
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 2:
						start.drawball(ballsel-2,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_titanium,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
				}
			}
			else if(highscore[0]>=25000)
			{
				switch (ballsel)
				{
					case 0:
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel+1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
					case 1:
						start.drawball(ballsel-1,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_steel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
				}
			}
			else
			{
				switch (ballsel)
				{
					case 0:
						start.drawball(ballsel,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()/3,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f*0.3f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xff0000ff);
						Iw2DDrawImage(_b_matt,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DSetColour(0xffffffff);
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						Iw2DDrawImage(_locked,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
						break;
				}
			}
			if(x<0)
			{
				x++;
			}
			if(x>0)
			{
				switch (click)
				{
					case 1:
						Iw2DSetFont(hfont);
						Iw2DDrawString("Get Highscore 25000",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.85f),CIwFVec2((float)Iw2DGetSurfaceWidth(),40),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
						break;
					case 2:
						Iw2DSetFont(hfont);
						Iw2DDrawString("Get Highscore 50000",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.85f),CIwFVec2((float)Iw2DGetSurfaceWidth(),40),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
						break;
					case 3:
						Iw2DSetFont(hfont);
						Iw2DDrawString("Get Highscore 75000",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.85f),CIwFVec2((float)Iw2DGetSurfaceWidth(),40),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
						break;
					case 4:
						Iw2DSetFont(hfont);
						Iw2DDrawString("Get Highscore 120000",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.85f),CIwFVec2((float)Iw2DGetSurfaceWidth(),40),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
						break;
				}
				x--;
			}
			break;
		case 6:
			Iw2DDrawImage(_control,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.2f),CIwFVec2(Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*0.3f));

			if(acc == 0)
			{
				Iw2DSetColour(0xff0000ff);
				Iw2DDrawImage(_control_touch,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.5f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
				Iw2DSetColour(0xffffffff);
				Iw2DDrawImage(_control_acc,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.7f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
			}
			else
			{
				Iw2DSetColour(0xff0000ff);
				Iw2DDrawImage(_control_acc,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.7f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
				Iw2DSetColour(0xffffffff);
				Iw2DDrawImage(_control_touch,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.5f),CIwFVec2(Iw2DGetSurfaceWidth()*0.4f,Iw2DGetSurfaceWidth()*0.4f*0.3f));
			}

			if(x<0)
			{
				x++;
			}
			break;
		case 3:
			Iw2DSetFont(dfont);
			Iw2DDrawImage(_title_high,CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.1f),CIwFVec2(Iw2DGetSurfaceWidth()*0.7f,Iw2DGetSurfaceWidth()*0.7f*0.3f));
			sprintf(str,"I>		%.0f",highscore[0]);
			Iw2DDrawString(str,CIwFVec2(0,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.3f),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*0.4f*0.3f),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			sprintf(str,"II>	%.0f",highscore[1]);
			Iw2DDrawString(str,CIwFVec2(0,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.5f),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*0.4f*0.3f),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			sprintf(str,"III>	%.0f",highscore[2]);
			Iw2DDrawString(str,CIwFVec2(0,(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.7f),CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()*0.4f*0.3f),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			break;
	}
}

void CGame::main_Page_Update()
{
	if(option == 0)
	{
		if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
		{
			_store->b = ballsel;
			for(i=0;i<6;i++)
			{
				_store->bp[i].y = barpos[i].y;
				_store->bp[i].x = barpos[i].x;
			}
			_store->r = resume;
			_store->p = page;
			_store->t = track;
	
			_store->m_p.x = m_Position.x;
			_store->m_p.y = m_Position.y;
			
			_store->l = life;
			_store->s = score;

			_store->ts = time_scale;
			_store->tsb = time_scale_ball;

			for(i=0;i<3;i++)
			{
				_store->hs[i] = highscore[i];
			}

			s3eSecureStoragePut(_store,sizeof(struct _save));
			s3eAudioStop();
			s3eDeviceRequestQuit();
		}
		if( s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN )
		{
			if(resume == 0)
			{
				if(s3ePointerGetX()>=(float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.3f)/2&&s3ePointerGetX()<=(float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.3f)/2+(float)Iw2DGetSurfaceWidth()*0.3f&&s3ePointerGetY()>=(float)Iw2DGetSurfaceHeight()*0.45f&&s3ePointerGetY()<=(float)Iw2DGetSurfaceHeight()*0.45f+(float)Iw2DGetSurfaceWidth()*(0.3f/1.39f))
				{
					page = 1;
					trans = 1;
				}
			}
			else
			{
				if(s3ePointerGetX()>=(float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceHeight()*0.3f)/2&&s3ePointerGetX()<=(float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceHeight()*0.3f)/2+(float)Iw2DGetSurfaceHeight()*0.3f&&s3ePointerGetY()>=(float)Iw2DGetSurfaceHeight()/2-(float)Iw2DGetSurfaceHeight()*(0.3f/3.32f)-10&&s3ePointerGetY()<=(float)Iw2DGetSurfaceHeight()/2-(float)Iw2DGetSurfaceHeight()*(0.3f/3.32f)-10+(float)Iw2DGetSurfaceHeight()*(0.3f/3.32f))
				{
					reset();
					resume=-1;
					page = 1;
					trans = 1;
				}
				else if(s3ePointerGetX()>=(float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.40f)/2&&s3ePointerGetX()<=(float)Iw2DGetSurfaceWidth()/2-((float)Iw2DGetSurfaceWidth()*0.40f)/2+(float)Iw2DGetSurfaceWidth()*0.40f&&s3ePointerGetY()>=(float)Iw2DGetSurfaceHeight()/2+30&&s3ePointerGetY()<=(float)Iw2DGetSurfaceHeight()/2+30+(float)Iw2DGetSurfaceWidth()*(0.40f/3.24f))
				{
					page = 1;
					trans = 1;
				}
			}
			if((s3ePointerGetX()>=5&&s3ePointerGetX()<=(float)Iw2DGetSurfaceWidth()*0.17f+5)&&s3ePointerGetY()>=(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f)-5&&s3ePointerGetY()<=(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f)+(float)Iw2DGetSurfaceWidth()*0.17f)
			{
				option = 1;
			}
			else if((s3ePointerGetX()>=(float)(Iw2DGetSurfaceWidth()-(float)Iw2DGetSurfaceWidth()*0.17f-5)&&s3ePointerGetX()<=(float)(Iw2DGetSurfaceWidth()-(float)Iw2DGetSurfaceWidth()*0.17f-5)+(float)Iw2DGetSurfaceWidth()*0.17f)&&(s3ePointerGetY()>=(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f-5)&&s3ePointerGetY()<=(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f-5)+(float)Iw2DGetSurfaceWidth()*0.17f))
			{
				option = 2;
			}
			else if(s3ePointerGetX()>=(float)Iw2DGetSurfaceWidth()/2-(float)(Iw2DGetSurfaceWidth()*0.3f)/2 && s3ePointerGetX()<=(float)Iw2DGetSurfaceWidth()/2+(float)(Iw2DGetSurfaceWidth()*0.3f)/2 && s3ePointerGetY()>=(float)Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)-20 && s3ePointerGetY()<=(float)Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)-20+(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f))
			{
				option = 3;
			}
			s3eKeyboardClearState();
		}
	}
	else
	{
		option_Win_Update();
	}
}

void CGame::play_Page_Update()
{
	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
	{
		resume=1;
		if(over==1)
		{
			if(score>highscore[0])
			{
				highscore[2] = highscore[1];
				highscore[1] = highscore[0];
				highscore[0] = score;
			}
			else if(score>highscore[1])
			{
				highscore[2] = highscore[1];
				highscore[1] = score;
			}
			else if(score>highscore[2])
			{
				highscore[2] = score;
			}
			reset();
		}
		page=0;
		trans=1;
		j=m_tran;
		s3eKeyboardClearState();
	}
	if(over==0)
	{
		score++;

		if(time_scale>=3000)
			time_scale=3000;
		else if(time_scale>=2500)
			time_scale+=0.0001f;
		else if(time_scale>=2000)
			time_scale+=0.001f;
		else if(time_scale>=1500)
			time_scale+=0.01f;
		else if(time_scale>=1000)
			time_scale+=0.1f;
		else if(time_scale>=500)
			time_scale++;
		else time_scale+=10;
		
		if(time_scale_ball<=400)
			time_scale_ball+=5;
		else time_scale_ball = 700.0f;

		if(iscaptured==true)
		{
			m_Position.y -=speed*time_scale;
			if(m_Position.x<(barpos[z].x-(m_Size.x*0.75f))||m_Position.x>(barpos[z].x+b_size.x-(m_Size.x*0.25f)))
			{
				iscaptured =false;
			}
		}
		else
		{
			m_Position.y+=b_speed*time_scale_ball;
			for(i=0;i<6;i++)
			{
				if((barpos[i].y-m_Position.y)<=(int)m_Size.x&&(barpos[i].y-m_Position.y)>=0)
				{
					if(m_Position.x>=(barpos[i].x-(m_Size.x*0.85f))&&m_Position.x<=(barpos[i].x+b_size.x-(m_Size.x*0.15f)))
					{
						iscaptured=true;
						z=i;
						m_Position.y = barpos[i].y-m_Size.y;
						break;
					}
					else
					{
						iscaptured=false;
					}
				}
				else
				{
					iscaptured=false;
				}
			}
		}

		for(i=0;i<6;i++)
		{
			barpos[i].y -=speed*time_scale;
			if(barpos[i].y<0)
			{
				barpos[i].y = (float)Iw2DGetSurfaceHeight();
				barpos[i].x=(float)(rand()%(Iw2DGetSurfaceWidth()-(int)b_size.x));
			}
		}

		if( s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN )
		{
			if(s3ePointerGetX()<=Iw2DGetSurfaceWidth()/2)
			{
				if(x>-20)
					x--;
				if(m_Position.x <= 0)
				{
					m_Position.x = 0;
					x=0;
				}
				else if(m_Position.x > Iw2DGetSurfaceWidth()-m_Size.x)
				{
					m_Position.x = (float)Iw2DGetSurfaceWidth()-m_Size.x;
					x=0;
				}
				else
				{
					m_Position.x += bh_speed*x;
				}
			}
			else
			{
				if(x<20)
					x++;
				if(m_Position.x >= Iw2DGetSurfaceWidth()-m_Size.x)
				{
					m_Position.x = (float)Iw2DGetSurfaceWidth()-m_Size.x;
					x=0;
				}
				else if(m_Position.x < 0)
				{
					m_Position.x = 0;
					x=0;
				}
				else
				{
					m_Position.x += bh_speed*x;
				}
			}
		}
		else
		{
			if(x<0)
			{
				x++;
				if(m_Position.x <= 0)
				{
					m_Position.x = 0;
					x=0;
				}
				else
				{
					m_Position.x += bh_speed*x;
				}
			}
			else if(x>0)
			{
				x--;
				if(m_Position.x >= Iw2DGetSurfaceWidth()-m_Size.x)
				{
					m_Position.x = (float)Iw2DGetSurfaceWidth()-m_Size.x;
					x=0;
				}
				else
				{
					m_Position.x += bh_speed*x;
				}
			}
		}
		if(m_Position.y>Iw2DGetSurfaceHeight()||m_Position.y<(-m_Size.x))
		{	
			life--;
			if (life==0)
			{
				over=1;
			}
			else
			{
				for(i=0;i<6;i++)
				{
					barpos[i].y=(float)(Iw2DGetSurfaceHeight()+i*(Iw2DGetSurfaceHeight()/6));
					barpos[i].x=(float)(rand()%(Iw2DGetSurfaceWidth())-(int)b_size.x*2/3);
				}
				m_Position.y = 0;
				m_Position.x = Iw2DGetSurfaceWidth()*0.5f;
				iscaptured = false;
			}
		}
	}
}

void CGame::play_Page_Update_Acc()
{
	if(s3eDeviceGetInt(S3E_DEVICE_OS) != S3E_OS_ID_WP8)
	{
		s3eDeviceBacklightOn();
	}

	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
	{
		resume=1;
		if(over==1)
		{
			if(score>highscore[0])
			{
				highscore[2] = highscore[1];
				highscore[1] = highscore[0];
				highscore[0] = score;
			}
			else if(score>highscore[1])
			{
				highscore[2] = highscore[1];
				highscore[1] = score;
			}
			else if(score>highscore[2])
			{
				highscore[2] = score;
			}
			reset();
		}
		page=0;
		trans=1;
		j=m_tran;
		s3eAccelerometerStop();
		s3eKeyboardClearState();
	}
	if(over==0)
	{
		deltaX = s3eAccelerometerGetX() - accX;
		accX += IW_FIXED_MUL(lSmoothFactor, deltaX);
				
		score++;

		if(time_scale>=3000)
			time_scale=3000;
		else if(time_scale>=2500)
			time_scale+=0.0001f;
		else if(time_scale>=2000)
			time_scale+=0.001f;
		else if(time_scale>=1500)
			time_scale+=0.01f;
		else if(time_scale>=1000)
			time_scale+=0.1f;
		else if(time_scale>=500)
			time_scale++;
		else time_scale+=10;
		
		if(time_scale_ball<=400)
			time_scale_ball+=5;
		else time_scale_ball = 700;

		if(iscaptured==true)
		{
			m_Position.y -=speed*time_scale;
			if(m_Position.x<(barpos[z].x-(m_Size.x*0.75f))||m_Position.x>(barpos[z].x+b_size.x-(m_Size.x*0.25f)))
			{
				iscaptured =false;
			}
		}
		else
		{
			m_Position.y+=b_speed*time_scale_ball;
			for(i=0;i<6;i++)
			{
				if((barpos[i].y-m_Position.y)<=(int)m_Size.x&&(barpos[i].y-m_Position.y)>=0)
				{
					if(m_Position.x>=(barpos[i].x-(m_Size.x*0.85f))&&m_Position.x<=(barpos[i].x+b_size.x-(m_Size.x*0.15f)))
					{
						iscaptured=true;
						z=i;
						m_Position.y = barpos[i].y-m_Size.y;
						break;
					}
					else
					{
						iscaptured=false;
					}
				}
				else
				{
					iscaptured=false;
				}
			}
		}

		for(i=0;i<6;i++)
		{
			barpos[i].y -=speed*time_scale;
			if(barpos[i].y<0)
			{
				barpos[i].y = (float)Iw2DGetSurfaceHeight();
				barpos[i].x=(float)(rand()%(Iw2DGetSurfaceWidth()-(int)b_size.x));
			}
		}

		if(accX < -100 || accX > 100)
		{
			if(accX<-100)
			{
				if(x>-20)
					x--;
				if(m_Position.x <= 0)
				{
					m_Position.x = 0;
					x=0;
				}
				else if(m_Position.x > Iw2DGetSurfaceWidth()-m_Size.x)
				{
					m_Position.x = (float)Iw2DGetSurfaceWidth()-m_Size.x;
					x=0;
				}
				else
				{
					m_Position.x += bh_speed*x;
				}
			}
			else
			{
				if(x<20)
					x++;
				if(m_Position.x >= Iw2DGetSurfaceWidth()-m_Size.x)
				{
					m_Position.x = (float)Iw2DGetSurfaceWidth()-m_Size.x;
					x=0;
				}
				else if(m_Position.x < 0)
				{
					m_Position.x = 0;
					x=0;
				}
				else
				{
					m_Position.x += bh_speed*x;
				}
			}
		}
		else
		{
			if(x<0)
			{
				x++;
				if(m_Position.x <= 0)
				{
					m_Position.x = 0;
					x=0;
				}
				else
				{
					m_Position.x += bh_speed*x;
				}
			}
			else if(x>0)
			{
				x--;
				if(m_Position.x >= Iw2DGetSurfaceWidth()-m_Size.x)
				{
					m_Position.x = (float)Iw2DGetSurfaceWidth()-m_Size.x;
					x=0;
				}
				else
				{
					m_Position.x += bh_speed*x;
				}
			}
		}
		if(m_Position.y>Iw2DGetSurfaceHeight()||m_Position.y<(-m_Size.x))
		{	
			life--;
			if (life==0)
			{
				over=1;
			}
			else
			{
				for(i=0;i<6;i++)
				{
					barpos[i].y=(float)(Iw2DGetSurfaceHeight()+i*(Iw2DGetSurfaceHeight()/6));
					barpos[i].x=(float)(rand()%(Iw2DGetSurfaceWidth())-(int)b_size.x*2/3);
				}
				m_Position.y = 0;
				m_Position.x = Iw2DGetSurfaceWidth()*0.5f;
				iscaptured = false;
			}
		}
	}
}

void CGame::option_Win_Update()
{
	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
		{
			if(option == 4 || option == 5 || option == 6)
			{
				option = 1;
				s3eKeyboardClearState();
			}
			else
			{
				option = 0;
				click =0;
				s3eKeyboardClearState();
			}
		}
	else if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN) && option!=0)
	{
		if(option == 1 || option == 4 || option == 5 || option == 6)
		{
			if((s3ePointerGetX()>=5&&s3ePointerGetX()<=(float)Iw2DGetSurfaceWidth()*0.17f+5)&&s3ePointerGetY()>=(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f)-5&&s3ePointerGetY()<=(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f)+(float)Iw2DGetSurfaceWidth()*0.17f)
			{
				option = 0;
				click=0;
			}
		}
		else if(option == 2)
		{
			if((s3ePointerGetX()>=(float)(Iw2DGetSurfaceWidth()-(float)Iw2DGetSurfaceWidth()*0.17f-5)&&s3ePointerGetX()<=(float)(Iw2DGetSurfaceWidth()-(float)Iw2DGetSurfaceWidth()*0.17f-5)+(float)Iw2DGetSurfaceWidth()*0.17f)&&(s3ePointerGetY()>=(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f-5)&&s3ePointerGetY()<=(float)(Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*0.17f-5)+(float)Iw2DGetSurfaceWidth()*0.17f))
			{
				option = 0;
				click=0;
			}
		}
		else if(option == 3)
		{
			if(s3ePointerGetX()>=(float)Iw2DGetSurfaceWidth()/2-(float)(Iw2DGetSurfaceWidth()*0.3f)/2 && s3ePointerGetX()<=(float)Iw2DGetSurfaceWidth()/2+(float)(Iw2DGetSurfaceWidth()*0.3f)/2 && s3ePointerGetY()>=(float)Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)-20 && s3ePointerGetY()<=(float)Iw2DGetSurfaceHeight()-(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f)-20+(float)Iw2DGetSurfaceWidth()*(0.3f/3.19f))
			{
				option = 0;
				click=0;
			}
		}
	}
	switch (option)
	{
		case 1:
			if( s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN )
			{
				if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.7f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.15f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.15f+Iw2DGetSurfaceWidth()*0.7f*0.3f)
				{
					x = -30;
					option = 4;
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.35f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.35f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.4f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.4f+Iw2DGetSurfaceWidth()*0.35f*0.55f)
				{
					option = 5;
					x = -30;
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.7f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.7f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.65f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.65f+Iw2DGetSurfaceWidth()*0.7f*0.3f)
				{
					x = -30;
					option = 6;
				}
			}
			break;
		case 2:
			if( s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN )
			{
				if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.7f/2-Iw2DGetSurfaceWidth()*0.17f*0.35f && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.7f/2-Iw2DGetSurfaceWidth()*0.17f*0.35f+Iw2DGetSurfaceWidth()*0.17f && s3ePointerGetY()>=Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2+Iw2DGetSurfaceWidth()*0.17f*0.2f && s3ePointerGetY()<=Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2+Iw2DGetSurfaceWidth()*0.17f*0.2f+Iw2DGetSurfaceWidth()*0.17f)
				{
					switch (s3eDeviceGetInt(S3E_DEVICE_OS))
					{
						case S3E_OS_ID_ANDROID:
							s3eOSExecExecute("http://www.windowsphone.com/s?appid=ab3f9c1b-68bc-43fe-ac24-231ef3d92122",false);
							break;
						case S3E_OS_ID_WP8:
							s3eOSExecExecute("zune:reviewapp?appid=appab3f9c1b-68bc-43fe-ac24-231ef3d92122",false);
							break;
						default:
							s3eOSExecExecute("http://www.windowsphone.com/s?appid=ab3f9c1b-68bc-43fe-ac24-231ef3d92122",false);
							break;
					}
					option = 0;
				}
			}
			break;
		case 4:
			if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN) && (x==0))
			{
				if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.29f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.29f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(track!=0)
					{
						track = 0;
						s3eAudioStop();
					}
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.45f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.45f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(track!=1)
					{
						track = 1;
						s3eAudioPlay("bk_music.mp3",0);
					}
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.61f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.61f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(track!=2)
					{
						track = 2;
						s3eAudioPlay("bk_music1.mp3",0);
					}
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.77f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.77f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(track!=3)
					{
						track = 3;
						s3eAudioPlay("bk_music2.mp3",0);
					}
				}
			}
			break;
		case 5:
			if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN) && x>=0)
			{
				if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.250f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(ballsel!=0)
					{
						ballsel = 0;
					}
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.375f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(ballsel!=1 && highscore[0]>=25000)
					{
						ballsel = 1;
						click = 0;
					}
					else if(highscore[0]<25000)
					{
						click = 1;
						x=100;
					}
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.500f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(ballsel!=2 && highscore[0]>=50000)
					{
						ballsel = 2;
						click = 0;
					}
					else if(highscore[0]<50000)
					{
						click = 2;
						x=100;
					}

				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.625f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(ballsel!=3 && highscore[0]>=75000)
					{
						ballsel = 3;
						click = 0;
					}
					else if(highscore[0]<75000)
					{
						click = 3;
						x=100;
					}
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.75f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(ballsel!=4 && highscore[0]>=120000)
					{
						ballsel = 4;
						click = 0;
					}
					else if(highscore[0]<120000)
					{
						click = 4;
						x=100;
					}
				}
			}
			break;
		case 6:
			if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN) && x>=0)
			{
				if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.5f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.5f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(acc != 0)
					{
						acc = 0;
					}
				}
				else if(s3ePointerGetX()>=Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetX()<=Iw2DGetSurfaceWidth()*0.5f+Iw2DGetSurfaceWidth()*0.4f/2 && s3ePointerGetY()>=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.7f && s3ePointerGetY()<=(Iw2DGetSurfaceHeight()*0.55f-Iw2DGetSurfaceWidth()*1.25f/2)+(Iw2DGetSurfaceWidth()*1.25f)*0.7f+Iw2DGetSurfaceWidth()*0.4f*0.3)
				{
					if(acc != 1)
					{
						acc = 1;
					}
				}
			}
			
			break;
	}
}

void CGame::reset()
{
	b_size.x=Iw2DGetSurfaceWidth()*0.6f;
	b_size.y=Iw2DGetSurfaceHeight()*0.03f;
	m_Position.x=(float)Iw2DGetSurfaceWidth()/2;
	m_Position.y=0;
	
	life = 3;
	score = 0;

	x = 0;
	resume = 0;
	trans = 0;
	over = 0;
	option = 0;
	
	speed = 0.002f;
	b_speed = 0.01f;
	bh_speed = 0.5f;
	iscaptured=false;
	
	for(m_tran=1;(Iw2DGetSurfaceWidth()*0.5f)-m_tran>=0;m_tran+=8){};

	j=1;

	time_scale = 1.0f;
	time_scale_ball = -3.0f;

	for(i=0;i<6;i++)
	{
		barpos[i].y=(float)(Iw2DGetSurfaceHeight()+i*(Iw2DGetSurfaceHeight()/6));
		barpos[i].x=(float)(rand()%(Iw2DGetSurfaceWidth())-(int)b_size.x*2/3);
	}
}