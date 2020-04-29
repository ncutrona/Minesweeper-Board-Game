
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;


//Function Comments are commented for each function in the code below, not in the declarations.
int **makeBoard(int dimension);
int getSize(int &size);
void printBoard(int **board, int dimension_val);
int **placeBombs(int **board, int size_val);
bool isInBounds(int row, int col, int size);
void placeCounts(int **board, int size);
char **makeVisibleBoard(int size);
void printVisible(char **my_array, int dimension_val);
bool chooseSquare(int **board1, char **board2, int size);
bool addBomb(char **visBoard, int size, int *bomb_counter);
void removeBomb(char **visBoard, int size, int *bombs_found);
bool checkForWin(int **my_board, char **vis_board, int size);
void removeBoard(int **my_board, int size);
void removeVisible(char **my_board, int size);


//Main Function - NOTE: this was provided for the programmer.
int main() {
	srand(time(NULL));
	int size = 0;
	int bombsfound = 0;
	getSize(size);
	cout << "Size is " << size << endl;
	int **mat = makeBoard(size);
	//printBoard(mat, size);
	placeBombs(mat,size);
	//printBoard(mat, size);
	placeCounts(mat, size);
	//printBoard(mat, size);
	char **visible = makeVisibleBoard(size);
	/* For testing purposes:
	printVisible(visible,size);
	chooseSquare(mat,visible,size);
	printVisible(visible,size);
	addBomb(visible, size, &bombsfound);
	printVisible(visible,size);
	removeBomb(visible, size, &bombsfound);
	printVisible(visible,size);
	checkForWin(mat, visible, size);
	 End of testing */
	char c;
	bool flag = true;
	char again = 'n';
	while (flag) {
		printVisible(visible,size);
		cout << "Choose: A for choose square, B for add Bomb, C for remove bomb: " << endl;
		cin >> c;
		if (c == 'A' || c == 'a') {
			flag = chooseSquare(mat, visible, size);
			if (!flag) {
				cout << "YOU LOSE! YOU HIT A BOMB!!" << endl;
				printBoard(mat, size);
				cout << "\n\nDo you want to play again?" <<endl;
				cin >> again;
			}
		}
		if (c == 'B' || c == 'b') {
			if (addBomb(visible, size, &bombsfound)) {
				cout << "Bombs found: " << bombsfound << endl;
				cout << "You appear to think you've found all the bombs" << endl;
				cout << "Choose: D for check for Win, or C for remove bomb:" << endl;

				cin >> c;
				if (c == 'D' || c == 'd') {
					if (checkForWin(mat, visible, size)) {
						cout <<"YOU WON!!! WOO WOO!!!" << endl;
						printBoard(mat, size);
					}
					else {
						cout <<"Sorry, you were wrong. Boo hoo." << endl;
						printBoard(mat, size);
					}
					removeBoard(mat, size);
					removeVisible(visible, size);
					bombsfound = 0;
					flag = false;
				}
				cout << "\n\nDo you want to play again? (y or n)" <<endl;
				cin >> again;
			}
		}
		if (c == 'C' || c == 'c') {
			removeBomb(visible, size, &bombsfound);
		}
		if (!flag && (again == 'y' || again == 'Y')) {
			flag = true;
			bombsfound = 0;
			getSize(size);
			cout << "Size is " << size << endl;
			mat = makeBoard(size);
			//printBoard(mat, size); - for testing purposes
			placeBombs(mat,size);
			//printBoard(mat, size); - for testing purposes
			placeCounts(mat, size);
			//printBoard(mat, size); - for testing purposes
			visible = makeVisibleBoard(size);
		}
	}
	return 0;
}

//Function: getSize --> Returns an integer that is randomly generated between the range of 7 and 20 (not including 20). The argument for this function was a pointer
//to an integer. This function uses call by reference to modify the size of my game board.
int getSize(int &size){
	size = 7 + rand() %(13);
	return size;
}


//Function: makeBoard --> Returns an integer matrix that is the game board. The argument for this function is an integer representing the size from the getSize function.
//With that size, the matrix is created on the heap. Note, this is the behinid the scenes board not visible to the player.

int **makeBoard(int dimension){
	int i,j;
	int value =0;
	int **board = NULL;
	board = new int*[dimension];
	for(i = 0; i < dimension; i++) {
	    board[i] = new int[dimension];
	}

	for(i=0; i<dimension; i++){
		for(j=0; j<dimension;j++){
			board[i][j]=value;
		}
	}


	return board;

}


