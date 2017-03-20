/*Backanoid!
 * Version 1.0 (FINAL)
 * File: main.c
 * Date: 15 March 2017
 * 
 * Matthew Schalk
 * Jay Siberski
 * 
 * Do not distribute until after 01 April 2017, confidential (school project)!
 * Backanoid (C) Matthew Schalk/Jay Siberski, all rights belong to them. Not for resale
 * 
 * update: konami code, bug fixes, intro annimation, end game function
*/

#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

//the include files for my other .c files
#include "scoreboard.h"//functions for the scroeboard
#include "arrow.c"//I know you aren't supposed to import a .c file, but this is the easiest way get it to work
#include "set_ticker.h"//the one function for the ticker

//all the methods used
void makeWall();//establishes the walls boundries
void moveBall(int[],int[]);//moves the ball in the direction we need it to go
void ball(int);//handles all the ball's movements
int checkForCollision(int, int);//check to see if the ball hits something
int changeDirection();//changes the direction based on a collision value
void makeLevel();//assigns all the values for each display value accordingly by each block
void makeBlock();//prints all the blocks to the screen
void removeBlock(int, int);//removes the block at a particular coordinate when
void readyGoMessage();//does the ready go annimation
void quitGame(int);//exits out of the game
void handleKey(int);//handles the user input during the game, designed to recognize <-,->, q, p
void levelReset();//resets everything such as the ball position back to default for the next level
void movePaddle(int);//moves the paddle left and right accordingly
void death();//handles what happens when you lose a life
void pauseGame();//allows the user to suspend all ball and paddle movement
	
//display stuff
int displayRowMax = 18;//set the max amount of rows here
int displayColMax = 36;//set the max amount of columns here
int display[18][36];//using the same numbers for r/c, make it [row][col]
/*display values (used for ball calculations)
 * 0 = empty space
 * 1 = wall
 * 2 = block
 * 3 = paddle
*/

//scoreboard variables
int lives = 3;//live counter, goes until the score reaches zero
int score = 0;//increments by 25 or so (not sure, perhaps a larger score based on rows)
int level = 1;//one by default, increments by one at a time each time the stage is cleared.
#define MAXLEVEL 9//the highest level, done before looping back to level one


//ball stuff
#define SPEED 200//use a delay to determine the speed, (200ms delay by default, potential to increase based on row block hit))

int currentRow = 16;//default row
int currentCol = 17;//default column
int collision = 0;//used to determine if there is a colision with 
int direction = 2;//up right by default
/*direction variable values - assume the direction changes in terms of col/row in increments of 1 unless specified otherwise
 * 1 = up left
 * 2 = up right
 * 3 = down left
 * 4 = down right
 * 5 = up (slope = 2) left
 * 6 = up (slope = 2) right
 * 7 = down (slope = -2) left
 * 8 = down (slope = -2) right
*/


//block stuff
int numberOfBlocks = 0;// zero by default, set a number later
int oneupcounter = 0;
int paddleRow[36];//an array of dedicated to storing whether or not there is a block there
int currentPaddleLeft =16;//the left position for the paddle

//idea: for game over screen, perhaps make something for high scrores. save them to a .csv file for safe keeping.

