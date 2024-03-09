#include <iostream>
#include <fstream>
#include <conio.h>		// to control inputs and outputs
#include <ctime>
#include <windows.h>
#include <cstdlib>
#include <vector>

#define Height 25		// one line functions called Macros
#define Width 50

using namespace std;

class Snake
{
	private:		
		COORD pos;
		int length;
		int speed;
		char direction;
		vector<COORD> body;
		
	public:
		Snake(COORD pos,int speed);	//control speed
		void change_dir(char dir);	//control direction
		void move_snake();			//control movement
		COORD get_pos();				//to get the current position of snake
		
		vector<COORD> get_body();	// making the snake a vector object so its particles never cross each other
		bool eaten(COORD food_pos);
		
		void grow();
		bool collide();
};

class Food
{
	private:
		COORD pos;
	
	public:
		void gen_food();
		COORD get_pos();
};

Snake snake ({Width/2, Height/2}, 1) ;
Food food;

int score;

void Food::gen_food()	// to generate food
{
	pos.X= (rand() % (Width-4))+2;		//to generate food at random positions
	pos.Y= (rand() % (Height-4))+2;
}

COORD Food::get_pos()
{
	return pos;
}


Snake::Snake(COORD pos, int speed)	// to control speed of snake and its position
{
	this-> pos=pos;
	this-> speed=speed;
	length=1;
	direction='n';
	
	body.push_back(pos);
}

void Snake::change_dir(char dir) 	// to change directions of the snake
{
	direction=dir;
}

void Snake::grow()
{
	length++;
}

COORD Snake::get_pos() 
{
	return pos;
}

vector<COORD>	Snake::get_body() {return body;}

void Snake::move_snake()	// to control the movement of the snake
{
	switch(direction)
	{
		case 'u': pos.Y -= speed;	//to move snake upward
		break;
		case 'd': pos.Y += speed;	//to move snake downward
		break;
		case 'l': pos.X -= speed;	//to move snake on left
		break;
		case 'r': pos.X += speed;	//to move snake on right
		break;
	}
	if(pos.X>Width-2)	// if snake strike the right side it appaear  from left side
	{
		pos.X=1;
	}
	if(pos.X<1)			// if snake strike left side it appear from left side
	{
		pos.X=Width-2;
	}
	if(pos.Y>Height-2)	// if snake strike the upper side it appaear from down side
	{
		pos.Y=1;
	}
	if(pos.Y<1)			// if snake strike down side it appear from upper side
	{
		pos.Y=Height-2;
	}
	
	body.push_back(pos);
	if (body.size()>length)
	{
		body.erase(body.begin());
	}
}

bool Snake::eaten(COORD food_pos)		// whenever snake and food positions are similar, the snake eat food
{
	if (food_pos.X==pos.X && food_pos.Y==pos.Y)
	{
		return true;
	}
	else{
		return false;
	}
}

bool Snake::collide()
{
	for(int i=0; i<length-1; i++)		// game over if snake collide with its body
	{
		if(pos.X==body[i].X && pos.Y==body[i].Y)
		{
			return true;
		}
	}
	return false;
}



void Boundary()			//for making the boundary of the game
{
	COORD snake_pos=snake.get_pos();		//to capture/get the position of snake on screen
	COORD food_pos=food.get_pos();			// to get the position of food
	
	vector<COORD> snake_body = snake.get_body();
	int High=0;
	ifstream file("SnakeGame_HighScore.txt");
	file>>High;
	file.close();
	cout<<"\n  Use Arrow-Keys to Control!\n";
	cout<<"   High Score is: "<<High;
	cout<<"\n   SCORE: "<<score<<"\n";
	for(int a=0;a<Height;a++)
	{
		cout<<"\t\t\t\t\t\t\t0";
		for(int b=0;b<(Width-2);b++)
		{
			if(a==0 || a==(Height-1))
			{
				cout<<"0";
			}
			else if(a==snake_pos.Y && b+1==snake_pos.X)	
			{
				cout<<"$";		// represent the snake
			}
			else if(a==food_pos.Y && b+1==food_pos.X)
			{
				cout<<"*";
			}
			else
			{
				bool Body_collision = false;
				for(int k=0; k<snake_body.size()-1; k++)
				{
					if (a==snake_body[k].Y && b+1== snake_body[k].X)
					{
						cout<<"c";
						Body_collision=true;
						break;
					}
				}
				if (!Body_collision)
				{
					cout<<" ";
				}
			}
		}
		cout<<"0\n";
	}
}

int main()
{
	score=0;
	srand(time(NULL));		// generating random food positions 
	food.gen_food();
	bool game_over=false;
	while (!game_over)
	{
		Boundary();
		if (kbhit())		//if press any key it evaluates it true
		{
			switch(getch())		//to capture the key enter by the user
			{
				case 72:{
					snake.change_dir('u');
					break;
				}
				case 80:{
					snake.change_dir('d');
					break;
				}						// ASCII values of Arrow-keys
				case 75:{
					snake.change_dir('l');
					break;
				}
				case 77:{
					snake.change_dir('r');
					break;
				}
			}
		}
		if(snake.collide())
		{
			game_over=true;
		}
		
		if(snake.eaten(food.get_pos()))	//to generate another food if snake eaten the previous food
		{
			food.gen_food();
			snake.grow();
			score++;
		}
		
		snake.move_snake();		//to keep moving the snake

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),{0,0}); 	
	}
	int High=0;
	ifstream file("SnakeGame_HighScore.txt");
	file>>High;
	file.close();
	
	ofstream HighScore("SnakeGame_HighScore.txt");
	if (score>High)
	{
		HighScore<<score;
	}
	else 
	{
		HighScore<<High;
	}
	HighScore.close();
	if (score<=High)
	{
		cout<<"\n\t\tYour Score is: "<<score;
		cout<<"\n\t\tGame Over!";
	}
	else 
	{
		cout<<"\n\t\tCongratulations!\n\t\tYou make a New High Score!";
		cout<<"\n\t\tYour Score is: "<<score;
		cout<<"\n\t\tNew High Score is: '"<<score<<"'\n";
	}
	return 0;
}
