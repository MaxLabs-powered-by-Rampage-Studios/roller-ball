#ifndef GAME_H
#define GAME_H

class CGame
{
public:
    CGame();
    ~CGame();

    void Update();

    void Render();

	void main_Page();
	void play_Page();
	void option_Win();

	void main_Page_Update();
	void play_Page_Update();
	void play_Page_Update_Acc();
	void option_Win_Update();

	void trans_Page();
	void reset();
private:
    CIwFVec2 m_Position, m_Size;
};

#endif