int main()
{
	initscr();//initialize the screen
	crmode();
	noecho();//turn off echo so no keyboard display
	curs_set(0);//turn the cursor off, no need and it takes away from the gameplay
	signal(SIGINT, quitGame);//make ctrl c end the game gracefully
	
	while(1)
	{
		clear();//clear the screen junk
		lives = 3;
		score = 0;
		level = 1;
		direction = 2;
		currentRow = 16;
		currentCol = 17;
		
		//title screen
		mvprintw(0,0,"Welcome to Backanoid! Coded by Matthew Schalk and Jay Siberski");
		mvprintw(2,0,"Controls:");
		mvprintw(3,0, "----------------------------------------------");
		mvprintw(4,0,"<- arrow key moves the paddle left");
		mvprintw(5,0,"-> arrow key moves the paddle right");
		mvprintw(6,0, "press \"p\" to pause the game");
		mvprintw(8,0, "press \"q\" to quit at any time");
		mvprintw(7,0, "press \"e\" to end the game at any time");
		mvprintw(9,0, "----------------------------------------------");
		mvprintw(10,0, "each block (==) is worth 25 point");
		mvprintw(11,0, "every 5000 points result in an extra life");
		mvprintw(12,0, "miss the ball, you lose a life,\nlose all your lives, the game ends");
		
		mvprintw(15,0, "Press \"Enter\" to start!");
		refresh();
		
		//wait for user input
		char codeCompare[10] = {'u','u','d','d','l','r','l','r','b','a'};//compares against the cheat code
		char code[10];//the cheat code
		int counter = 0;//index for cheat code
		int konami = 0;//level select boolean
		int inputCodeCounter = 0;
		char titleInput = 0;//the input recieved from getch
		
		while(1)//go until q or enter is pressed
		{
			titleInput = getArrow();//get the keyboard input
			refresh();
			
			if(titleInput == 'q')
				quitGame(0);
			if(titleInput == '\n')
				break;
			
			if((titleInput == 2) && (counter < 10))					//switch statement crashes...
				code[counter] = 'd';//down
			else if((titleInput == 3)&& (counter < 10))
				code[counter] = 'u';//up
			else if((titleInput == 4)&& (counter < 10))
				code[counter] = 'l';//left
			else if((titleInput == 5)&& (counter < 10))
				code[counter] = 'r';//right
			else if((titleInput == 'a')&& (counter < 10))
				code[counter] = 'a';//a
			else if((titleInput = 'b')&& (counter < 10))
				code[counter] = 'b';//b
			else
			{
				//do nothing
			}
			
			counter = counter + 1;
			
			if(counter == 10)
			{
				int konami = 1;//the konami code
				
				for(int k = 0; k < 10;k++)
				{
					if(code[k] != codeCompare[k])
					{
						konami = 0;//not the code
					}
				}
				if(konami == 1)//if the code is correct
				{
					clear();
					curs_set(1);
					mvprintw(0,0, "What level would you like to go to? (1-9)");
					refresh();
					int end  = 1;
					
					while( end == 1)
					{
						char levelSelectChar = getArrow();
						int levelSelect = (int)levelSelectChar - (int)48;
						if((levelSelect >=1) && (levelSelect <= MAXLEVEL))
						{
							level = levelSelect;
							end = 0;
						}
						else{
							mvprintw(0,0, "Try again\nWhat level would you like to go to? (1-9)");
						}
					}
					curs_set(0);
					break;
				}
				refresh();
			}
			refresh();
		}
		
		
		if(titleInput == 'q')//quit game if the user pressed q
		{
			quitGame(0);//quit game with the bad ending
		}
		
		clear();//clear the title screen
		mvprintw(0,0, "A runabout?");
		refresh();
		sleep(1);
		mvprintw(1,0, "I'LL STEAL IT!");
		refresh();
		sleep(1);
		mvprintw(2,0, "NO ONE WILL EVER KNOW!");
		refresh();
		sleep(5);
		clear();
		
		for(int i = 0; i < displayRowMax; i++)//initialize display by getting rid of all those nasty null values
		{
			for(int j =0; j < displayColMax; j++)
				display[i][j] = 0;//everything is 0
		}
	
		scoreboard(level,score,lives);	//initialize the scoreboard (level, score, lives)
		makeWall();//add the coordinates and print the wall
		makeLevel();//add the first level
		movePaddle(2);//initialize the paddle
		mvprintw(16, 17, "o");//set print the ball to default
		refresh();//show everything
		readyGoMessage();//let the user its time to go
		
		signal(SIGALRM, ball);//every time the ticker goes off, move the ball
		set_ticker(SPEED);//200 milliseconds
		int keyboardIn = 0;//the keyboard input
		while(lives != 0)//run until user prompt or game over, 6 is the input prompts
		{	
			keyboardIn = getArrow();//get the user keyboard input
			if(lives >0)
				handleKey(keyboardIn);//process it accordingly
	//		refresh();//refresh the screen
		}
	}
//--------------------------------------------------------------------------------------------------	
	quitGame(1);//should never get to this point, but just in case
	
	return 0;//end the program on a good note
}



