#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define RAD 5

#define BACKGROUND_WHITE (BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE)
#define AREA 40
#define idx(c,r,wi) (c + wi * r)

int distance(int x, int y, const int WIDTH, const int HEIGHT);
void circunference(CHAR_INFO consoleBuffer[], const int WIDTH, const int HEIGHT, int radius);
void pixel(CHAR_INFO consoleBuffer[],const int WIDTH,int x, int y,int color);
void loading(void);

struct dot{
	int posx;
	int posy;
}dots[AREA];

int distance(int x, int y, const int WIDTH, const int HEIGHT){
	return (sqrt((pow(x - (WIDTH/2), 2)) + pow(y - (HEIGHT/4), 2)));
}

void pixel(CHAR_INFO consoleBuffer[], const int WIDTH, int x, int y, int color){
	consoleBuffer[idx(x,y,WIDTH)] = (CHAR_INFO) {.Char.AsciiChar = ' ', .Attributes = color};
}

void circunference(CHAR_INFO consoleBuffer[], const int WIDTH, const int HEIGHT, int radius){
	int i = 0;
	for(int width_step = 0; width_step < WIDTH; width_step++){
		for(int height_step = 0; height_step < HEIGHT; height_step++){
			if(distance(width_step, height_step, WIDTH, HEIGHT) == radius){
				pixel(consoleBuffer, WIDTH, width_step, height_step, BACKGROUND_WHITE);
				dots[i].posx = width_step;
				dots[i].posy = height_step;
				i++;
			}
			else
				pixel(consoleBuffer, WIDTH, width_step, height_step, 0);
		}
	}
}



void loading(void){
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(wHnd, &csbi);

    const int WIDTH = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    const int HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	CHAR_INFO consoleBuffer[WIDTH * HEIGHT];
	COORD charBufSize = {WIDTH, HEIGHT};
	COORD characterPos = {0, 0};
	SMALL_RECT writeArea = {0, 0, WIDTH - 1, HEIGHT - 1};
	COORD bufferSize = {WIDTH, HEIGHT};

	SetConsoleTitle(TEXT("LOADING..."));

	circunference(consoleBuffer, WIDTH, HEIGHT, RAD);
	
	for(int i = 0; i < (5 + rand() % 15); i++){
		int arr[AREA] = {6,5,4,3,2,1,0,8,7,12,11,15,17,19,21,23,25,26,29,30,33,34,35,36,37,38,39,31,32,27,28,24,22,20,18,16,14,13,10,9};
		for(int i = 0; i < AREA; i++){
			pixel(consoleBuffer, WIDTH, dots[arr[i]].posx, dots[arr[i]].posy, 0);
			if(i+1 < AREA)
				pixel(consoleBuffer, WIDTH, dots[arr[i+1]].posx, dots[arr[i+1]].posy, 0);
			WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
			Sleep(5);
			pixel(consoleBuffer, WIDTH, dots[arr[i]].posx, dots[arr[i]].posy, BACKGROUND_WHITE);
			if(i+1 < AREA)
				pixel(consoleBuffer, WIDTH, dots[arr[i+1]].posx, dots[arr[i+1]].posy, BACKGROUND_WHITE);
			WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
		}
		Sleep(5);
	}
	for(int i = 0; i < HEIGHT * WIDTH; i++){
		consoleBuffer[i] = (CHAR_INFO) {.Char.AsciiChar = ' ', .Attributes = 0};
	}
}