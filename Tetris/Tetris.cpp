// Tetris game
// John Breckenridge
// September 2016
// This is a full game of tetris for Programming II, now includes everything!

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

using namespace std;

class Shape {
public:
	Shape();//this is the default constructor. it will generate a random shape; there is no need to be able to control the type of shape created
	char shapeArray[4][4];//this will record which shape it is and how it is oriented
	int xPosition, yPosition;//this will record where the piece is at
	void rotatePiece(char board[][18]);//rotates the piece 90 degrees clockwise
	void movePiece(int input, char board[][18]);//moves it one square left or right
	int dropPiece(char board[][18]);//moves the piece down one row
	int stopPiece(char board[][18]);//fixes the piece in place and generates new shape. called by dropPiece when it detects a collision
};

Shape::Shape()
{
	int randnum = rand() % 7;//this will pick which of the 7 shapes will be generated
	xPosition = 4;
	yPosition = 1;//it starts at 1 instead of 0 to allow for all pieces to be more central on the grid for better rotation
	for (int y = 0; y < 4; y++)//this will make sure everything is blank except the actual components of the shape
		for (int x = 0; x < 4; x++)
			shapeArray[x][y] = ' ';
	if (randnum == 0)// L
	{
		shapeArray[1][0] = '@';
		shapeArray[1][1] = '@';
		shapeArray[1][2] = '@';
		shapeArray[2][2] = '@';
	}
	else if (randnum == 1)//backwards L
	{
		shapeArray[2][0] = '@';
		shapeArray[2][1] = '@';
		shapeArray[1][2] = '@';
		shapeArray[2][2] = '@';
	}
	else if (randnum == 2)//s
	{
		shapeArray[1][1] = '@';
		shapeArray[2][1] = '@';
		shapeArray[0][2] = '@';
		shapeArray[1][2] = '@';
		yPosition = 0;//some peices don't have any blocks in the top row and thus start 1 row higher (so the top block of the piece always appears at the top row)
	}
	else if (randnum == 3)//z
	{
		shapeArray[0][1] = '@';
		shapeArray[1][1] = '@';
		shapeArray[1][2] = '@';
		shapeArray[2][2] = '@';
		yPosition = 0;//some peices don't have any blocks in the top row and thus start 1 row higher (so the top block of the piece always appears at the top row)
	}
	else if (randnum == 4)// line or l
	{
		shapeArray[1][0] = '@';
		shapeArray[1][1] = '@';
		shapeArray[1][2] = '@';
		shapeArray[1][3] = '@';
	}
	else if (randnum == 5)//T
	{
		shapeArray[0][1] = '@';
		shapeArray[1][1] = '@';
		shapeArray[2][1] = '@';
		shapeArray[1][2] = '@';
		yPosition = 0;//some peices don't have any blocks in the top row and thus start 1 row higher (so the top block of the piece always appears at the top row)
	}
	else if (randnum == 6)//block
	{
		shapeArray[1][1] = '@';
		shapeArray[2][1] = '@';
		shapeArray[1][2] = '@';
		shapeArray[2][2] = '@';
		yPosition = 0;//some peices don't have any blocks in the top row and thus start 1 row higher (so the top block of the piece always appears at the top row)
	}
}