void makeWall()//establishes the walls boundries
{
	for(int i = 0; i < displayColMax;i++)//traverse for the cieling,i is index
	{
		display[2][i] = '1';//this is the ceiling
		mvprintw(1, i, "_"); //print the ceiling
	}
	
	for(int i = 2; i < displayRowMax; i++)//traverse for the walls, i is index, start at one because the top was already made
	{
		display[i][0] = '1';//the left wall
		mvprintw(i, 0, "|");//print the left wall
		display[i][displayColMax - 1] = '1';//the right wall, max distance minus one so it can be used for an index
		mvprintw(i, displayColMax-1, "|");//print the right wall
	}
	
}

void ball(int s)
{
	collision = 0;//resets to zero just in case
	
	int oldPosition[2] = {currentRow, currentCol};//stores the original position for reference
	int newPosition[2];//initialize the new position, then calculate it accordingly
	
	if(direction == 1)//up left
	{
		newPosition[0] = currentRow -1;
		newPosition[1] = currentCol -1;
	}
	else if(direction == 2)//up right
	{
		newPosition[0] = currentRow -1;
		newPosition[1] = currentCol +1;
	}
	else if(direction == 3)//down left
	{
		newPosition[0] = currentRow +1;
		newPosition[1] = currentCol -1;
	}
	else if(direction == 4)//down right
	{
	    newPosition[0] = currentRow +1;
		newPosition[1] = currentCol +1;
	}
	
	
		
	collision = checkForCollision(newPosition[0], newPosition[1]);//check to see if it collides
    
    if(collision == 0)//if there is no collision, move
		moveBall(oldPosition, newPosition);
	else if(collision == 6)//if you miss, lose a life
	{
		death();
	}		
	else//handle the collision appropriately
	{
		direction = changeDirection(direction, collision);//change the direction
		ball(0);//does nothing with s so whatever, call it again so that it still moves in the same heartbeat
	}

	refresh();
	
	if(oneupcounter < 13)
	{
		oneupcounter++;
	}
	
	else
	{
		mvprintw(19,14, "     ");
	}

	if(lives != 0)
		levelReset();//check for end of level, if so, reset
}


int changeDirection(/*int direction, int collision*/)
{
	int outDir = direction;
	if((direction == 1) && (collision == 3))//up left and hits a cieling or hits a block
	{
		outDir = 3;
	}
	else if((direction == 1) && (collision == 2))//up left and left wall
	{
		outDir = 2;
	}
	
	else if((direction == 2) && (collision == 3))//up right and hits the cieling or hits a block
	{
		outDir = 4;
	}
	else if((direction == 2) && (collision == 1))//up right and hits the right wall
	{
		outDir = 1;
	}
	
	else if((direction == 3) && (collision == 5))//down left and hits a block or paddle
	{
		outDir = 1;
	}
	
	else if((direction == 3) && (collision == 2))
	{
		outDir = 4;  
	}
	
	else if((direction == 4) && (collision == 5))
	{
		outDir = 2;
	}
	else if((direction == 4) && (collision == 1))
	{
		outDir = 3;
	}
	
	else if((direction ==1) && (collision == 4))
	{
		outDir = 3;
	}
	
	else if((direction == 2) && (collision == 4))
	{
		outDir = 4;
	}
	
	else if((direction == 3) && (collision == 4))
	{
		outDir = 1;
	}
	
	else if((direction == 4) && (collision == 4))
	{
		outDir = 2;
	}
	
	else
	{
		//do nothing
	}
	
	return outDir;
}

int checkForCollision(int newPosRow, int newPosCol)
{
	collision = 0;//determines if the ball collides with an object
	if(newPosCol == 35)//if it hits the right wall
	{
		collision = 1;//hit right wall
	}
	
	else if(newPosCol == 0)//if it hits the left wall
	{
		collision = 2;
	}
	
	else if(newPosRow == 1)//if it hits the cieling
	{
		collision = 3;
	}
	
	else if(display[newPosRow][newPosCol] == 2)//if it hits a block
	{
		collision = 4;
		removeBlock(newPosRow, newPosCol);
		score = score + 25;
		
		if((((score % 5000) == 0) && (score > 0))&& (lives < 10))
		{
			lives = lives + 1;
			mvprintw(19,14, "1-Up!");
			oneupcounter = 0;
		}

		
		scoreboard(level,score,lives);
	}
	
	else if(newPosRow == 17)//if it hits the paddle, fix this
	{
		//check for paddle, if not make collision = 6
		if((paddleRow[newPosCol + 1] == 1) && (direction == 3))//find out if it is above the paddle rather than the new position
		{
			collision = 5;
		}
		else if((paddleRow[newPosCol - 1] == 1) && (direction == 4))//same thing as before except for the right
		{
			collision = 5;
		}
		else//you die
		{
			collision = 6;
		}
	}
	
	return collision;
}