//Function: printBoard --> This function returns nothing (void). The argument for this function is an integer matrix and and integer representing the size of the board.
//This function simply prints out the board to the console when called, turining all values of 0 of the board to empty spaces.
void printBoard(int **my_array, int dimension_val){
	int i,j;

	for(i=0; i<dimension_val; i++){
		for(j=0;j<dimension_val; j++){
			if(my_array[i][j]==0){
				cout<<' '<<" ";
				}
			else{
				cout<<my_array[i][j]<<" ";
			}

			}
			cout<<endl;
		}
	}

//Function: placeBombs --> This function returns an integer matrix of the board with size + 1 bombs placed on it randomly. The argument for this function is
//an integer matrix of the board and an integer representing the size of the board. Some more comments are listed below.
int **placeBombs(int **my_board, int size_val){
	int bomb_label = 9; //updside down b
	int counter = size_val +1;
	int count =0;
	int border = size_val;
	//assigning random numbers to be coordinates on the board in order to place size + 1 bombs randomly.
	while(count < counter){
		int x = rand()%border;
		int y = rand()%border;
		while(my_board[x][y]==bomb_label){
			 x = rand()%border;
			 y = rand()%border;
		}

		if(my_board[x][y] != bomb_label){
			my_board[x][y]=bomb_label;
		}
		count++;
	}

	return my_board;
}

//Function: isInBounds --> This function returns a boolean value depending on if the coordinate called is in bounds of the board created. This is simply a helper function to
//the place counts function. The arguments for this function are three integers representing the rows, cols, and size of the board.
bool isInBounds(int row, int col, int size){
	return ((row>=0)&&(row<size)&&(col >=0)&&(col<size));
}


//Function: placeCounts --> This function does not return anything (void). The arguments for this function are an integer matrix representing the board and an integer
//representing the size of the board. This function uses the isInBounds function to check each index that is not a bomb and count the total bombs around the index.
void placeCounts(int **board, int size){
	for(int i = 0; i <size; i++){
		for(int j = 0; j <size; j++){
			int bomb_label = 9;
			int count = 0;

			if(board[i][j]!=bomb_label){
			if(isInBounds(i-1,j,size)){
				if(board[i-1][j]==bomb_label){
					count++;
						}
			}

			if(isInBounds(i+1,j,size)){
				if(board[i+1][j]==bomb_label){
					count++;
							}
						}

			if(isInBounds(i,j-1,size)){
				if(board[i][j-1]==bomb_label){
					count++;
									}
						}

			if(isInBounds(i,j+1,size)){
				if(board[i][j+1]==bomb_label){
					count++;
					}
						}

			if(isInBounds(i-1,j-1,size)){
				if(board[i-1][j-1]==bomb_label){
					count++;
									}
						}

			if(isInBounds(i+1,j+1,size)){
				if(board[i+1][j+1]==bomb_label){
					count++;
									}
						}

			if(isInBounds(i-1,j+1,size)){
				if(board[i-1][j+1]==bomb_label){
					count++;
									}
						}

			if(isInBounds(i+1,j-1,size)){
				if(board[i+1][j-1]==bomb_label){
					count++;
									}
						}
			board[i][j]=count;

		}}
	}
}


//Function: makeVisibleBoard --> Thsi function returns a character matrix representing the board of the game but in characters, making it visible to the player.
//The argument for this function is an integer representing the size of the board.
char **makeVisibleBoard(int size){
	int i,j;
	int hyphen ='-';
	char **board = NULL;
	board = new char*[size];
	for(i = 0; i < size; i++) {
	    board[i] = new char[size];
	}

	for(i=0; i<size; i++){
		for(j=0; j<size;j++){
			board[i][j]=hyphen;
		}
	}

	return board;
}

//Function: printVisible --> This function returns nothing (void). The arguments for this function are the character array, creating above and an integer representing the size
//of the board. This function prints out the board to user while they are playing.
void printVisible(char **my_array, int dimension_val){
	int i,j;
	char value = '-';

	for(i=0; i<dimension_val; i++){
		for(j=0;j<dimension_val; j++){
			if(my_array[i][j]==value)
			cout<<value<<" ";
			else{
				cout<<my_array[i][j]<<" ";

			}
			}
			cout<<endl;
		}
	}


//Function: chooseSquare --> This function returns a boolean value (false if the square chosen with cin and cout was a bomb & true if the square chosen was not a bomb).
//This function also converts the number from the integer array to a character and places it on the char board visible to the user. The argument for this function is
//the character matrix, integer matrix, and an integer representing the size of the board.
bool chooseSquare(int **board1, char **board2, int size){
	int bomb_label =9;
	int x;
	int y;

	cout<<"User, choose a row coordinate between 0 and "<<size-1<<": ";
	cin>>x;
	if(x<0 || x > size-1){
		while(x<0 || x > size-1){
			cout<<"User, choose a row coordinate between 0 and "<<size-1<<": ";
			cin>>x;
	}
	}

	cout<<"User, choose a column coordinate between 0 and "<<size-1<<": ";
	cin>>y;
	if(y<0 || y > size-1){
		while(y<0 || y > size-1){
			cout<<"User, choose a column coordinate between 0 and a"<<size<<": ";
			cin>>y;
		}
		}


	char c = '0' + board1[x][y];
	board2[x][y] = c;

	if(board1[x][y]==bomb_label){
		return false;
	}
	else{
		return true;
	}

}