int Shape::stopPiece(char board[][18])
{
	int lineCount = 0;//counts how many lines were cleared for scoring purposes
	for (int y = 0; y < 4; y++)//this will add the blocks of the piece to the board
	{
		for (int x = 0; x < 4; x++)
		{
			if (shapeArray[x][y] == '@')
			{
				board[xPosition + x][yPosition + y] = '@';
			}
		}
	}
	int count1 = 0;//keeps track of how many blocks are filled in a row
	for (int y = 16; y > 0; y--)
	{
		for (int x = 1; x < 11; x++)//this will check to see if an entire line is full
		{
			if (board[x][y] == '@')
				count1++;
		}
		if (count1 == 10)//if it is full, then it will be cleared
		{
			for (int yy = y; yy > 0; yy--)
			{
				for (int x = 1; x < 11; x++)
				{
					board[x][yy] = board[x][yy - 1];
				}
			}
			y++;//after everything gets moved down the same line needs to get checked again
			lineCount++;
		}
		count1 = 0;
	}

	//check for any pieces at roof after stopping and clearing lines, if there are return a -1 so main loop knows to end
	for (int x = 1; x < 11; x++)
		if (board[x][1] == '@')
			return -1;

	if (lineCount == 0)//returns a 1 if a piece was stopped but nothing cleared
		return 1;
	else if (lineCount == 1)//return appropriate points total for lines cleared
		return 100;
	else if (lineCount == 2)
		return 300;
	else if (lineCount == 3)
		return 500;
	else if (lineCount == 4)
		return 800;

}

int Shape::dropPiece(char board[][18])//returns 0 if piece drops, 1 if peice stops, 100,300,500, or 800 if lines are cleared, -1 for game end
{
	int drop = 1;
	for (int y = 0; y < 4; y++)//this will check to see if the place the piece is trying to move is blocked. if so, it calls stopPiece
	{
		for (int x = 0; x < 4; x++)
		{
			if (shapeArray[x][y] == '@' && (board[xPosition + x][yPosition + y + 1] == '@' || board[xPosition + x][yPosition + y + 1] == '#'))
			{
				drop = 0;
			}
		}
	}
	if (drop)//if the space is clear, then the piece drops
	{
		yPosition++;
		return 0;
	}
	else//but if it is not clear then it will call stop piece and tell main to generate a new shape
	{
		drop = stopPiece(board);
		return drop;
	}
}


void Shape::rotatePiece(char board[][18])
{
	//checks to make sure the piece can be rotated
	//if it can, turn the piece 90 degrees clockwise
	char tempArray[4][4];//this will hold the rotated piece until it can be confirmed that it won't hit anything
	for (int i = 0; i < 4; i++)
		tempArray[3][i] = shapeArray[i][0];
	for (int i = 0; i < 3; i++)
		tempArray[2 - i][3] = shapeArray[3][i + 1];
	for (int i = 0; i < 3; i++)
		tempArray[0][2 - i] = shapeArray[2 - i][3];
	tempArray[1][0] = shapeArray[0][2];
	tempArray[2][0] = shapeArray[0][1];

	tempArray[2][1] = shapeArray[1][1];
	tempArray[2][2] = shapeArray[2][1];
	tempArray[1][2] = shapeArray[2][2];
	tempArray[1][1] = shapeArray[1][2];

	//check to see if temp array would hit anything
	bool rotate = true;
	for (int y = 0; y < 4; y++)//this will check to see if the places the piece would rotate to is clear
	{
		for (int x = 0; x < 4; x++)
		{
			if (tempArray[x][y] == '@' && (board[xPosition + x][yPosition + y + 1] == '@' || board[xPosition + x][yPosition + y + 1] == '#'))
			{
				rotate = false;//if anything would overlap, don't rotate
			}
		}
	}
	if (rotate)//if the space is clear, then the piece rotates
	{
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				shapeArray[x][y] = tempArray[x][y];
	}//if its not clear, don't do anything
}

void Shape::movePiece(int input, char board[][18])
{
	//check to make sure piece can move
	//move the piece 1 column left or right
	bool move = true;
	int lr = 76 - input;//this will give a -1 or right and a 1 for left
	lr = lr*-1;
	for (int y = 0; y < 4; y++)//this will check to see if the place the piece would move to is clear
	{
		for (int x = 0; x < 4; x++)
		{
			if (shapeArray[x][y] == '@' && (board[xPosition + x + lr][yPosition + y] == '@' || board[xPosition + x + lr][yPosition + y] == '#'))
			{
				move = false;//if anything would overlap, don't move
			}
		}
	}
	if (move)//if the way is clear, move, otherwise don't do anything
		xPosition += lr;
}

