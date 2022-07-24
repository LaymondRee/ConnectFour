/***************************************************************************
 * Program: connectfour.cpp
 * Author: Raymond Lee
 * Date: 03-14-21
 * Description: Plays game of connect four with 1-2 players and a board size
 * ranging from 4 by 4 to 20 by 20
 * Input: players, rows, cols
 * Output: connect four game
***************************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

int get_int();

void choices(int, char**, int &, int &, int &);
void create(char** &, int* &, int, int);
void del_board(char** &, int* &, int);
void print(char**, int, int, int*);
void computer(char**, int, int, int*);
void two_player(char**, int, int, int*);
void place(char**, int, int, int, int, int &);
void winner(char**, int, int, int, int, int, int &);

int main(int argc, char** argv){
	int players;
	int rows;
	int cols;
	char** board;
	int* nums;
	int again = 1;

	do{
		choices(argc, argv, players, rows, cols);
		create(board, nums, rows, cols);

		if(players == 1){ //Player chooses to play against computer
			computer(board, rows, cols, nums);
		}else if(players == 2){ //Player chooses to play against another player
			two_player(board, rows, cols, nums);
		}

		del_board(board, nums, rows);

		cout << "Would you like to play again? (1 - Yes), (0 - No): ";

		do{
			again = get_int(); //Error handling for data type
			if(again > 1){
				cout << "Please enter a valid integer: ";
			}
		}while(again > 1); //Error handling for integer range
	}while(again == 1); //Loops program if user wants to play again

	return 0;
}

/***************************************************************************
 * Function: choices
 * Description: processes all user inputs
 * Parameters: argc, argv, players, rows, cols
 * Pre-conditions: user choices
 * Post-conditions: returns valid choice inputs
***************************************************************************/
void choices(int argc, char** argv, int &players, int &rows, int &cols){
	if(argc == 4){
		players = atoi(argv[1]);
		rows = atoi(argv[2]);
		cols = atoi(argv[3]);
	}

	if((players > 2 || players < 1) || (rows > 20 || rows < 4) || (cols > 20 || cols < 4) || (argc > 4)){
		cout << "Would you like to play against the computer (1) or with two players (2)?" << endl;
		do{
			players = get_int(); //Error handling for data type
			if(players > 2 || players < 1)
				cout << "Please enter a valid integer: ";
		}while(players > 2 || players < 1); //Error handling for integer range

		cout << "How many rows would you like (4 to 20)?" << endl;
		do{ 
			rows = get_int(); //Error handling for data type
			if(rows > 20 || rows < 4)
				cout << "Please enter a valid integer: ";
		}while(rows > 20 || rows < 4); //Error handling for integer range

		cout << "How many columns would you like (4 to 20)?" << endl;
		do{
			cols = get_int(); //Error handling for data type
			if(cols > 20 || cols < 4)
				cout << "Please enter a valid integer: ";
		}while(cols > 20 || cols < 4); //Error handling for integer range
	}	
}

/***************************************************************************
 * Function: create
 * Description: creates the array where pieces will be placed
 * Parameters: board, nums, rows, cols
 * Pre-conditions: size of the board
 * Post-conditions: returns array for board and column numbers
***************************************************************************/
void create(char** &board, int* &nums, int rows, int cols){
	board = new char*[rows]; //2d dynamic array declaration
	for(int i = 0; i < rows; i++){
		board[i] = new char[cols];
		for(int j = 0; j < cols; j++){ //Initialize all elements to ' '
			board[i][j] = ' '; 
		}
	}

	nums = new int[cols]; //Dynamic array numbering columns
	for(int i = 0; i < cols; i++){
		nums[i] = i;
	}
}

/***************************************************************************
 * Function: del_board
 * Description: deletes the arrays
 * Parameters: board, nums, rows
 * Pre-conditions: the rows of the board
 * Post-conditions: deletes the arrays
***************************************************************************/
void del_board(char** &board, int* &nums, int rows){
	for(int i = 0; i < rows; i++){ //Delete all created dynamic arrays
		delete[] board[i];
		board[i] = NULL;
	}
	delete[] board;
	board = NULL;

	delete[] nums;
	nums = NULL;
}

