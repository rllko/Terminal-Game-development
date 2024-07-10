/*
* TicTacToe.c
* Date: 23/05/2022 
* Author: RikkoRicardo
*/

 #include <stdio.h>
 #include <String.h>
 #include <stdlib.h>
 #define TABLE_SIZE 3


/*
if you subtract one and add another
you have one diagonal, if you use 
the one that goes from 0 to TABLE_SIZE
you get another

0,0| * |0,2
--------
 *|1,1|*
--------
2,0|* |2,2

*/
int searchDiagonal(char board[TABLE_SIZE][TABLE_SIZE],char ch){
	int x=0,y=0,diagonalScore[2] = {0,0};

	
	for(x = 0, y = TABLE_SIZE-1; x <=TABLE_SIZE-1;x++,y--){
		if(board[x][y] == ch && x+y == TABLE_SIZE) 
			diagonalScore[1]++;
			
		if(board[x][x] == ch && x+y == TABLE_SIZE) 
			diagonalScore[0]++;
	}

	return (diagonalScore[0] >= TABLE_SIZE || diagonalScore[1] >= TABLE_SIZE);
}

int searchVertical(char board[][TABLE_SIZE],int index,char ch){	
	//Checks the placed mark's column
	int i=0,counter=0;
	
	while(board[i++][index] == ch)
		counter++;

	return counter == TABLE_SIZE;
}

/*
searchHorizontal is too redundant,
can be optimized easily using:
char v[] = {ch};
return strcspn(board,v) >=3;

Which works but i need to rewrite the change turn function
since theres a weird bug that gives the win to the wrong player
*/

int searchHorizontal(char board[],char ch){
	//Compares the 3 char bytes for duplicates
	char temp[TABLE_SIZE];
	memset(temp,ch,TABLE_SIZE);

	return !memcmp(board,temp,TABLE_SIZE * sizeof(char));
}

void resetboard(char board[TABLE_SIZE][TABLE_SIZE]){
	//just clears the entire board 
	memset(board,' ',TABLE_SIZE * TABLE_SIZE);
}

void showboard(char board[TABLE_SIZE][TABLE_SIZE]){
	int boardY,boardX;
	for(boardY = 0; boardY < TABLE_SIZE; boardY++){
		//Draw Two elements of the board, check if it needs separation
		for(boardX = 0; boardX < TABLE_SIZE; boardX++) 
			printf("%c %c", board[boardY][boardX], boardX != TABLE_SIZE-1 ? '|':' ')	;
		
		//Check if last and draw line
		if(boardY != TABLE_SIZE-1) 
			printf("\n--------\n");
	}
	putchar('\n');
}

const char SPACE = ' ';
short turnNumber = 0;
char ch = 'X';

int main(int argc, char *argv[]) 
{
	//Table size limit
	if(TABLE_SIZE < 3)
		return 1;
	
	//initiate and populate the board
	char board[TABLE_SIZE][TABLE_SIZE];
	resetboard(board);

	//initialize all game variables	
	char message[50] = "TicTacToe time!\n";
	short row,col;

	do{
		//clear console and Show UI
		system ("CLS");
		puts(message);
		showboard(board);
		
		//Ask for input
		printf("\nPick two cordinates (x, y): ");
		scanf("%hu %hu",&row,&col);
		
		//Check if the position is picked
		if(board[row-1][col-1] != SPACE){
			strcpy(message,"Position picked, try again!\n");
			//"continue" resets the while loop
			continue;
		}
		
		//Draw the player choice
		board[row-1][col-1] = ch;
		
		//Check for a win
		if(searchHorizontal(board[row-1],ch))
			break;
		
		if(searchVertical(board,col-1,ch))
			break;
		
		if(searchDiagonal(board,ch)){
			break;
		}
		
		//Change player turn 		
		ch = (ch == 'O') ? 'X' : 'O';
		
	}while(++turnNumber != sizeof(board));
	// end game when the board is full;
	
	if(turnNumber == sizeof(board)){
		printf("Tie!");
		return 0;
	}
		
	printf("\n%c won in %d turns!\n",ch,turnNumber);
	
	return 0;
}
