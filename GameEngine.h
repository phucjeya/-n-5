#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <Windows.h>
#include <string>
#include <ctime>

class GameEngine
{
private:
	void game_thread();

protected:
	bool game_status;

	int screen_width;
	int screen_height;

	CHAR_INFO *buf_screen;
	HANDLE console_output;
	HANDLE console_input;
	SMALL_RECT rect_window;

public:
	GameEngine();
	~GameEngine();

	int get_screen_width();
	int get_screen_height();
	int console(int, int, int, int);

	void clip(int&, int&);
	virtual void draw(int, int, short pixel_type = 0x2588, short col = 0x000F);
	void fill(int, int, int, int, short pixel_type = 0x2588, short col = 0x000F);

	virtual bool on_user_create() = 0;
	virtual bool on_user_update(float elapsed_time) = 0;
	virtual bool on_user_destroy() { return true; }

	void start();
};