/***************************************************************************
 * Function: print
 * Description: prints the board to the terminal
 * Parameters: board, rows, cols, nums
 * Pre-conditions: size of the board
 * Post-conditions: checkered board with placed pieces
***************************************************************************/
void print(char** board, int rows, int cols, int* nums){
	for(int i = 0; i < cols; i++){
		if(nums[i] < 10) //handles displacement of two digit numbers
			cout << "  " << nums[i] << " ";
		else
			cout << "  " << nums[i];
	}

	cout << endl;

	for(int i = 0; i < rows; i++){ //Prints the 2d array with a checkered pattern
		for(int j = 0; j < cols; j++){
			if(i % 2 == 0 && j % 2 == 0)
				cout << "|\033[30;47m " << board[i][j] << " ";
			else if(i % 2 == 1 && j % 2 == 1)
				cout << "|\033[30;47m " << board[i][j] << " ";
			else
				cout << "|\033[0m " << board[i][j] << " ";
			cout << "\033[0m";
		}
		cout << endl;
	}
}

/***************************************************************************
 * Function: computer
 * Description: player plays against computer
 * Parameter: board, rows, cols, nums
 * Pre-conditions: the board
 * Post-conditions: plays game with computer
***************************************************************************/
//Function is over 20 lines because I thought it was counterintuitive to make a function for player and computer
void computer(char** board, int rows, int cols, int* nums){
	srand(time(NULL));
	int place_p; //Column piece is placed in
	int first; //Choice for player or computer to go first
	int turn = 1; //The current player placing
	int loop = 1; //Creates infinite loop
	int p_row; //The row the piece ended up placing on
	int win = 0; //Checks to see if game is over
	string buffer; 

	cout << "Would you like the first move? (1 - Yes), (2 - No)" << endl;

	do{
		first = get_int();
		if(first > 2 || first < 1)
			cout << "Please enter a valid integer: ";
	}while(first > 2 || first < 1);

	print(board, rows, cols, nums);

	do{
		if(first == 1){ //Skips player turn first loop if player doesn't choose to go first
			cout << "What column does player 1 want to place in?" << endl;
			do{ //Error handling for piece placement
				place_p = get_int();
				if(place_p < 0 || place_p > cols - 1 || board[0][place_p] != ' ')
					cout << "Please enter a valid integer: ";
			}while(place_p < 0 || place_p > cols - 1 || board[0][place_p] != ' ');

			place(board, rows, cols, place_p, turn, p_row);
			print(board, rows, cols, nums);
			winner(board, rows, cols, place_p, turn, p_row, win);

			if(win == 1 && turn == 1){ //Returns to main function when player 1 wins
				cout << "Player 1 has won!" << endl;
				return;
			}else if(win == 2){ //Returns to main function when there are no more available spaces
				cout << "It is a tie!" << endl;
				return;
			}
		}
		first = 1; //Ensures player 1 gets to place for the remainder of the game
		turn = 2; //Changes the who is placing

		cout << "Computer is placing, press enter to continue" << endl;
		getline(cin, buffer);
 
		do{ //Error handling for piece placement
			place_p = rand() % cols; //Generate a random number between 0 and total columns
		}while(place_p < 0 || place_p > cols - 1 || board[0][place_p] != ' ');

		place(board, rows, cols, place_p, turn, p_row);
		print(board, rows, cols, nums);
		winner(board, rows, cols, place_p, turn, p_row, win);

		if(win == 1 && turn == 2){ //Returns to main function when computer wins
			cout << "The computer has won!" << endl;
			return;
		}else if(win == 2){ //Returns to main function when there are no more available spaces
			cout << "It is a tie!" << endl;
			return;
		}
		turn = 1; //Changes who is placing

	}while(loop = 1); //Continues to loop until there is a winner or board is filled
}