//Function: addBomb --> This function returns a boolean value (true if the number of bombs found is equal to size + 1 & false if not). This function allows the user to
//add a bomb label to the visible board if they think they have found a bomb using cin and cout. The argumets for this function is the character matrix of the board,
//the integer representing the size, and a pointer to the number of bombs found (an integer).
bool addBomb(char **visBoard, int size, int *bombs_found){
	int bomb_label = 9;
	int num_bombs = size+1;
	int x;
	int y;
	bool final = false;
	cout<<"User, choose a row coordinate between 0 and "<<size-1<<": ";
	cin>>x;
	if(x<0 || x > size-1){
		while(x<0 || x > size-1){
			cout<<"User, choose a row coordinate between 0 and "<<size-1<<": ";
			cin>>x;
		}
		}
	cout<<"User, choose a column coordinate between 0 and "<<size-1<<": ";
	cin>>y;
	if(y<0 || y > size-1){
		while(y<0 || y > size-1){
			cout<<"User, choose a column coordinate between 0 and "<<size-1<<": ";
			cin>>y;
			}
			}
	//want to see if the coordinates equal the bomb.
	if(visBoard[x][y]== '-'){
		visBoard[x][y] = '0'+bomb_label;
		*bombs_found=*bombs_found+1;
	}

	//makes sure the count is going up if the user accidentally chooses the same spot.
	else{
		cout<<"You have already placed a bomb here."<<endl;
	}

	if(*bombs_found == num_bombs){
		final = true;
	}

	return final;

}


//Function: removeBomb --> This function returns nothing (void). The arguments for this function are the character matrix of the board, an integer representing the size of the
//board, and also a pointer to the number of bombs found. This function allows the user to remove a bomb from the board (after they chose that square as a bomb previously)
//using cin and cout. The function will remove the bomb label and return the square to a hyphen.
void removeBomb(char **visBoard, int size, int *bombs_found){
	char hyphen = '-';
	char bomb_label = '9';
	int x;
	int y;

	cout<<"User, choose a row coordinate between 0 and "<<size-1<<": ";
	cin>>x;
	if(x<0 || x > size-1){
		while(x<0 || x > size-1){
			cout<<"User, choose a row coordinate between 0 and "<<size-1<<": ";
			cin>>x;
		}
		}

	cout<<"User, choose a column coordinate between 0 and "<<size-1<<": ";
	cin>>y;
		if(y<0 || y > size-1){
			while(y<0 || y > size-1){
				cout<<"User, choose a column coordinate between 0 and "<<size-1<<": ";
				cin>>y;
			}
			}

	if(visBoard[x][y]==bomb_label){
		visBoard[x][y] = hyphen;
		*bombs_found=*bombs_found-1;
		cout<<*bombs_found<<"Test"<<endl;
	}

	else{
		cout<<"You did not remove a bomb.";
	}

	cout<<endl;
}


//Function: checkForWin --> This function returns a boolean value (true if the user has found all the bombs, and false if they have not). The arguments for this function are
//the integer board, the character board, and the an integer representing the size of the board. Essentially, if all '9''s on the character board match the 9's on the
//integer board, the player has won the game.
bool checkForWin(int **my_board, char **vis_board, int size){
	int i;
	int j;
	int num_bomb_label = 9;
	char char_bomb_label = '9';

	for(i =0; i<size; i++){

		for(j=0;j<size;j++){
			if(my_board[i][j]!=num_bomb_label && vis_board[i][j]!=char_bomb_label){
				continue;
			}

			else if(my_board[i][j]==num_bomb_label && vis_board[i][j]!=char_bomb_label){
				return false;
			}

			else if(i == size-1 && j == size-1){
				return true;

			}

		}
	}
}


///Function: removeBoard --> The function does not return anything (void). The argument for this function is the integer array. This function simply removes the array off
//the heap.
void removeBoard(int **my_board, int size){
	delete[] my_board;
}

///Function: removeVisible --> The function does not return anything (void). The argument for this function is the character array. This function simply removes the array off
//the heap.
void removeVisible(char **my_board, int size){
	delete[] my_board;
}





