#include "Game.h"

using namespace std;

Game::Game()
{

}

Game::~Game()
{

}

bool Game::on_user_create()
{
	//hiển thị menu
	menu();
	//đặt kích thước map
	map.width = screen_width;
	map.height = screen_height;

	//đặt thuộc tính của 2 players
	player1.width = 15;
	player1.x = (int)((map.width - player1.width) / 2);
	player1.y = map.height - 2;
	player2.width = 15;
	player2.x = (int)((map.width - player2.width) / 2);
	player2.y = 1;

	//đặt thuộc tính ban đầu của banh
	ball.x = (int)(map.width / 2);
	ball.y = map.height - 3;
	ball.vy = -25.0;
	//chọn ngẫu nhiên hướng phát banh
	//qua bên phải nếu random được 1, bên trái nếu 0
	srand(time(NULL));
	if (rand() % 2)
		ball.vx = 25.0;
	else
		ball.vx = -25.0;

	return true;
}

bool Game::on_user_update(float elapsed_time)
{
	fill_map();
	player_moving(elapsed_time);
	return ball_moving(elapsed_time);//nếu banh ngừng di chuyển, game kết thúc
}

void Game::menu()
{
	string m[12];
	m[0] = "**** * *    *  **  *    ****";
	m[1] = "*    * * *  * *    *    *   ";
	m[2] = "**** * *  * * * ** *    ****";
	m[3] = "   * * *   ** *  * *    *   ";
	m[4] = "**** * *    *  **  **** ****";
	m[5] = "                            ";
	m[6] = "                            ";
	m[7] = "*   * *  * *    *****   *";
	m[8] = "** ** *  * *      *     *";
	m[9] = "* * * *  * *      *     *";
	m[10] = "*   * *  * *      *     *";
	m[11] = "*   * **** ****   *     *";

	for (int i = 0; i < 12; i++)
	{
		goto_xy(25, 44 + i);
		cout << m[i] << '\n';
	}

	//ấn M để chơi 2 người, S để chơi với máy
	while (1)
	{
		if (GetAsyncKeyState(0x0053) & 0x8000)
		{
			multiplayer = false;
			break;
		}
		if (GetAsyncKeyState(0x04D) & 0x8000)
		{
			multiplayer = true;
			break;
		}
	}
}

void Game::fill_map()
{
	//hiển thị map
	fill(0, 0, map.width, map.height, 0x2588, 0x000B);
	for (int i = 0; i < map.width; i++)
	{
		for (int j = 0; j < map.height; j++)
		{
			if (i == 0 || j == 0 || i == map.width - 1 || j == map.height - 1)
				draw(i, j, 0x2588, 0x000F);
		}
	}
}

void Game::player_moving(float elapsed_time)
{
	//player 1, phím mũi tên trái và phải để di chuyển
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (player1.x > 2)
			player1.x -= 60.0f*elapsed_time;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (player1.x < map.width - 1 - player1.width)
			player1.x += 60.0f*elapsed_time;
	}
	for (int i = 0; i < player1.width; i++)
		draw(player1.x + i, player1.y, 0x2588, 0x0001);

	//nếu là chế độ 2 người
	if (multiplayer)
	{
		//player 2, phím a và d để di chuyển
		if (GetAsyncKeyState(0x0041) & 0x8000)
		{
			if (player2.x > 2)
				player2.x -= 60.0f*elapsed_time;
		}
		if (GetAsyncKeyState(0x0044) & 0x8000)
		{
			if (player2.x < map.width - 1 - player2.width)
				player2.x += 60.0f*elapsed_time;
		}
		for (int i = 0; i < player2.width; i++)
			draw(player2.x + i, player2.y, 0x2588, 0x0005);
	}
	//nếu là chế độ chơi đơn
	else
		npc_moving(elapsed_time);
}

