#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_board.h"
#include "scrabble.h"

//gets the points for a word
int get_points(char word[]){
	int res = 0;

	int points[] = {1, 3, 3, 2, 1, 4, 2, 4,1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
	int n = strlen(word);

	for(int i = 0; i < n; i++){

		res += points[word[i]-'A'];
	} 
	return res;
}

//gets the points cosidering the bonus table
int get_bonus_points(int points, int x, int y, char* word, int direction, char sir1[], char sir2[]){

	int length = strlen(word);

	if(direction == 0)
			
		for(int j = 0; j < length; j++){

			if((bonus_board[y][x + j] == 1) && (strstr(word, sir1)) != NULL)
				points *= 2; 

			if(bonus_board[y][x + j] == 2 && (strstr(word, sir2)) != NULL){


				if(strcmp(strstr(word, sir2), sir2) == 0)
					points *= 3;
			}
		}
	
	else
		for(int j = 0; j < length; j++){
		
			if((bonus_board[y + j][x] == 1) && (strstr(word, sir1)) != NULL)
				points *= 2; 


			if(bonus_board[y + j][x] == 2 && strstr(word, sir2) != NULL){
			
				if(strcmp(strstr(word, sir2), sir2) == 0)
					points *= 3;

			}
		}

	return points;
}

//allocate memory for the input data
void alloc_read(int **x, int **y, int **direction, char ***words, int n){
	*x = (int*)calloc(n, sizeof(int));
	if(*x == NULL){
		printf("Eroare alocare\n");
		exit(EXIT_FAILURE);
	}

	*y = (int*)calloc(n, sizeof(int));
	if(*y == NULL){
		printf("Eroare alocare\n");
		exit(EXIT_FAILURE);
	}

	*direction = (int*)calloc(n, sizeof(int));
	if(*direction == NULL){
		printf("Eroare alocare\n");
		exit(EXIT_FAILURE);
	}

	*words = (char**)calloc(n, sizeof(char*));
	if(*words == NULL){
		printf("Eroare alocare\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < n; i++){
		(*words)[i] = (char*)calloc(32, sizeof(char));
		if((*words)[i] == NULL){
			for(int j = 0; j < i; j++)
				free((*words)[j]);
			printf("Eroare alocare\n");
			exit(EXIT_FAILURE);
		}
	}
}

//free the memory from input data
void free_read(int **x, int **y, int **direction, char ***words, int n){
	free(*x);
	free(*y);
	free(*direction);
	for(int i = 0; i < n; i++)
		free((*words)[i]);
	free(*words);
}

//read input data
void read(char str[], int **x, int **y, int **direction, char ***words, int i){

	char *p, sep[2] = " ";

	p = strtok(str, sep);

	(*y)[i] = atoi(p);

	p = strtok(NULL, sep);

	(*x)[i] = atoi(p);

	p = strtok(NULL, sep);

	(*direction)[i] = atoi(p);

	p = strtok(NULL, sep);

	if(p[strlen(p) - 1] == '\n' || p[strlen(p) - 1] == ' ')
		strncpy((*words)[i], p, strlen(p) - 1);
	else
		strcpy((*words)[i], p);
}

//check if a word is already on int table
int word_is_used(char **words, char word[], int n){

	for(int i = 0; i < n; i++)

		if(strcmp(words[i], word) == 0)

			return 1;

	return 0;
}

//get the available direction for a word
int get_direction(char board[BOARD_SIZE][BOARD_SIZE], int line, int col, char word[]){

	int okL = 0, okC = 0, n = strlen(word);

	if(line == -1)
		return -2;

	//checks if line is available
	if((col + n) <= 15){

		for(int i = 1; i < n && okL == 0; i++)
			if(board[line][col + i] != '.')
				okL = 1;
	}
	else
		okL = 1;

	//checks if the column is available
	if((line + n) <= 15){

		for(int i = 1; i < n && okC == 0; i++)
			if( board[line + i][col] != '.')
				okC = 1;
	}
	else
		okC = 1;

	//if line is available
	if(okL == 0)
		return 0;

	if(okC == 0)
		return 1;
	
	return -1;
}

//checks if a letter is on board
void letter_on_board(char board[BOARD_SIZE][BOARD_SIZE], char letter, 
	int *line, int *col, int counter){

	//used to get coordinates of the counterth appearance of letter
	int c = 0;
	
	for(int i = 0; i < BOARD_SIZE; i++){

		for(int j = 0; j < BOARD_SIZE; j++){

			if(board[i][j] == letter){

				c++;

				if(c == counter){
					*line = i;
					*col = j;
				
					return;
				}
			}
		}
	}

	*line = -1;
	*col = -1;
}

int main(void){

	int taskNo, *x, *y, *direction, okNr = 0, okStr = 0, n, i = 0;

	char board[BOARD_SIZE][BOARD_SIZE], str[100], **input_words, 
		cifre[] = "1234567890", sir1[10], sir2[10];

	int player1 = 0, player2 = 0;

	//create an empty board
	for(int x = 0; x < BOARD_SIZE; x++){

		for(int y = 0; y < BOARD_SIZE; y++){

			board[x][y] = '.';
		}
	}
	
	//read input data
	while(fgets(str, sizeof(str), stdin)){

		if(strstr(str, " ") == NULL){

			if(strchr(cifre, str[0])){

				if(okNr == 0){

					okNr = 1;

					taskNo = atoi(str);

				}
				else{
					n = atoi(str);
					alloc_read(&x, &y, &direction, &input_words, n);
				}

			}
			else{
				if(okStr == 0){
					strncpy(sir1, str, strlen(str) - 1);
					okStr = 1;
				}
				else
					strncpy(sir2, str, strlen(str) - 1);
			}
		}
		else{
			read(str, &x, &y, &direction, &input_words, i);
			i++;
		}
	}

	sir2[strlen(sir2) - 1] = 0;

	if(taskNo == 0){
		print_board(board);
		return 0;
	}

	if(taskNo == 1){

		for(int i = 0; i < n; i++){

			int length = strlen(input_words[i]);

			if(direction[i] == 0)

				for(int j = 0; j < length; j++)
					board[y[i]][x[i] + j] = input_words[i][j];

			else
				for(int j = 0; j < length; j++)
					board[y[i] + j][x[i]] = input_words[i][j];
		}
		print_board(board);
	}

	if(taskNo == 2){

		for(int i = 0; i < n; i++){

			if(i % 2 == 0)
				player1 += get_points(input_words[i]);
			
			else
				player2 += get_points(input_words[i]);
		}

		printf("Player 1: %d Points\n", player1);
		printf("Player 2: %d Points\n", player2);

	}

	if(taskNo == 3){

		for(int i = 0; i < n; i++){
			
			if(i % 2 == 0)

				player1 += get_bonus_points(get_points(input_words[i]), 
					x[i], y[i], input_words[i], direction[i], sir1, sir2);
			
			else
				player2 += get_bonus_points(get_points(input_words[i]), 
					x[i], y[i], input_words[i], direction[i], sir1, sir2);
		}
		printf("Player 1: %d Points\n", player1);
		printf("Player 2: %d Points\n", player2);
	}

	if(taskNo == 4){

		//add words to the map
		for(int i = 0; i < n; i++){

			int length = strlen(input_words[i]);

			if(direction[i] == 0)

				for(int j = 0; j < length; j++)
					board[y[i]][x[i] + j] = input_words[i][j];

			else
				for(int j = 0; j < length; j++)
					board[y[i] + j][x[i]] = input_words[i][j];
		}

		int line, col, dir, ok4 = 0;

		//iterate over the words
		for(int i = 0; i < NUM_WORDS && ok4 == 0; i++){

			int counter = 0;
			dir = 0;

			if(word_is_used(input_words, words[i], n) == 0){

				while(dir != -2 && ok4 == 0){

					counter++;

					letter_on_board(board, words[i][0], &line, &col, counter);

					dir = get_direction(board, line, col, words[i]);

					//if word respects the conditions
					if(dir != -1 && dir != -2){

						int length = strlen(words[i]);

						if(dir == 0)

							for(int j = 0; j < length; j++)
								board[line][col + j] = words[i][j];

						else
							for(int j = 0; j < length; j++)
								board[line + j][col] = words[i][j];
						//a word has been added
						ok4 = 1;
					}
				}

			}
		}
		print_board(board);
	}

	if(taskNo == 5){

		//compute the table
		for(int i = 0; i < n; i++){

			if(i % 2 == 0)

				player1 += get_bonus_points(get_points(input_words[i]), 
					x[i], y[i], input_words[i], direction[i], sir1, sir2);
			
			else
				player2 += get_bonus_points(get_points(input_words[i]), 
					x[i], y[i], input_words[i], direction[i], sir1, sir2);
		

			int length = strlen(input_words[i]);

			if(direction[i] == 0)

				for(int j = 0; j < length; j++)
					board[y[i]][x[i] + j] = input_words[i][j];

			else
				for(int j = 0; j < length; j++)
					board[y[i] + j][x[i]] = input_words[i][j];
		}
		

		int necessary_points = player1 - player2;

		int  dir, ok4 = 0, max = 0, lmax, colmax, dm;
		char word[100];

		//iterate over the board
		for(int x = 0; x < BOARD_SIZE && ok4 == 0; x++)

			for(int y = 0; y < BOARD_SIZE && ok4 == 0; y++){

				char letter = board[x][y];

				//if there is a letter on it
				if(letter != '.')
					
					for(int i = 0; i < NUM_WORDS && ok4 == 0; i++){

						dir = 0;

						//the word begins with the given letter
						//and is not on the board
						if(words[i][0] == letter && 
							word_is_used(input_words, words[i], n) == 0){

							dir = get_direction(board, x, y, words[i]);

							//the direction is valid
							if(dir >= 0){
						
								int points = get_bonus_points(get_points(words[i]),
								 y, x, words[i], dir, sir1, sir2);

								if(points > max && points >= necessary_points){
									//get the max points
									max = points;
									strncpy(word, words[i], strlen(words[i]) + 1);
									lmax = x;
									colmax = y;
									dm = dir;
								}
							}
							
						}
					}
				
			}
		

		//if there is no word found
		if(max == 0)
			printf("Fail!\n");
		else{
			//add word
			int length = strlen(word);

			if(dm == 0)

				for(int j = 0; j < length; j++)
					board[lmax][colmax + j] = word[j];

			else
				for(int j = 0; j < length; j++)
					board[lmax + j][colmax] = word[j];

			print_board(board);
		}
	}

	if(taskNo == 6){

		int  dir, ok4 = 0;
		
		//get an array of words to store the words on the table
		char **board_words = (char**)calloc(2 * n, sizeof(char*));
		if(board_words == NULL){
			printf("Eroare alocare\n");
			exit(EXIT_FAILURE);
		}

		for(int i = 0; i < 2 * n; i++){
			(board_words)[i] = (char*)calloc(32, sizeof(char));
			if(board_words[i] == NULL){
				for(int j = 0; j < i; j++)
					free(board_words[j]);
				printf("Eroare alocare\n");
				exit(EXIT_FAILURE);
			}
		}

		int idx = 0;

		for(int i = 0; i < n; i++){

			char word[100] = "";
			int max = 0, lmax = 0, colmax = 0, dm = 0;

			int length = strlen(input_words[i]);

			//get points for player1
			player1 += get_bonus_points(get_points(input_words[i]),
			 x[i], y[i], input_words[i], direction[i], sir1, sir2);

			strcpy(board_words[idx++], input_words[i]);

			if(direction[i] == 0)

				for(int j = 0; j < length; j++)
					board[y[i]][x[i] + j] = input_words[i][j];

			else
				for(int j = 0; j < length; j++)
					board[y[i] + j][x[i]] = input_words[i][j];

				
			for(int x = 0; x < BOARD_SIZE && ok4 == 0; x++)

				for(int y = 0; y < BOARD_SIZE && ok4 == 0; y++){

					char letter = board[x][y];

					if(letter != '.')
						
						for(int i = 0; i < NUM_WORDS && ok4 == 0; i++){

							dir = 0;

							if(words[i][0] == letter && word_is_used(board_words, words[i], 2*n) == 0){

								dir = get_direction(board, x, y, words[i]);

								if(dir >= 0){
							
									int points = get_bonus_points(get_points(words[i]), y, x, words[i], dir, sir1, sir2);

									if(points > max){
										max = points;
										strncpy(word, words[i], strlen(words[i]) + 1);
										lmax = x;
										colmax = y;
										dm = dir;
									}
								}
								
							}
						}
					
				}
			

			//get points for player2
			player2 += get_bonus_points(get_points(word), 
				x[i], y[i], word, direction[i], sir1, sir2);

			strcpy(board_words[idx++], word);

			length = strlen(word);

			if(dm == 0)

				for(int j = 0; j < length; j++)
					board[lmax][colmax + j] = word[j];

			else
				for(int j = 0; j < length; j++)
					board[lmax + j][colmax] = word[j];
		}
	
		print_board(board);

		if(player1 > player2)
			printf("Player 1 Won!\n");
		else
			printf("Player 2 Won!\n");

		//free the memory for the array of words
		for(int i = 0; i < 2 * n; i++)
			free(board_words[i]);
		free(board_words);
	}

	//free the memory for the input data
	free_read(&x, &y, &direction, &input_words, n);

	return 0;
}