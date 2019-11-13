#pragma once
#include "GameEngine.h"
#include "Object.h"

class Game :public GameEngine
{
private:
	Object player1;
	Object player2;
	Object map;
	Object ball;

	bool multiplayer;

	void goto_xy(int, int);
	void show_result(bool);
	void menu();

	void fill_map();
	void player_moving(float);
	void npc_moving(float);
	void update_ball_position(float);
	bool ball_moving(float);

	bool on_user_create();
	bool on_user_update(float);

public:
	Game();
	~Game();

};

