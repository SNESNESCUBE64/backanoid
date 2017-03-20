Product Description:

Backanoid is a game based off of the arcade classic, Arkanoid. This product makes use
of the lncurses library to print out a display. the idea of the game is to break all
the blocks using the paddle to bounce a ball, just like in the original. Do that gain 
points and eventually lives also. As soon as you run out of lives, the game ends. 


Installation:

In the ./Backanoid directory, type "make Backanoid" or "make". This will compile the 
main.c, set_ticker.c, arrow.c, and scoreboard.c, creating a main.o file. The resulting main.o file will then be compiled with -lcurses tag to enable the lcurses library.

Components:  

main 
- This is the main program of the game, it handles how the game is displayed, where
each object is located, user controls, and movement of the ball and paddle. It handles
the logic that allows the game to determine levels, start screen, and end screens.

arrow
- This class uses the lcurses library to listen for keyboard input. It tracks the 
keyboard to tell when arrow keys are hit. It then returns a numerical value based on 
the input to the main file.

scoreboard
- The scoreboard class acts as a secondary display. It takes in the lives, score, and 
level from the main method and displays the resulting graphic off to the side of the
game board.

set_ticker
- This class takes an input of milliseconds and converts that into a number of ticks
per second. A ticker device is then invoked and returned to the main method to control
the speed at which the game moves.
 
Game Controls:

Controlling the game is quite simple. You use the left and right arrow keys to move the paddle back and forth respectively. There is also a few other keys that perform various operation. The "p" key will pause the game, the "q" key will quit out of the program", and the "e" key to end the current game but not the program. There is also a secret code for the title screen to enable a level select involving Konami :). 
