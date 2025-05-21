
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>
#include "player.h"

#define MEM_VRAM 0x06000000
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

typedef u16 M3LINE[SCREEN_WIDTH];
#define m3_mem ((M3LINE*)MEM_VRAM)

void drawPixel(int x, int y, int color){
	m3_mem[y][x] = color;
}

void update();
void render();
void circleBres(int xc, int yc, int r, int color);

struct Entity a = {0,0,15,0x7FFF, 0, 0};

void clearScreen(){
	circleBres(a.lastX, a.lastY,a.lastRad, 0x0000);
}


int main(void) {

	irqInit();
	irqEnable(IRQ_VBLANK);

	SetMode(MODE_3 | BG2_ON);


	while (1) {
		VBlankIntrWait();
		
		update();
		
		clearScreen();
		render();
		
	}
}

void update(){
	a.lastX = a.x;
	a.lastY = a.y;
	a.lastRad = a.radius;

	scanKeys();
	int key_pressed = keysDown();
	int key_released = keysUp();

	if(key_pressed & KEY_UP) a.velY = -1;
	else if (key_pressed & KEY_DOWN) a.velY = 1;
	else if (key_released && KEY_UP && a.velY <0) a.velY = 0;
	else if (key_released && KEY_DOWN && a.velY >0) a.velY = 0;

	if(key_pressed & KEY_LEFT) a.velX = -1;
	else if(key_pressed & KEY_RIGHT) a.velX = 1;
	else if (key_released && KEY_LEFT && a.velX <0) a.velX = 0;
	else if (key_released && KEY_RIGHT && a.velX >0) a.velX = 0;

	if(key_pressed & KEY_A){
		a.radius = a.radius +1;
	}


	a.x = a.x + a.velX;
	a.y = a.y + a.velY;

	a.color = a.color+1;
	if(a.color > 0x7FFF) a.color = 0x0000;
}

void render(){
	circleBres(a.x, a.y, a.radius, a.color);
}

void drawCircle(int xc, int yc, int x, int y, int color){
    drawPixel(xc+x, yc+y, color);
    drawPixel(xc-x, yc+y, color);
    drawPixel(xc+x, yc-y, color);
    drawPixel(xc-x, yc-y, color);
    drawPixel(xc+y, yc+x, color);
    drawPixel(xc-y, yc+x, color);
    drawPixel(xc+y, yc-x, color);
    drawPixel(xc-y, yc-x, color);
}

void circleBres(int xc, int yc, int r, int color){
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y, color);
    while (y >= x){
      
        // check for decision parameter
        // and correspondingly 
        // update d, y
        if (d > 0) {
            y--; 
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;

        // Increment x after updating decision parameter
        x++;
        
        // Draw the circle using the new coordinates
        drawCircle(xc, yc, x, y, color);
    }
}



