#ifndef _WINDOWCONSOLE_H_

#define _WINDOWCONSOLE_H_

#include <Windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

void DisableCtrButton(bool Close, bool Min, bool Max);

void gotoxy(short posX, short posY);

void ShowCur(bool CursorVisibility);

void setTextColor(long color);

void clrscr();

void drawRec(long x, long y, long posX, long posY);

void clear(int a, int b, int c, int d);

void DisableResizeWindow();

void ShowScrollbar(BOOL Show);

void setColor(int color);

int getColor();

void setBackgroundColor(int BackC);

void clearSearch();

#endif // !_WINDOWCONSOLE_H_
