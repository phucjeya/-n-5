#include "GameEngine.h"

using namespace std;

void GameEngine::game_thread()
{
	if (!on_user_create())
		game_status = false;

	auto t1 = chrono::system_clock::now();
	auto t2 = chrono::system_clock::now();

	while (game_status)
	{
		while (game_status)//để game hoạt động lại nhanh nhất có thể nếu game_status chuyển
		{				   //từ false về true
			t2 = chrono::system_clock::now();//xử lí thời gian
			chrono::duration<float> time = t2 - t1;
			t1 = t2;
			float elapsed_time = time.count();

			if (!on_user_update(elapsed_time))//nếu game dừng
				game_status = false;

			//tiêu đề console
			wchar_t s[256];
			swprintf_s(s, 256, L"Game - FPS: %3.2f", 1.0f / elapsed_time);
			SetConsoleTitle(s);
			WriteConsoleOutput(console_output, buf_screen, { (short)screen_width, (short)screen_height }, { 0,0 }, &rect_window);
		}
	}
}

GameEngine::GameEngine()
{
	screen_width = 80;
	screen_height = 80;
	console_output = GetStdHandle(STD_OUTPUT_HANDLE);
	console_input = GetStdHandle(STD_INPUT_HANDLE);
}

GameEngine::~GameEngine()
{
	SetConsoleActiveScreenBuffer(console_output);
	delete[] buf_screen;//hủy cấp phát động
}

int GameEngine::console(int width, int height, int font_width, int font_height)
{
	if (console_output == INVALID_HANDLE_VALUE)
		return 0;

	screen_width = width;
	screen_height = height;

	//chỉnh kích thước của console về nhỏ nhất để screenbuffer co lại
	//nhỏ hơn kích thước thật
	rect_window = { 0,0,1,1 };
	SetConsoleWindowInfo(console_output, true, &rect_window);

	//đặt giá trị kích thước của screenbuffer
	COORD coord = { (short)screen_width,(short)screen_height };
	if (!SetConsoleScreenBufferSize(console_output, coord))
		return 0;

	//chỉ định screenbuffer cho console
	if (!SetConsoleActiveScreenBuffer(console_output))
		return 0;

	//đặt giá trị kích thước font
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize = { (SHORT)font_width,(SHORT)font_height };
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	if (!SetCurrentConsoleFontEx(console_output, false, &cfi))
		return 0;

	//lấy thông tin screenbuffer để xem kích thước lớn nhất của cửa sổ
	//nếu giá trị kích thước nhập vào lớn hơn max, tự chỉnh về max
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(console_output, &csbi))
		return 0;
	if (screen_width > csbi.dwMaximumWindowSize.X)
		screen_width = csbi.dwMaximumWindowSize.X;
	if (screen_height > csbi.dwMaximumWindowSize.Y)
		screen_height = csbi.dwMaximumWindowSize.Y;

	//điều chỉnh kích thước màn hình
	rect_window = { 0,0,(short)screen_width - 1,(short)screen_height - 1 };
	if (!SetConsoleWindowInfo(console_output, true, &rect_window))
		return 0;

	//cấp phát động cho screenbuffer
	buf_screen = new CHAR_INFO[screen_width*screen_height];
	memset(buf_screen, 0, sizeof(CHAR_INFO)*screen_width*screen_height);

	return 1;//hoàn tất điều chỉnh kích thước console, font chữ
}

int GameEngine::get_screen_width()
{
	return screen_width;//lấy chiều rộng console
}

int GameEngine::get_screen_height()
{
	return screen_height;//lấy chiều dài console
}

void GameEngine::clip(int &x, int &y)
{
	//điều chỉnh giá trị x và y sao cho khác 0 và bé hơn kích thước màn hình
	if (x < 0)
		x = 0;
	if (x >= screen_width)
		x = screen_width;
	if (y < 0)
		y = 0;
	if (y >= screen_height)
		y = screen_height;
}

void GameEngine::draw(int x, int y, short pixel_type, short col)
{
	//vẽ (đặt thuộc tính cho) 1 pixel vị trí {x, y}
	if (x >= 0 and x < screen_width and y >= 0 and y < screen_height)
	{
		buf_screen[y*screen_width + x].Char.UnicodeChar = pixel_type;
		buf_screen[y*screen_width + x].Attributes = col;
	}
}

void GameEngine::fill(int x1, int y1, int x2, int y2, short pixel_type, short col)
{
	//tô 1 vùng trên console từ {x1, y1} đến {x2, y2}
	clip(x1, y1);
	clip(x2, y2);
	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++)
			draw(i, j, pixel_type, col);
}

void GameEngine::start()
{
	game_status = true;
	auto t = thread(&GameEngine::game_thread, this);
	t.join();// đợi thread t thoát ra
}