bool Game::ball_moving(float elapsed_time)
{
	//vị trí mới của banh
	int new_x = ball.x + (ball.vx*elapsed_time);
	int new_y = ball.y + (ball.vy*elapsed_time);
	bool change = false;//nếu đổi hướng (vx hoặc vy đổi dấu)
	bool increase_speed = false;//nếu chạm người chơi, tăng tốc
	//banh nằm ở góc dưới map
	if (new_y >= map.height - 2)
	{
		//nếu người chơi k đỡ đc
		if (new_x < player1.x || new_x > player1.x + player1.width)
		{
			show_result(0);
			return false;
		}
		//nếu nguời chơi đỡ đc
		else
		{
			change = true;//đổi hướng

			if (ball.vx < 75.0 and ball.vx > -75.0)
				increase_speed = true;

			ball.vy = -ball.vy;
			update_ball_position(elapsed_time);
		}
	}
	//nếu banh nằm ở góc trên
	else if (new_y <= 1)
	{
		//người chơi k đỡ đc
		if (new_x < player2.x || new_x > player2.x + player2.width)
		{
			show_result(1);
			return false;
		}
		//đỡ đc
		else
		{
			change = true;

			if (ball.vx < 75.0 and ball.vx > -75.0)
				increase_speed = true;// tăng tốc độ banh

			ball.vy = -ball.vy;
			update_ball_position(elapsed_time);
		}
	}

	//nếu chạm vách tường bên trái hoặc phải
	if (new_x == 0 || new_x == map.width - 1)
	{
		change = true;//đổi hướng
		ball.vx = -ball.vx;
		update_ball_position(elapsed_time);
	}

	//nếu k đổi hướng
	if (!change)
		update_ball_position(elapsed_time);

	//nếu chạm người chơi
	if (increase_speed)
	{
		ball.vx *= 1.1;
		ball.vy *= 1.1;
	}
	draw(ball.x, ball.y, 0x2588, 0x000C);
}

void Game::npc_moving(float elapsed_time)
{
	//gia tốc di chuyển của npc
	srand(time(NULL));
	int max_a = 15;
	int min_a = 12;
	float ax = rand() % (max_a - min_a) + min_a;
	//gia tốc npc dựa theo tốc độ, hướng véc tơ vận tốc vx của banh
	player2.vx = ax * (ball.vx / 15.0);
	player2.x += player2.vx*elapsed_time;
	//chạm vách tường
	if (player2.x < 1)
		player2.x = 1;
	if (player2.x > map.width - player2.width)
		player2.x = map.width - player2.width - 1;
	for (int i = 0; i < player2.width; i++)
		draw(player2.x + i, player2.y, 0x2588, 0x0005);
}

void Game::update_ball_position(float elapsed_time)
{
	//cập nhật tọa độ mới của banh
	ball.x += (ball.vx*elapsed_time);
	ball.y += (ball.vy*elapsed_time);
}

void Game::goto_xy(int x, int y)
{
	//di chuyển con trỏ đến {x, y}
	SetConsoleCursorPosition(console_output, { (SHORT)x, (SHORT)y });
}

void Game::show_result(bool player1_win)
{
	//hiển thị người chiến thắng
	string res[5];
	if (player1_win)
	{
		res[0] = "***  *      *   *   * **** ***    *   *     * * *   *";
		res[1] = "*  * *     * *   * *  *    *  *  **   *     * * **  *";
		res[2] = "***  *    *****   *   **** ***    *   *     * * * * *";
		res[3] = "*    *    *   *   *   *    *  *   *   *  *  * * *  **";
		res[4] = "*    **** *   *   *   **** *  *   *    *   *  * *   *";
	}
	else
	{
		res[0] = "***  *      *   *   * **** ***    ****   *     * * *   *";
		res[1] = "*  * *     * *   * *  *    *  *      *   *     * * **  *";
		res[2] = "***  *    *****   *   **** ***    ****   *     * * * * *";
		res[3] = "*    *    *   *   *   *    *  *   *      *  *  * * *  **";
		res[4] = "*    **** *   *   *   **** *  *   ****    *   *  * *   *";
	}
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < res[i].size(); j++)
		{
			if (res[i][j] == '*')
				draw(13 + j, 40 + i, 0x2588, 0x0000);
		}
}