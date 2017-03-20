/*Backanoid!
 * Version 1.0 (FINAL)
 * File: arrow.c
 * Date: 13 March 2017
 * 
 * Matthew Schalk
 * Jay Siberski
 * 
 * Backanoid (C) Matthew Schalk/Jay Siberski, all rights belong to them. Not for resale
*/

#include <stdio.h>
#include <ncurses.h>

int getArrow()
{
	raw();//keeps the paddle from moving before the ball starts
	int ch = 0;
	keypad(stdscr, TRUE);//allows you to access the arrow keys

	ch = getch();//get the keyboard

	return ch;
}