void moveBall(int oldPosition [], int newPosition [])//the idea of this is to just move the ball in a given direction. If it reaches a particular point, it will bounce
{	
		currentRow = newPosition[0];
		currentCol = newPosition[1];
		
		mvprintw(oldPosition[0], oldPosition[1], " ");
		mvprintw(newPosition[0], newPosition[1], "o");
}

void makeLevel()
{
	movePaddle(2);//reinitialize it
	if(level > MAXLEVEL)//if there is no level afterword, then loop back to the first level
	{
		level = 1;
		scoreboard(level,score,lives);
	}
	
	if(level == 1)//loads level 1 of the game
	{
		for(int i = 1; i < displayColMax-1; i++)
			display[2][i] = 2;
		for(int i = 1; i < displayColMax-1; i++)
			display[3][i] = 2;
		for(int i = 1; i < displayColMax-1; i++)
			display[4][i] = 2;
		for(int i = 1; i < displayColMax-1; i++)
			display[4][i] = 2;
	}
	
	else if(level == 9)//loads level 2 of the game
	{
		//row2
		display[2][9] = 2;
		display[2][10] = 2;
		display[2][13] = 2;
		display[2][14] = 2;
		display[2][17] = 2;
		display[2][18] = 2;
		display[2][19] = 2;
		display[2][20] = 2;
		display[2][21] = 2;
		display[2][22] = 2;
		display[2][25] = 2;
		display[2][26] = 2;
		
		//row3
		display[3][9] = 2;
		display[3][10] = 2;
		display[3][13] = 2;
		display[3][14] = 2;
		display[3][17] = 2;
		display[3][18] = 2;
		display[3][19] = 2;
		display[3][20] = 2;
		display[3][21] = 2;
		display[3][22] = 2;
		display[3][25] = 2;
		display[3][26] = 2;
		
		//row4		
		display[4][9] = 2;
		display[4][10] = 2;
		display[4][11] = 2;
		display[4][12] = 2;
		display[4][13] = 2;
		display[4][14] = 2;
		display[4][19] = 2;
		display[4][20] = 2;
		display[4][25] = 2;
		display[4][26] = 2;
		
		//row5
		display[5][9] = 2;
		display[5][10] = 2;
		display[5][11] = 2;
		display[5][12] = 2;
		display[5][13] = 2;
		display[5][14] = 2;
		display[5][19] = 2;
		display[5][20] = 2;
		display[5][25] = 2;
		display[5][26] = 2;
		
		//row6
		display[6][9] = 2;
		display[6][10] = 2;
		display[6][13] = 2;
		display[6][14] = 2;
		display[6][17] = 2;
		display[6][18] = 2;
		display[6][19] = 2;
		display[6][20] = 2;
		display[6][21] = 2;
		display[6][22] = 2;
		
		//row7
		display[7][9] = 2;
		display[7][10] = 2;
		display[7][13] = 2;
		display[7][14] = 2;
		display[7][17] = 2;
		display[7][18] = 2;
		display[7][19] = 2;
		display[7][20] = 2;
		display[7][21] = 2;
		display[7][22] = 2;
		display[7][25] = 2;
		display[7][26] = 2;
	}
	
	else if(level == 3)
	{
		for(int i = 3; i <= 32; i++)
		{
			display[3][i] = 2;
		}
		for(int i = 4; i <= 6; i++)
		{
			display[i][3] = 2;
			display[i][4] = 2;
			display[i][31] = 2;
			display[i][32] = 2;
		}
		
		for(int i = 3; i <= 32; i++)
		{
			display[7][i] = 2;
		}
	}
	
	else if(level == 4)
	{
		int numberIndex = 4;
		for(int i = 2; i <= 8; i++)
		{
			for(int j = 1; j <= numberIndex; j++)
			{
				display[i][j] = 2;
			}
			numberIndex = numberIndex+4;
		}
	}
	
	else if(level == 5)
	{
		for(int i = 3; i<=7; i+=4)
		{
			for(int j = 1; j <= 34; j++)
			{
				display[i][j] = 2;
			}
		}
		
		for(int i = 4; i<=6; i++)
		{
			display[i][1] = 2;
			display[i][2] = 2;
			display[i][3] = 2;
			display[i][4] = 2;
			display[i][17] = 2;
			display[i][18] = 2;
			display[i][31] = 2;
			display[i][32] = 2;
			display[i][33] = 2;
			display[i][34] = 2;
		}
		
		for(int i = 4; i <= 6; i+=2)
		{
			display[i][7] = 2;
			display[i][8] = 2;
			display[i][13] = 2;
			display[i][14] = 2;
			display[i][21] = 2;
			display[i][22] = 2;
			display[i][27] = 2;
			display[i][28] = 2;
		}
		
		display[5][5] = 2;
		display[5][6] = 2;
		display[5][15] = 2;
		display[5][16] = 2;
		display[5][19] = 2;
		display[5][20] = 2;
		display[5][29] = 2;
		display[5][30] = 2;
	}
	
	else if(level == 6)
	{
		//row2
		display[2][1] = 2;
		display[2][2] = 2;
		display[2][3] = 2;
		display[2][4] = 2;
		display[2][5] = 2;
		display[2][6] = 2;
		display[2][9] = 2;
		display[2][10] = 2;
		display[2][11] = 2;
		display[2][12] = 2;
		display[2][13] = 2;
		display[2][14] = 2;
		display[2][17] = 2;
		display[2][18] = 2;
		display[2][21] = 2;
		display[2][22] = 2;
		display[2][23] = 2;
		display[2][24] = 2;
		display[2][25] = 2;
		display[2][26] = 2;
		display[2][29] = 2;
		display[2][30] = 2;
		display[2][31] = 2;
		display[2][32] = 2;
		display[2][33] = 2;
		display[2][34] = 2;
		
		//row 3
		display[3][3] = 2;
		display[3][4] = 2;
		display[3][10] = 2;
		display[3][9] = 2;
		display[3][13] = 2;
		display[3][14] = 2;
		display[3][23] = 2;
		display[3][24] = 2;
		display[3][29] = 2;
		display[3][30] = 2;
		display[3][33] = 2;
		display[3][34] = 2;
		
		for(int i = 4; i <= 7; i++)
		{
			display[i][3] = 2;
			display[i][4] = 2;
			display[i][9] = 2;
			display[i][10] = 2;
			display[i][13] = 2;
			display[i][14] = 2;
			display[i][17] = 2;
			display[i][18] = 2;
			display[i][23] = 2;
			display[i][24] = 2;
			display[i][29] = 2;
			display[i][30] = 2;
			display[i][33] = 2;
			display[i][34] = 2;
		}
			display[5][11] = 2;
			display[5][12] = 2;
			display[7][31] = 2;
			display[7][32] = 2;
	}
	
	else if(level == 7)
	{
		int startLeft = 5;
		int endLeft = 8;
		int startRight = 27;
		int endRight = 30;
		int tempIndex = 0;
		
		for(int i = 2; i <= 8; i++)
		{
			if(i < 6)
			{			
				for(tempIndex = startLeft; tempIndex <= endLeft; tempIndex++)
				{
					display[i][tempIndex] = 2;
				}
				
				
				for(tempIndex = startRight; tempIndex <= endRight; tempIndex++)
				{
					display[i][tempIndex] = 2;
				}
				
				endLeft += 4;
				startRight -= 4;
			}
			
			if(i > 5)
			{
				if(i == 6)
				{
					int startLeft = 9;
					int endLeft = 16;
					int startRight = 17;
					int endRight = 26;
					int tempIndex = 0;
				}
				
				for(tempIndex = startLeft; tempIndex <= endLeft; tempIndex++)
				{
					display[i][tempIndex] = 2;
				}		
				for(tempIndex = startRight; tempIndex <= endRight; tempIndex++)
				{
					display[i][tempIndex] = 2;
				}
				startLeft += 4;
				endRight -= 4;
			}
			
			
		}
		
		display[6][17] = 0;
		display[6][18] = 0;
		display[7][17] = 0;
		display[7][18] = 0;
	}
	
	
	else if(level == 8)
	{
		for(int i = 3; i <= 32; i+=4)
		{
			display[3][i] = 2;
			display[3][i + 1] = 2;
		}
		for(int i = 4; i <= 5; i++)
		{
			for(int j = 3; j <= 32; j++)
			{
				display[i][j] = 2;
			}
		}
		
		for(int i = 5; i <= 30; i++)
		{
			display[6][i] = 2;
		}
	}
	
	else if(level == 2)
	{
		int row = 2;
		for(row; row <= 8; row+= 6)
		{
			for(int i = 1; i < 35; i+=1)
				display[row][i] = 2;
		}
		for(row = 3; row <= 7; row++)
		{
			display[row][1] = 2;
			display[row][2] = 2;
			display[row][33] = 2;
			display[row][34] = 2;
		}
		for(row = 4; row <= 6; row+=2)
		{
			for(int i = 5; i <= 30; i++)
				display[row][i] = 2;
		}
		display[5][5] = 2;
		display[5][6] = 2;
		display[5][29] = 2;
		display[5][30] = 2;
	}
	
	makeBlock();//have the blocks show up on the screen
}