/***************************************************************************
 * Function: two_player
 * Description: player plays against another player
 * Parameter: board, rows, cols, nums
 * Pre-conditions: the board
 * Post-conditions: plays game with another player
***************************************************************************/
//Function is over 20 lines because I thought it was counterintuitive to make a function for each player
void two_player(char** board, int rows, int cols, int* nums){
	int place_p; //Column piece is placed in
	int turn = 1; //The current playing placing
	int loop = 1; //Creates infinite loop
	int p_row; //The row the piece ended up placing in
	int win = 0; //Checks to see if game is over

	print(board, rows, cols, nums);

	do{
		cout << "What column does player 1 want to place in?" << endl;
		do{ //Error handling for piece placement
			place_p = get_int();
			if(place_p < 0 || place_p > cols - 1 || board[0][place_p] != ' ')
				cout << "Please enter a valid integer: ";
		}while(place_p < 0 || place_p > cols - 1 || board[0][place_p] != ' ');

		place(board, rows, cols, place_p, turn, p_row);
		print(board, rows, cols, nums);
		winner(board, rows, cols, place_p, turn, p_row, win);

		if(win == 1 && turn == 1){ //Returns to main function when player 1 wins
			cout << "Player 1 has won!" << endl;
			return;
		}else if(win == 2){ //Returns to main function when there are no available spaces
			cout << "It is a tie!" << endl;
			return;
		}
		turn = 2; //Changes who is placing

		cout << "What column does player 2 want to place in?" << endl;
		do{ //Error handling for piece placement
			place_p = get_int();
			if(place_p < 0 || place_p > cols - 1 || board[0][place_p] != ' ')
				cout << "Please enter a valid integer: ";
		}while(place_p < 0 || place_p > cols - 1 || board[0][place_p] != ' ');

		place(board, rows, cols, place_p, turn, p_row);
		print(board, rows, cols, nums);
		winner(board, rows, cols, place_p, turn, p_row, win);

		if(win == 1 && turn == 2){ //Returns to main function when player 2 wins
			cout << "Player 2 has won!" << endl;
			return;
		}else if(win == 2){ //Returns to main function when there are no available spaces
			cout << "It is a tie!" << endl;
			return;
		}
		turn = 1; //Changes who is placing

	}while(loop = 1); //Continues to loop until there is a winner or board is filled
}

/***************************************************************************
 * Function: place
 * Description: places a piece on the board
 * Parameter: board, rows, cols, place_p, turn, p_row
 * Pre-conditions: the column the piece is placed in
 * Post-conditions: places piece in specified location
***************************************************************************/
void place(char** board, int rows, int cols, int place_p, int turn, int &p_row){
	if(turn == 1){ //Places piece from bottom to top for X's
		for(int i = 0; i < rows; i++){
			if(board[rows - i - 1][place_p] == ' '){
				board[rows - i - 1][place_p] = 'X';
				p_row = rows - i - 1;
				break;
			}
		}
	}else if(turn == 2){ //Places piece from bottom to top for O's
		for(int i = 0; i < rows; i++){
			if(board[rows - i - 1][place_p] == ' '){
				board[rows - i - 1][place_p] = 'O';
				p_row = rows - i - 1;
				break;
			}
		}
	}
}

