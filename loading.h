#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define RAD 5

#define CX (canvas.WIDTH / 2)
#define CY (canvas.HEIGHT / 4)
#define BACKGROUND_WHITE (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BACKGROUND_GRAY (BACKGROUND_RED + BACKGROUND_RED)
#define AREA (RAD * 8)					 // 8 = all the possible spaces around one pixel.
#define idx(c, r) (c + canvas.WIDTH * r) // equation to transform a simple array into a multi-dimensional array.

void circunference(int radius, CHAR_INFO consoleBuffer[]);
void pixel(int x, int y, int color, CHAR_INFO consoleBuffer[]);
void setbcolor(int color, CHAR_INFO consoleBuffer[]);
void line(int x1, int x2, int y1, int y2, int color, CHAR_INFO consoleBuffer[]);
void loading_txt(CHAR_INFO consoleBuffer[]);
int distanceL(int x, int y, int x1, int x2, int y1, int y2);
int distanceC(int x, int y, int cx, int cy);
void loading(void);

struct dot{
	int posx;
	int posy;
} dots[AREA];

struct canvas{
	int HEIGHT;
	int WIDTH;
} canvas;

void setbcolor(int color, CHAR_INFO consoleBuffer[]){//Changes back color
	for (int clear = 0; clear < canvas.HEIGHT * canvas.WIDTH; clear++)
		consoleBuffer[clear] = (CHAR_INFO){.Char.AsciiChar = ' ', .Attributes = color};
}

int distanceC(int x, int y, int cx, int cy){//Pitagoras.
	return (sqrt((pow(x - cx, 2)) + pow(y - cy, 2)));
}

int distanceL(int x, int y, int x1, int x2, int y1, int y2){//Line matriz.
	return ((y1 - y2) * x + (x2 - x1) * y + x1 * y2 - x2 * y1);
}

void pixel(int x, int y, int color, CHAR_INFO consoleBuffer[]){//Puts a pixel.
	consoleBuffer[idx(x, y)] = (CHAR_INFO){.Char.AsciiChar = ' ', .Attributes = color};
}

void line(int x1, int y1, int x2, int y2, int color,CHAR_INFO consoleBuffer[]){//Draws a line
	for (int width_step = (x1 > x2 ? x2 : x1); width_step <= (x1 > x2 ? x1 : x2); width_step++){
		for (int height_step = (y1 > y2 ? y2 : y1); height_step <= (y1 > y2 ? y1 : y2); height_step++){
			if (distanceL(width_step, height_step, x1, x2, y1, y2) >= -3 && distanceL(width_step, height_step, x1, x2, y1, y2) <= 3)
				pixel(width_step, height_step, color, consoleBuffer);
		}
	}
}

void circunference(int radius, CHAR_INFO consoleBuffer[]){//Draws a circle
	int i = 0;
	for (int width_step = 0; width_step < canvas.WIDTH; width_step++){
		for (int height_step = 0; height_step < canvas.HEIGHT; height_step++){
			if (distanceC(width_step, height_step, CX, CY) == radius){
				pixel(width_step, height_step, BACKGROUND_WHITE, consoleBuffer);
				dots[i].posx = width_step;
				dots[i].posy = height_step;
				i++;
			}
		}
	}
}