void makeBlock()//actually prints the level to the screen
{
	for(int rowIndex = 0; rowIndex < displayRowMax; rowIndex++)//goes through each row
	{
		for(int colIndex = 0; colIndex < displayColMax; colIndex++)//goes through each colum
		{
			if(display[rowIndex][colIndex] == 2)//if the display at the current point has the block value
			{
				mvprintw(rowIndex, colIndex, "=");//our "block"
				numberOfBlocks++;//make sure it is accounted for
			}
		}
	}
	numberOfBlocks = numberOfBlocks/2;//because there is two additions per block, divide by two
}

void removeBlock(int newPosRow, int newPosCol)
{
	int blockPos = newPosCol % 2;//1 is left, 0 is right
	if(blockPos == 1)//if the second half is on the right
	{
		display[newPosRow][newPosCol] = 0;
		display[newPosRow][newPosCol + 1];
		mvprintw(newPosRow, newPosCol, " ");
		mvprintw(newPosRow, newPosCol + 1, " ");
	}
	else//the second half is on the left
	{
		display[newPosRow][newPosCol] = 0;
		display[newPosRow][newPosCol - 1];
		mvprintw(newPosRow, newPosCol, " ");
		mvprintw(newPosRow, newPosCol - 1, " ");
	}
	numberOfBlocks--;//make sure you know that there is one less block
}

