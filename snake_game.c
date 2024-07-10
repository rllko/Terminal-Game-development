// snake_game.C
// made by rikko
#include <stdio.h>
#include <conio.h>
#include <ctime>

// To clear the screen
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <random>

//field properties
#define HEIGHT 40
#define WIDTH 40

int x, y, head, tail, var, dir = 'd';

int Field[WIDTH][HEIGHT] = { 0 };
bool frog_status = false, game_status = false;


void
print_table() {
	const int CHAR_SNAKE_BODY = 176; // ░
	const int CHAR_SNAKE_HEAD = 178; // ▓
	const int CHAR_HOR = 205; // ═
	const int CHAR_VERT = 186; // ║
	const char CHAR_EMPTY = ' ';
	const int CHAR_TOPL = 201; // ╔
	const int CHAR_TOPR = 187; // ╗
	const int CHAR_BOTL = 200; // ╚
	const int CHAR_BOTR = 188; // ╝
	const int CHAR_FOOD = 15; // ☼

	//TOP
	unsigned char ch;
	for (int i = 0; i <= WIDTH; i++)
	{
		ch = CHAR_HOR;

		if (i == 0)
			ch = CHAR_TOPL;

		if (i == WIDTH)
			ch = CHAR_TOPR;

		putc(ch, stdout);
	}

	putc('\n', stdout);

	// ALL THE LINES

	for (int render_y_pos = 0; render_y_pos < HEIGHT; render_y_pos++) {
		char curr_ch = CHAR_EMPTY;

		//Print player stuff
		for (int render_x_pos = 0; render_x_pos < WIDTH; render_x_pos++) {
			int* snake_pos = *(Field + render_y_pos) + render_x_pos;
			if (render_x_pos == 0 || render_x_pos == WIDTH - 1)
				printf("%c", CHAR_VERT);

			//Head is not a constant therefore cant be used in switch
			if (*snake_pos == head) {
				curr_ch = (char)CHAR_SNAKE_HEAD;;
			}

			if (!(*snake_pos)) {
				curr_ch = CHAR_EMPTY;
			}

			if (*snake_pos == -1)
				curr_ch = CHAR_FOOD;

			if (*snake_pos > 0 && *snake_pos < head)
				curr_ch = (char)CHAR_SNAKE_BODY;

			printf("%c", curr_ch);
		}

		putc('\n', stdout);
	}

	//BOTTOM PART
	for (int i = 0; i <= WIDTH; i++)
	{
		ch = CHAR_HOR;

		if (i == 0)
			ch = CHAR_BOTL;

		if (i == WIDTH)
			ch = CHAR_BOTR;

		putc(ch, stdout);
	}
	putc('\n', stdout);
}

//func to take care of the input
int input_request() {
	if (_kbhit())
		return _getch();

	return -1;
}

void gameOver() {
	// windows beep sound
	printf("\a");
	Sleep(1500);
	system("cls");

	//end game loop and display message
	game_status = true;
	printf("\n\n\tGame Over!");
}

void moviment() {
	var = input_request();
	var = tolower(var);

	//pointers and flags for readability
	const int food_location = -1;
	int* curr_position = &Field[y][x];

	// takes care of the direction loop where the snake is going
	// it disables the opposite moviment using the ascii char numbers w - s = 5
	if (((var == 'w' || var == 's') || (var == 'a' || var == 'd'))
		&& (abs(dir - var) > 5)) dir = var;

	switch (dir) {
	case 'd':
		//update position
		head++;
		x++;

		//if the updated position is not itself
		if (*curr_position != 0 && *curr_position != -1)
			gameOver();

		//x access right bounds check
		if (x >= WIDTH - 1) x = 0;

		//Food collision check
		if (*curr_position == food_location) {
			frog_status = false;
			tail -= 1;
		}

		*curr_position = head;
		break;
	case 'a':
		//update position
		head++;
		x--;

		//if the updated position is not itself
		if (*curr_position != 0 && *curr_position != -1)
			gameOver();

		//x access left bounds check
		if (!x) x = WIDTH - 1;

		//Food collision check
		if (*curr_position == food_location) {
			frog_status = false;
			tail -= 1;
		}

		*curr_position = head;
		break;
	case 'w':
		//update position
		y--;
		head++;

		//if the updated position is not itself
		if (*curr_position != 0 && *curr_position != -1)
			gameOver();

		//y access top bounds check
		if (y <= 0) y = HEIGHT - 1;

		//Food collision check
		if (*curr_position == food_location) {
			frog_status = false;
			tail -= 1;
		}

		*curr_position = head;
		break;
	case 's':
		//update position
		y++;
		head++;

		//if the updated position is not itself
		if (*curr_position != 0 && *curr_position != -1)
			gameOver();

		//y access bottom bounds check
		if (y >= HEIGHT + 1) y = 0;

		//Food collision check
		if (*curr_position == food_location) {
			frog_status = false;
			tail -= 1;
		}

		*curr_position = head;
		break;
	}
}

void
reset_screen() {
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Position = { 0,0 };

	SetConsoleCursorPosition(hOut, Position);
}

void
generate_rand_frog() {
	//sets random seed to null
	srand((unsigned)time(NULL));

	//generate random cordiantes
	int y_cord = (rand() % (HEIGHT - 1)) + 1;
	int x_cord = (rand() % (WIDTH - 1)) + 1;

	//place frog and activate the flag
	*(*(Field + y_cord) + x_cord) = -1;
	frog_status = true;
}

void
snakeInitialization() {
	x = WIDTH / 2, y = HEIGHT / 2;
	tail = 1, head = 5;
	//generates snake body
	int* index = &Field[y][x - head];
	for (int i = tail; i <= head; i++)
		*index++ = i;
}

void removetail() {
	//This will remove the outdated body block
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (Field[i][j] == tail) {
				Field[i][j] = 0;
			}
		}
	}
	//Added because the snake is calculated by using the difference
	//between the head and tail, by default 5
	tail++;
}



void init() {
	snakeInitialization();
	generate_rand_frog();
}

void draw() {
	print_table();
	reset_screen();
}

void input() {
	Sleep(99);
	moviment();
	removetail();
}

void logic() {
	if (frog_status == false)
		generate_rand_frog();
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