void loading_txt(CHAR_INFO consoleBuffer[]){//LOADING text
	//OUTLINES
	line(0,0,canvas.WIDTH,0,BACKGROUND_WHITE,consoleBuffer);
	line(0,canvas.HEIGHT-1,canvas.WIDTH,canvas.HEIGHT-1,BACKGROUND_WHITE,consoleBuffer);
	line(0,0,0,canvas.HEIGHT-1,BACKGROUND_WHITE,consoleBuffer);
	line(canvas.WIDTH-1,0,canvas.WIDTH-1,canvas.HEIGHT-1,BACKGROUND_WHITE,consoleBuffer);
	//mini canvas setup
	int letterSize = 3;//3x3
	int letterQuant = 7;//L=1 O=2 ...
	int miniCanvasW[2] = {CX - 13, CX + 13};//4 letter + blank row for each side (=13) 
	int miniCanvasH[2] = {(CY + RAD + 1)+1, (CY + RAD + 1 + letterSize)};
	int i = 0;
	for (int width_step = miniCanvasW[0]; width_step <= miniCanvasW[1]; width_step++){
		i++;
		for (int height_step = miniCanvasH[0]; height_step <= miniCanvasH[1]; height_step++){
			pixel(width_step, height_step, BACKGROUND_WHITE, consoleBuffer);//Letter are going to be "written" by their negative space.
			if(i % 4 == 0)
				pixel(width_step, height_step, 0, consoleBuffer);//Blank lines between letters
		}
	}
	
//L
	pixel(miniCanvasW[0] + 1,miniCanvasH[0],0,consoleBuffer);
	pixel(miniCanvasW[0] + 2,miniCanvasH[0],0,consoleBuffer);
	pixel(miniCanvasW[0] + 1,miniCanvasH[0]+1,0,consoleBuffer);
	pixel(miniCanvasW[0] + 2,miniCanvasH[0]+1,0,consoleBuffer);
	miniCanvasW[0]+=letterSize+1;
//O
	pixel(miniCanvasW[0]+1, miniCanvasH[0]+1, 0,consoleBuffer);
	miniCanvasW[0]+=letterSize+1;
//A
	pixel(miniCanvasW[0],miniCanvasH[0],0,consoleBuffer);
	pixel(miniCanvasW[0] + 2,miniCanvasH[0],0,consoleBuffer);
	pixel(miniCanvasW[0] + 1,miniCanvasH[0]+ 2,0,consoleBuffer);
	miniCanvasW[0]+=letterSize+1;
//D
	pixel(miniCanvasW[0]+ 1,miniCanvasH[0]+1,0,consoleBuffer);
	pixel(miniCanvasW[0] + 2,miniCanvasH[0],0,consoleBuffer);
	pixel(miniCanvasW[0] + 2,miniCanvasH[0]+ 2,0,consoleBuffer);
	miniCanvasW[0]+=letterSize+1;
//I
	pixel(miniCanvasW[0],miniCanvasH[0]+1,0,consoleBuffer);
	pixel(miniCanvasW[0]+ 2,miniCanvasH[0]+1,0,consoleBuffer);
	miniCanvasW[0]+=letterSize+1;
//N
	pixel(miniCanvasW[0]+ 1,miniCanvasH[0]+1,0,consoleBuffer);
	pixel(miniCanvasW[0]+ 1,miniCanvasH[0]+2,0,consoleBuffer);
	miniCanvasW[0]+=letterSize+1;
//G	
	pixel(miniCanvasW[0]+2,miniCanvasH[0],0,consoleBuffer);
	pixel(miniCanvasW[0]+ 1,miniCanvasH[0]+1,0,consoleBuffer);
}

void loading(void){//makes the loop
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(wHnd, &csbi);

	const int WIDTH = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	canvas.WIDTH = WIDTH;
	const int HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	canvas.HEIGHT = HEIGHT;

	CHAR_INFO consoleBuffer[WIDTH * HEIGHT];
	COORD charBufSize = {WIDTH, HEIGHT};
	COORD characterPos = {0, 0};
	SMALL_RECT writeArea = {0, 0, WIDTH - 1, HEIGHT - 1};
	COORD bufferSize = {WIDTH, HEIGHT};
	SetConsoleTitle(TEXT("LOADING..."));
	setbcolor(0,consoleBuffer);
	loading_txt(consoleBuffer);
	circunference(RAD,consoleBuffer);
	
	for(;;){
		int arr[AREA] = {6,5,4,3,2,1,0,8,7,12,11,15,17,19,21,23,25,26,29,30,33,34,35,36,37,38,39,31,32,27,28,24,22,20,18,16,14,13,10,9};//Rearrenging the sequence.
		for(int i = 0; i < AREA; i++){
			pixel(dots[arr[i]].posx, dots[arr[i]].posy, 0,consoleBuffer);
            if(i-1 >= 0)
				pixel(dots[arr[i-1]].posx, dots[arr[i-1]].posy, BACKGROUND_GRAY,consoleBuffer);
			if (i == 0)
                pixel(dots[arr[AREA-1]].posx, dots[arr[AREA-1]].posy, BACKGROUND_GRAY,consoleBuffer);
            WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
			if(i >= 5 && i <= 17)
				Sleep(10 * i);
			else if(i >= 18 && i <= 22)
				Sleep(arr[i]/10);
			else if(i >=23 && i <= 26)
				Sleep(10);
			else
				Sleep(5);
			pixel(dots[arr[i]].posx, dots[arr[i]].posy, BACKGROUND_GRAY,consoleBuffer);
			if(i-1 >= 0)
				pixel(dots[arr[i-1]].posx, dots[arr[i-1]].posy, BACKGROUND_WHITE,consoleBuffer);
            if (i == 0)
                pixel(dots[arr[AREA-1]].posx, dots[arr[AREA-1]].posy, BACKGROUND_WHITE,consoleBuffer);
            WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
		}
		Sleep(5);
	}
}