void readyGoMessage()//just the pretty image at the beginning
{
	mvprintw(15, 14, "Ready?");
	refresh();
	sleep(1);
	mvprintw(15, 14, "      ");
	refresh();
	sleep(1);
	mvprintw(15, 16, "GO!");
	refresh();
	sleep(3);
	mvprintw(15, 16, "   ");
	refresh();
}

void quitGame(int s)//allows the user to quit out of the game
{
	if(s == 1)//if the user gets an acutal gameover, else give them the bad ending
	{
		set_ticker(0);//stop the ticker
		clear();//clear the screen
		mvprintw(0,0, "CONFOUND THOSE BLOCKS!");
		refresh();
		sleep(1);//pause for effect
		mvprintw(1,0, "OOOOOOH HOW I HATE THEM!");
		refresh();
		sleep(1);//pause for effect
		mvprintw(3,0, "Game Over");
		mvprintw(4,0, "Your Score: %d", score);//give the user their score
		refresh();
		sleep(5);
//		clear();
//		endwin();//reset back to regular terminal
//		exit(0);//exit
		mvprintw(6,0, "Press any key to continue");
	}
	else
	{
		set_ticker(0);//stop the ticker
		clear();
		mvprintw(0,0, "RUN, COWARD!\nSINISTAR HUNGERS!");
		refresh();
		sleep(5);
		clear();
		endwin();//reset back to regular terminal
		exit(0);//exit
	}
	
	
}

void handleKey(int keyboardIn)
{	
		if(keyboardIn == 260)
		{
			//move paddle left
			movePaddle(0);
			
		}
		
		else if(keyboardIn == 261)
		{
			//move paddle right
			movePaddle(1);
		}
		
		if(keyboardIn == 'q')
		{
			//user wants to quit
			quitGame(0);//exit
		}
		
		if(keyboardIn == 'p')
		{
			//user wants to pause the game
			pauseGame();
		}
		
		if(keyboardIn == 'e')
		{
				//user wants to end the current game
				lives = 0;
				quitGame(1);
		}
}