/***************************************************************************
 * Function: winner
 * Description: determines if the game has a winner or tie
 * Parameter: b, rows, cols, c, turn, r, win
 * Pre-conditions: location of most recent piece
 * Post-conditions: determines if game is over
***************************************************************************/
//Function is over 20 lines because each piece is evaluated right after it is placed and in order to avoid seg faults I had to divide the board into quadrants
void winner(char** b, int rows, int cols, int c, int turn, int r, int &win){
	int tie = 0; //Checks if top row is filled

	if(r < rows - 3){
		if((b[r][c] == b[r+1][c] && b[r+1][c] == b[r+2][c]) && b[r+2][c] == b[r+3][c]){ //Vertical
			win = 1;
			return;
		}
	}

	if(c < cols - 3){
		if((b[r][c] == b[r][c+1] && b[r][c+1] == b[r][c+2]) && b[r][c+2] == b[r][c+3]){ //Horizantal
			win = 1;
			return;
		}
	}

	if(c > 2){
		if((b[r][c] == b[r][c-1] && b[r][c-1] == b[r][c-2]) && b[r][c-2] == b[r][c-3]){ //Horizantal
			win = 1;
			return;
		}
	}

	if(c < cols - 2){
		if((b[r][c-1] == b[r][c] && b[r][c] == b[r][c+1]) && b[r][c+1] == b[r][c+2]){ //Horizantal
			win = 1;
			return;
		}
	}

	if(c > 1){
		if((b[r][c+1] == b[r][c] && b[r][c] == b[r][c-1]) && b[r][c-1] == b[r][c-2]){ //Horizantal
			win = 1;
			return;
		}
	}

	if(r > 2 && c > 2){ //Bottom Right
		if((b[r][c] == b[r-1][c-1] && b[r-1][c-1] == b[r-2][c-2]) && b[r-2][c-2] == b[r-3][c-3]){ //Diagonal
			win = 1;
			return;
		}
	}

	if(r > 1 && c > 1 && r < rows - 1){ //Bottom Right
		if((b[r+1][c+1] == b[r][c] && b[r][c] == b[r-1][c-1]) && b[r-1][c-1] == b[r-2][c-2]){ //Diagonal
			win = 1;
			return;
		}
	}

	if(r > 2 && c < cols - 3){ //Bottom Left
		if((b[r][c] == b[r-1][c+1] && b[r-1][c+1] == b[r-2][c+2]) && b[r-2][c+2] == b[r-3][c+3]){ //Diagonal
			win = 1;
			return;
		}
	}

	if(r > 1 && c < cols - 2 && r < rows - 1){ //Bottom Left
		if((b[r+1][c-1] == b[r][c] && b[r][c] == b[r-1][c+1]) && b[r-1][c+1] == b[r-2][c+2]){ //Diagonal
			win = 1;
			return;
		}
	}

	if(r < rows - 3 && c < cols - 3){ //Top Left
		if((b[r][c] == b[r+1][c+1] && b[r+1][c+1] == b[r+2][c+2]) && b[r+2][c+2] == b[r+3][c+3]){ //Diagonal
			win = 1;
			return;
		}	
	}

	if(r < rows - 2 && c < cols - 2 && r > 0){ //Top Left
		if((b[r-1][c-1] == b[r][c] && b[r][c] == b[r+1][c+1]) && b[r+1][c+1] == b[r+2][c+2]){ //Diagonal
			win = 1;
			return;
		}	
	}

	if(r < rows - 3 && c > 2){ //Top Right
		if((b[r][c] == b[r+1][c-1] && b[r+1][c-1] == b[r+2][c-2]) && b[r+2][c-2] == b[r+3][c-3]){ //Diagonal
			win = 1;
			return;
		}
	}

	if(r < rows - 3 && c > 2 && r > 0){ //Top Right
		if((b[r-1][c+1] == b[r][c] && b[r][c] == b[r+1][c-1]) && b[r+1][c-1] == b[r+2][c-2]){ //Diagonal
			win = 1;
			return;
		}
	}


	for(int i = 0; i < cols; i++){ //Determine if top row is completely filled
		if(b[0][i] != ' ')
			tie += 1;
	}

	if(tie == cols){ //Returns tie condition if top row is filled
		win = 2;
		return;
	}
}

/***************************************************************************
 * Function: get_int
 * Description: error handling for integer inputs
 * Pre-conditions: gets input from user
 * Post-conditions: returns positive integer
***************************************************************************/
bool positive_int(string input){
	for(int i = 0; i < input.length(); i++){
		if(!(input[i] >= 48 && input[i] <= 57)){
			cout << "Please enter a valid integer: ";
			return false;
		}
	}
	return true;
}

int get_int(){
	string input;
	bool flag = false;

	do{
		getline(cin, input);
		flag = positive_int(input);
	}while(flag == false);
	return atoi(input.c_str());
}
