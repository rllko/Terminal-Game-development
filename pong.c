// snake_game.C
// made by rikko
#include <stdio.h>
#include <conio.h>
#include <ctime>
#include <math.h>

#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#define HEIGHT 20
#define WIDTH 40
#define BOX_SIZE 2

//TO DO: for testing purposes , define fps formula later
#define FPS 50

#define PADDLE_HEIGHT (HEIGHT - 1)
#define PADDLE_SIZE 6

//maybe the boxes will be dynamic i can remove and add more?
//typedef struct box {
//	int width = BOX_SIZE;
//	bool active = false;
//	int* coord = NULL;
//	char symbol = 251; // ░ 
//}Box;

struct {
	int* left_ptr, * right_ptr, * middle_ptr;
	int left, right, middle;
	int size, symbol_int;
	char symbol_ch;
} paddle;

enum hit_direction { left, right, none };

struct {
	int x, y;
	int velocity, speed_multiplier, symbol_int;
	char symbol_ch;
	hit_direction direction;
} ball;

//general purpose variables
bool game_status = false;
int var;

//2D environment
int Field[HEIGHT][WIDTH] = { 0 };

void setup_ball() {
	//setup ball properties
	ball.symbol_int = -1;
	ball.symbol_ch = 15;//☼

	ball.x = WIDTH / 2;
	ball.y = PADDLE_HEIGHT - 2;
	ball.speed_multiplier = 1;
	ball.direction = none;

	// Position = initial position + speed * multiplier
	ball.velocity = 1 * ball.speed_multiplier;
}

void setup_paddle() {
	//setup basic properties
	paddle.size = PADDLE_SIZE;
	paddle.symbol_int = 1;
	paddle.symbol_ch = (char)178;// ▓

	//setup paddle positions
	paddle.left_ptr = Field[HEIGHT - 1] + ((WIDTH / 2) - 1);
	paddle.right_ptr = paddle.left_ptr + paddle.size;

}

void init() {

	setup_paddle();

	//Build paddle in the array
	int* ch_placement_index = paddle.left_ptr;

	for (int size_index = 0; ch_placement_index != paddle.right_ptr; ch_placement_index++, size_index++) {

		if (size_index == paddle.size / 2)
			paddle.middle_ptr = ch_placement_index;

		*ch_placement_index = paddle.symbol_int;
	}

	setup_ball();

	//TO DO: BOXES, i dont feel like doing it
}
void print() {
	int curr_ch = 0;
	const int CHAR_EMPTY = ' ';

	for (int render_y_pos = 0; render_y_pos < HEIGHT; render_y_pos++) {
		for (int render_x_pos = 0; render_x_pos < WIDTH; render_x_pos++) {
			int* curr_pixel = &Field[render_y_pos][render_x_pos];

			//box render code ( -2 )
			//if (*curr_pixel == -2)
			//	curr_ch = box.;

			//Visual limits for the user
			if (render_x_pos == WIDTH)
				printf("%c", '|');

			if (!(*curr_pixel))
				curr_ch = CHAR_EMPTY;

			//Paddle render properties
			if (*curr_pixel == paddle.symbol_int)
				curr_ch = paddle.symbol_ch;

			if (*curr_pixel == ball.symbol_int)
				curr_ch = ball.symbol_ch;

			/*
			* DEBUG PADDLE TEXT*/
			if (curr_pixel == paddle.left_ptr)
				curr_ch = 'L';
			else if (curr_pixel == paddle.middle_ptr)
				curr_ch = 'M';
			else if (curr_pixel == paddle.right_ptr)
				curr_ch = 'R';

			printf("%c", curr_ch);
		}
		putc('\n', stdout);
	}
}

void resetscreen() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Position = { 0,0 };

	SetConsoleCursorPosition(hOut, Position);
}

//Simple function to control the user input
int handle_input() {
	if (_kbhit())
		return _getch();

	return -1;
}

void movement() {
	var = handle_input();
	var = tolower(var);

	switch (var) {
	case 'a':

		if (paddle.left_ptr <= Field[PADDLE_HEIGHT])
			return;

		//delete old position
		*paddle.right_ptr = 0;

		//change cordinate
		paddle.left_ptr -= 1;
		paddle.middle_ptr -= 1;
		paddle.right_ptr -= 1;

		//place block at new position
		*paddle.left_ptr = 1;

		break;
	case 'd':

		if (paddle.right_ptr >= Field[PADDLE_HEIGHT] + (WIDTH - 1))
			return;

		// delete old position
		*paddle.left_ptr = 0;

		//change cordinate
		paddle.left_ptr += 1;
		paddle.middle_ptr += 1;
		paddle.right_ptr += 1;

		//place block at new position
		*paddle.right_ptr = 1;
		break;
	}
}

void gameover() {
	//end game loop
	game_status = true;

	//clear screen and show message
	system("cls");
	printf("\tGAME OVER ! ! 1! !");
}

void check_colision() {
	//collision with the paddle
	paddle.left = (int)(paddle.left_ptr - Field[PADDLE_HEIGHT]);
	paddle.middle = (int)(paddle.middle_ptr - Field[PADDLE_HEIGHT]);
	paddle.right = (int)(paddle.right_ptr - Field[PADDLE_HEIGHT]);

	if (ball.y >= (PADDLE_HEIGHT - 1)) {
		if (ball.x == paddle.middle) {
			ball.velocity = -ball.velocity;
			ball.direction = none;
		}

		if ((ball.x < paddle.middle) && (ball.x >= paddle.left)) {
			ball.direction = left;
			ball.velocity = -ball.velocity;
		}

		if ((ball.x > paddle.middle) && (ball.x <= paddle.right)) {
			ball.direction = right;
			ball.velocity = -ball.velocity;
		}
	}

	//put the ball running
	ball.y -= ball.velocity;

	//repeat direction until colision
	switch (ball.direction) {
	case left:
		ball.x -= ball.velocity;
		break;
	case right:
		ball.x += ball.velocity;
		break;
	}

}

inline void delete_ball() {
	Field[ball.y][ball.x] = 0;
}

inline void create_new_ball() {


	//top limit
	if (ball.y >= 1) {
		ball.velocity = -ball.velocity;
	}

	//bottom limit / game over
	if (ball.y > HEIGHT) {
		gameover();
	}

	//left limit
	if (ball.x >= 1)ball.velocity = -ball.velocity;
	//top limit

	//right limit
	if (ball.x >= WIDTH) {
		ball.velocity = -ball.velocity;
		ball.direction = ball.direction == left ? right : left;
	}


	//corner collision check
	if (ball.x <= -1 || ball.y <= -1)
	{
		ball.y += 1;
		ball.x += 1;
		ball.velocity = -ball.velocity;
	}

	Field[ball.y][ball.x] = ball.symbol_int;
}

void update_ball() {
	delete_ball();

	check_colision();

	create_new_ball();
}

void draw() {
	print();
	Sleep(FPS);
	resetscreen();
}

void input() {
	movement();
}

void logic() {
	update_ball();
}

int
main()
{
	init();
	while (game_status == false) {
		draw();
		input();
		logic();
	}

	return 0;
}