void levelReset()
{
		if(numberOfBlocks == 0)
		{
			set_ticker(0);//stop the ticker
			mvprintw(currentRow, currentCol, " ");//hide the ball
			mvprintw(8, 8, "Level Complete!");//tell the user they completed the level
			refresh();
			sleep(3);
			level++;//goto the next level
			currentRow = 16;//reset the ball's current row
	        currentCol = 17;//reset the ball's current column
	        clear();
			direction = 2;//up right by default
			collision = 0;
			
			for(int i = 0; i < displayRowMax; i++)//initialize display
			{
				for(int j =0; j < displayColMax; j++)
					display[i][j] = 0;
			}
			
		    scoreboard(level,score,lives);	//initialize the scoreboard (level, score, lives)
			makeWall();//add the coordinates
	        mvprintw(currentRow, currentCol, "o");
			makeLevel();
			mvprintw(18, 0, "               ");
			readyGoMessage();
			set_ticker(SPEED);//set the ticker back
		}
}

void movePaddle(int paddleDirection)//0 is left, 1 is right, 2 is initialize
{
	int paddleRight = currentPaddleLeft+3;//the right most position of the paddle
	
	if((paddleDirection == 0) && (currentPaddleLeft > 1))//move left unless its already at the end
	{	
		paddleRow[currentPaddleLeft+3] = 0;
		mvprintw(17, paddleRight, " ");
		currentPaddleLeft--;
		paddleRow[currentPaddleLeft] = 1;
		mvprintw(17, currentPaddleLeft, "=");
	}
	
	else if((paddleDirection == 1) && (paddleRight < 34))//move right unless its already at the end
	{
		paddleRow[currentPaddleLeft] = 0;
		mvprintw(17, currentPaddleLeft, " ");
		currentPaddleLeft++;
		paddleRight++;
		paddleRow[paddleRight] = 1;
		mvprintw(17, paddleRight, "=");	
	}
	else if(paddleDirection == 2)//initialize or reset the paddle
	{
		currentPaddleLeft =16;//set the paddle back to default
		for(int i = 1; i < displayColMax-1; i++)//initialize the initial paddle, set up for the game
		{
			if((i > currentPaddleLeft -1) && ( i < currentPaddleLeft+4))//if its in the range of where it needs to be
			{
				paddleRow[i] = 1;//add the logistic part of it
				mvprintw(17,i, "=");//print it to the screen
			}
			else//nothing is there
			{
				paddleRow[i] = 0;
				mvprintw(17, i, " ");
			}			
		}
	}
	else
	{
		//do nothing
	}
}

void death()
{
	lives--;//subtract a life
	scoreboard(level,score,lives);	//initialize the scoreboard (level, score, lives)
	set_ticker(0);//stop the ticker
	mvprintw(currentRow, currentCol, " ");
	mvprintw(9, 9, "Ouch! You Died!");//let the user know they died
	refresh();
	sleep(3);
	mvprintw(9, 9, "               ");
	
	if(lives == 0)
	{
		quitGame(1);//gameover
	}
	
	if(lives > 0)
	{
	//reset everything
		direction = 2;//up right
		collision = 0;
		currentCol = 17;
		currentRow = 16;
		movePaddle(2);
		mvprintw(16, 17, "o");
		refresh();
		readyGoMessage();
		set_ticker(SPEED);
	}
}

void pauseGame()
{
	set_ticker(0);//stop the program
	mvprintw(15, 14, "Paused");//let the user know the game is paused
	mvprintw(currentRow, currentCol, " ");//so you cant cheat by seeing the ball
	refresh();
	
	char keyboardIn = getArrow();//get user input
	while(keyboardIn != 'p')//wait until the user quits or unpauses
	{
		if(keyboardIn == 'q')
		{
			quitGame(0);//exit the game
		}
		else if(keyboardIn != 'p')
		{
			keyboardIn = getArrow();
		}
	}
	
    set_ticker(SPEED);//start again
	mvprintw(15, 13, "       ");
	mvprintw(currentRow, currentCol, "o");//reinitialize the ball
	refresh();
}
