#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define RAD 5

#define CX (canvas.WIDTH/2)
#define CY (canvas.HEIGHT/4)
#define BACKGROUND_WHITE (BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE)
#define AREA (RAD * 8)// 8 = all the possible spaces around one pixel.
#define idx(c,r) (c + canvas.WIDTH * r)//equation to transform a simple array into a multi-dimensional array.

void circunference(int radius, CHAR_INFO consoleBuffer[]);
void pixel(int x, int y,int color, CHAR_INFO consoleBuffer[]);
void setbcolor(int color, CHAR_INFO consoleBuffer[]);
void line(int x1, int x2, int y1, int y2, CHAR_INFO consoleBuffer[]);
int distanceL(int x, int y,int x1, int x2, int y1, int y2);
int distanceC(int x, int y,int cx, int cy);
void loading(void);

struct dot{
	int posx;
	int posy;
}dot[2];

struct canvas{
	int HEIGHT;
	int WIDTH;
}canvas;

void setbcolor(int color, CHAR_INFO consoleBuffer[]){
	for(int clear = 0; clear < canvas.HEIGHT * canvas.WIDTH; clear++)
		consoleBuffer[clear] = (CHAR_INFO) {.Char.AsciiChar = ' ', .Attributes = color};
}

int distanceC(int x, int y, int cx, int cy){
	return (sqrt((pow(x - cx, 2)) + pow(y - cy, 2)));
}

int distanceL(int x, int y,int x1, int x2, int y1, int y2){
	return ((y1-y2)*x + (x2 - x1)*y + x1*y2 - x2*y1);
}

void pixel(int x, int y, int color, CHAR_INFO consoleBuffer[]){
	consoleBuffer[idx(x,y)] = (CHAR_INFO) {.Char.AsciiChar = ' ', .Attributes = color};
}

void line(int x1, int y1, int x2, int y2, CHAR_INFO consoleBuffer[]){
	for(int width_step = (x1 > x2 ? x2 : x1); width_step <= (x1 > x2 ? x1 : x2); width_step++){
		for(int height_step = (y1 > y2 ? y2 : y1); height_step <= (y1 > y2 ? y1 : y2); height_step++){
			if(distanceL(width_step,height_step,x1,x2,y1,y2) == 0)
				pixel(width_step, height_step, BACKGROUND_GREEN, consoleBuffer);
		}
	}
	pixel(x1, y1, BACKGROUND_BLUE,consoleBuffer);
	pixel(x2, y2, BACKGROUND_RED,consoleBuffer);
}

void circunference(int radius, CHAR_INFO consoleBuffer[]){
	for(int width_step = 0; width_step < canvas.WIDTH; width_step++){
		for(int height_step = 0; height_step < canvas.HEIGHT; height_step++){
			if(distanceC(width_step, height_step,CX,CY) == radius){
				pixel(width_step, height_step, BACKGROUND_WHITE, consoleBuffer);
				if(width_step == CX && height_step == CY + radius){
					dot[0].posx = width_step;
					dot[0].posy = height_step;
				}
			}
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
	canvas.HEIGHT = HEIGHT;
	canvas.WIDTH = WIDTH;

	setbcolor(0,consoleBuffer);
	circunference(RAD,consoleBuffer); 
	int px = dot[0].posx, py = dot[0].posy;
	int ox = CX, oy = CY;
	int c,s,dx,dy;
	float angleR;
	for(;;){
		for(int angleD = 0; angleD < 360; angleD++){
			angleR = (angleD * (M_PI/180));
			c = cos(angleR);
			s = sin(angleR);
			dx = ceill((cos(angleR) * (px-ox)) - (sin(angleR) * (py-oy)) + ox + 0.5);
			dy = ceill((sin(angleR) * (px-ox)) + (cos(angleR) * (py-oy)) + oy + 0.5);
			pixel(dx,dy,BACKGROUND_GREEN,consoleBuffer);
			WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
			Sleep(5);
			setbcolor(0,consoleBuffer);
			circunference(RAD,consoleBuffer);
		}
	}
	system("cls");
}