void setCursorTo(int x, int y)//this will set the position to be displayed at to the coordinates given, allowing out output of the grid to effectively be a new frame in a video
{
	HANDLE handle;
	COORD position;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(handle, position);
}
void gameEnd(int score)//this will display a game over text and the final score
{
	setCursorTo(0, 18);//c++ requires two "\\" in order to output one as it is the escape character
	cout << "     _____                                 ____                        " << endl;
	cout << "    / ____|                               / __ \\                       " << endl;
	cout << "   | |  __    __ _   _ __ ___     ___    | |  | | __   __   ___   _ __ " << endl;
	cout << "   | | |_ |  / _` | | '_ ` _ \\   / _ \\   | |  | | \\ \\ / /  / _ \\ | '__|" << endl;
	cout << "   | |__| | | (_| | | | | | | | |  __/   | |__| |  \\ V /  |  __/ | |   " << endl;
	cout << "    \\_____|  \\__,_| |_| |_| |_|  \\___|    \\____/    \\_/    \\___| |_|   " << endl;
	cout << endl << "Your final score is: " << score;
}

void displayBoard(Shape shape, char board[][18], int score)//this will output the board
{
	setCursorTo(0, 0);
	for (int y = 0; y < 4; y++)//this will temporarly add the shape to the board for display
	{
		for (int x = 0; x < 4; x++)
		{
			if (shape.shapeArray[x][y] == '@')
				board[x + shape.xPosition][y + shape.yPosition] = '@';
		}
	}
	cout << "Score: " << score << endl;//this will put the score at the top of the screen
	for (int y = 1; y < 18; y++)//this will output the board
	{
		for (int x = 0; x < 12; x++)
		{
			cout << board[x][y];
		}
		cout << endl;
	}
	for (int y = 0; y < 4; y++)//this will remove the shape from the board
	{
		for (int x = 0; x < 4; x++)
		{
			if (shape.shapeArray[x][y] == '@')
				board[x + shape.xPosition][y + shape.yPosition] = ' ';
		}
	}
}
char userInput()
{
	while (_kbhit())//this will only be true while a button is being pressed
	{
		char input = _getch();//this will get whatever key is being pressed without displaying it on screen
		return input;
	}
	return 0;
}

int main()
{
	//initilize variables
	bool lose = 0;
	char input = 0;
	int count = 0;
	int stop = 0;
	int score = 0;
	srand(time(NULL));//for use in randomly generating shapes

					  //initilize game
	char board[12][18] = {};
	for (int i = 0; i < 18; i++)//this will set the left and right sides as a border
	{
		board[0][i] = '#';
		board[11][i] = '#';
	}
	for (int i = 1; i < 11; i++)//and this will set the bottom as a border
	{
		board[i][17] = '#';
	}
	Shape shape;//this will make the first shape

				//main game loop
	while (!lose)
	{
		input = userInput();
		if (input == 75 || input == 77)//75 is right, 77 is left, 72 is up, 80 is down
			shape.movePiece(input, board);
		else if (input == 72)
			shape.rotatePiece(board);
		else if (input == 80)
			stop = shape.dropPiece(board);
		count++;
		if (count > 30 && stop == 0)//this will cause the piece to drop naturally but at a slower rate than the user can input commands
		{
			stop = shape.dropPiece(board);
			count = 0;
		}
		if (stop > 1)
			score += stop;
		if (stop == -1)
			lose = true;
		if (stop > 0)//if either of the above dropPiece() caused it to stop, then a new piece must be made
		{
			shape = Shape();
			stop = 0;
		}
		displayBoard(shape, board, score);
		Sleep(20);//tick time has been lowered dramatically. it really helped the controls feel better
	}
	gameEnd(score);//outputs a game over screen and final score
	cin.ignore();
}

