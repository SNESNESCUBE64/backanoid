/*Backanoid!
 * Version 1.0 (FINAL)
 * File: scoreboard.c
 * Date: 13 March 2017
 * 
 * Matthew Schalk
 * Jay Siberski
 * 
 * Backanoid (C) Matthew Schalk/Jay Siberski, all rights belong to them. Not for resale
*/

#include <stdio.h>
#include <curses.h>
#include <string.h>
#include "scoreboard.h"

int initialized = 0;//1 if it initialized

//void displayLevel(int level);//through
//void displayScore(int score);
//void displayLives(int lives);

//
void scoreboard(int level, int score, int lives)
{
	
	displayLevel(level);
	displayLives(lives);
	displayScore(score);
	refresh();
	initialized = 0;
	
}

void displayLevel(int level)
{
	if(initialized == 0)//in case it already printed this stuff
	{
		mvprintw(0, 0, "L");
		mvprintw(0,1, "e");
		mvprintw(0,2, "v");
		mvprintw(0,3, "e");
		mvprintw(0,4, "l");
		mvprintw(0,5, ":");
	}
	
	char levelChar[2] = {0,1};
	
	if(level < 10)
	{
		sprintf(levelChar, "%d", level);
		levelChar[1] = 'x';
	}
	else//in the range of 10+
	{
		sprintf(levelChar, "%d", level);
	}
	
	if(levelChar[1] == 'x')
	{
		levelChar[1] = levelChar[0];
		levelChar[0] = ' ';
	}
	
		mvprintw(0,7, "%c", levelChar[0]);
		mvprintw(0,8, "%c", levelChar[1]);
	
}

void displayLives(int lives)
{
	if(initialized == 0)
	{
		mvprintw(0, 11, "l");
		mvprintw(0, 12, "i");
		mvprintw(0, 13, "v");
		mvprintw(0, 14, "e");
		mvprintw(0, 15, "s");
		mvprintw(0, 16, ":");	
	}
	
	mvprintw(0,18, "%d", lives);
}

void displayScore(int score)
{
	if(initialized == 0)
	{
		mvprintw(0, 21, "S");
		mvprintw(0, 22, "c");
		mvprintw(0, 23, "o");
		mvprintw(0, 24, "r");
		mvprintw(0, 25, "e");
		mvprintw(0, 26, ":");	
	}
	
	mvprintw(0,28, "%d", score);
}

