/*******************************************************************************************
* Programmer: Bruno Sánchez
* Class: CptS 121, Fall 2021; 3
* Programming Assignment: PA6
* Date: November 9, 2021
* Description: Writes a program that implements a game of Battleship against a computer
*******************************************************************************************/
#include "PA6.h"
#include "ansi_color.h"

/*
* Name: Bruno Sanchez
* Function:
* Date: 11/6/2021
* Inputs:
* Returns:
* Description:
* Preconditions:
* Postconditions:
*/

/*
* Name: Bruno Sanchez
* Function: press_continue(int choice)
* Date: 11/6/2021
* Inputs: int choice
* Returns:
* Description: Is the keypress function that prompts players to press any key before continuing
* Preconditions: choice must be between 1 - 3
* Postconditions:
*/
void press_continue(int choice) {
	if (choice == 1){  
		printf("Press any key to continue");	
	}
	else if (choice == 2) {
		printf("Press any key to let the computer place it's ships!");
	}
	else if (choice == 3) {
		printf("Press any key to let the computer go!");
	}
	getch();
	printf("\n");
}

/*
* Name: Bruno Sanchez
* Function: welcome_screen(void)
* Date: 11/7/2021
* Inputs:
* Returns:
* Description: It's the first screen you see when you open this program. Gives you instructions on how to play
* Preconditions:
* Postconditions:
*/
void welcome_screen(void) {
	printf(WHT "Welcome to Battleship!\n");
	printf("There are 2 players: Player 1 will be you, the user. Player 2 will be the computer\n");
	printf("After placing your pieces, each player will be prompted to aim by inputting a coordinate as follows:\n");
	printf("(Letter betwen a - j) (Number between 1 - 10)");
	printf("Then, a shot is made at the coordinate that was chosen.\n");
	printf("If a ship is on that coordinate, it's a hit. If not, it's a miss\n");
	printf("Once all the coordinates on which a ship resides upon are hit, that ship is sunk\n");
	printf("The first player to sink all their opponent's ships, wins!\n" reset);
	press_continue(1);
	system("cls");
}
/*
* Name: Bruno Sanchez
* Function: init_board(Cell board_ptr[][MAX_ROWS]
* Date: 11/6/2021
* Inputs: Cell board_ptr[][MAX_ROWS]
* Returns:
* Description: Initializes a game board with all of it's variables
* Preconditions:
* Postconditions: cell board_ptr[][MAX_ROWS] will be ready to have stuff in it
*/
void init_board(Cell board_ptr[][MAX_ROWS]) {
	int row_ind = 0, col_ind = 0;
	for (; row_ind < MAX_ROWS; row_ind++) {
		for (col_ind = 0; col_ind < MAX_COLUMS; col_ind++) {
			board_ptr[row_ind][col_ind].location.row = row_ind;
			board_ptr[row_ind][col_ind].location.column = col_ind;
			board_ptr[row_ind][col_ind].symbol = '-';
			board_ptr[row_ind][col_ind].occupied = 0;
		}
	}
}

/*
* Name: Bruno Sanchez
* Function: init_pieces(Ship pieces[5]
* Date: 11/6/2021
* Inputs:Ship pieces[5]
* Returns:
* Description: Initializes an array of pieces that will be used during the game of Battleship
* Preconditions: pieces must have 5 pieces
* Postconditions: pieces[5] will be filled out
*/
void init_pieces(Ship pieces[5]) {
	//Names of the ships themselves
	pieces[0].name = "Carrier";
	pieces[1].name = "Battleship";
	pieces[2].name = "Cruiser";
	pieces[3].name = "Submarine";
	pieces[4].name = "Destroyer";

	//What the ships will be displayed as on the board
	pieces[0].symbol = 'c';
	pieces[1].symbol = 'b';
	pieces[2].symbol = 'r';
	pieces[3].symbol = 's';
	pieces[4].symbol = 'd';

	//The length of the ships
	pieces[0].size = 5;
	pieces[1].size = 4;
	pieces[2].size = 3;
	pieces[3].size = 3;
	pieces[4].size = 2;

	for (int index = 0; index < 5; index++) {
		//Whether the ships are sunk
		pieces[index].isSunk = 0;
		pieces[index].hits = 0;
		for (int j = 0; j < 5; j++) {
			//Coordinate of the ships
			pieces[index].points[j].row = 0;
			pieces[index].points[j].column = 0;
		}
	}
}

/*
* Name: Bruno Sanchez
* Function: init_player_stats(PlayerStats player)
* Date: 11/7/2021
* Inputs: PlayerStats player
* Returns:
* Description: Initializes the Statistics for a player
* Preconditions:
* Postconditions:
*/
void init_player_stats(PlayerStats player) {
	player.hits = 0;
	player.misses = 0;
	player.hit_miss_ratio = 0.0;
}

/*
* Name: Bruno Sanchez
* Function: display_board(Cell board_ptr[][MAX_ROWS])
* Date: 11/6/2021
* Inputs: Cell board_ptr[][MAX_ROWS]
* Returns:
* Description: Displays a board using a 2D Cell array, and with color!
* Preconditions:
* Postconditions:
*/
void display_board(Cell board_ptr[][MAX_ROWS]) {
	int row_ind = 0, col_ind = 0;
	printf(WHT "    1  2  3  4  5  6  7  8  9  10\n" reset);
	for (row_ind = 0; row_ind < MAX_ROWS; row_ind++) {
		printf(WHT "%2c " reset, (row_ind + 'A'));
		for (col_ind = 0; col_ind < MAX_COLUMS; col_ind++) {
			//Changes color of text
			char* color = WHT;
			if (board_ptr[col_ind][row_ind].symbol == '*') {
				color = BRED;
			}
			if (board_ptr[col_ind][row_ind].symbol == 'm') {
				color = BYEL;
			}
			
			printf("%s %s%c %s", BLUB, color, board_ptr[col_ind][row_ind].symbol, reset);
		}
		printf("\n");
	}
}

/*
* Name: Bruno Sanchez
* Function: get_starting_coordinates(Coordinate* starting)
* Date: 11/6/2021
* Inputs: Coordinate* starting
* Returns:
* Description: Prompts user for coordinates that a ship may be placed at, but only the first
* Preconditions:
* Postconditions: Passes on starting coordinates
*/
void get_starting_coordinates(Coordinate* starting) {
	char yCoord = '\0';
	int xCoord = 0;
	do {
		printf("Which coordinate do you want to place this ship in? (x y)\n");
		scanf(" %c %d", &yCoord, &xCoord);
		yCoord = toupper(yCoord);
		if (yCoord < 'A' || yCoord > 'J' || xCoord < 1 || xCoord > 10) {
			printf(RED "That combination of Coordinates aren't in range, try again!\n" reset);
		}
		else {
			starting->row = yCoord - 'A';
			starting->column = xCoord - 1;
		}
	} while (yCoord < 'A' || yCoord > 'J' || xCoord < 1 || xCoord > 10);
}

/*
* Name: Bruno Sanchez
* Function: fill_ship_coordinates(Ship* ship, char direction, Coordinate starting_point)
* Date: 11/6/2021
* Inputs: Ship* ship, char direction, Coordinate starting_point
* Returns:
* Description: Fills a ship with coordinates, starting from Coordinate starting_point. 
*			character direction affects which coordinates a ship will have
* Preconditions:
* Postconditions: ship.points[] are all filled
*/void fill_ship_coordinates(Ship* ship, char direction, Coordinate starting_point) {
	int index = 0;
	if (direction == 'D') {
		for (; index < ship->size; index++) {
			ship->points[index].row = starting_point.row + index;
			ship->points[index].column = starting_point.column;
			
		}
	}
	else if (direction == 'R') {
		for (; index < ship->size; index++) {
			ship->points[index].column = starting_point.column + index;
			ship->points[index].row = starting_point.row;
		}
	}


}

/*
* Name: Bruno Sanchez
* Function: validate_ship_coordinate(Cell board[][MAX_ROWS], Ship ship, char direction)
* Date: 11/6/2021
* Inputs: Cell board[][MAX_ROWS], Ship ship, char direction
* Returns: integer 1 or 0
* Description: Checks to see if the coordinates of a ship are able to be placed on a board
* Preconditions: board must be a 2D 10x10 board. Direction must be r for right or d for down
* Postconditions:
*/int validate_ship_coordinate(Cell board[][MAX_ROWS], Ship ship, char direction) {
	for (int index = 0; index < ship.size; index++) {
		if (ship.points[index].column >= MAX_COLUMS || ship.points[index].row >= MAX_ROWS) {
			return 0;
		}
		int occupied = board[ship.points[index].column][ship.points[index].row].occupied;
		if (occupied != 0 && occupied != 1) {
			// we should never hit this line, if it happens, it is a bug in initialization
			puts(RED"Occupied is neither 0 or 1");
		}
		if (board[ship.points[index].column][ship.points[index].row].occupied == 1)
		{
			return 0;
		}
	}
	return 1; 
}

/*
* Name: Bruno Sanchez
* Function: place_piece(Cell board_ptr[][MAX_ROWS], Ship ship)
* Date: 11/6/2021
* Inputs: Cell board_ptr[][MAX_ROWS], Ship ship
* Returns:
* Description: Places a ship on a board
* Preconditions: board_ptr[][] must be a 2D 10x10 board.
* Postconditions: board_ptr will have new symbols and occupied cells
*/
void place_piece(Cell board_ptr[][MAX_ROWS], Ship ship) { 
	int pointX = 0, pointY = 0;
	for (int index = 0; index < ship.size; index++) {
		board_ptr[ship.points[index].column][ship.points[index].row].symbol = ship.symbol;
		board_ptr[ship.points[index].column][ship.points[index].row].occupied = 1;
	}
}

/*
* Name: Bruno Sanchez
* Function: manual_placement_direction(Cell player_board[][MAX_ROWS], Ship playerShips[])
* Date: 11/6/2021
* Inputs: Cell player_board[][MAX_ROWS], Ship playerShips[]
* Returns:
* Description: User manually places ships on board
*			This is done by selecting a starting coordinate on the board, followed by the direction that the player wants the ship to be placed
*			Then the ship's coordinates are automatically filled out, then verified to see if it's valid
*			If the coordinates are valid, then the player board's coordinates are filled out accordingly
*			If the coordinates are not valid, then the player must start from the beginning with the prompts
* Preconditions:
* Postconditions: player_board will have new ships, each Ship in playerShips will be updated as well
*/void manual_placement_direction(Cell player_board[][MAX_ROWS], Ship playerShips[]) {
	char direction = '\0';
	int validShipLocation;
	Coordinate starting_point = { 0 , 0 };
	for (int ships_placed = 0; ships_placed < 5; ships_placed++) {
		display_board(player_board);
		do {
			direction = '\0';
			printf("Now placing the %s\n", playerShips[ships_placed]);
			//Gets starting point coordinates
			get_starting_coordinates(&starting_point);

			//Gets direction in which to place ship in
			while (direction != 'R' && direction != 'D') {
				printf("Which direction do you want the to face? 'R' for right and 'D' for down?\n");
				scanf(" %c", &direction);
				direction = toupper(direction);
				if (direction == 'R' || direction == 'D') {
					fill_ship_coordinates(&playerShips[ships_placed], direction, starting_point);
				}
				else {
					printf(RED "That is not a valid direction, try again!\n" reset);
				}
			}
			printf("Checking to see if the potential ship placement is valid...\n");
			validShipLocation = validate_ship_coordinate(player_board, playerShips[ships_placed], direction);
			if (validShipLocation == 1) {
				printf("The ship placement is valid!\n");
				press_continue(1);

			}
			else {
				printf("The ship placement is invalid, please start over\n");
			}
		} while (!validShipLocation);
		place_piece(player_board, playerShips[ships_placed]);
		system("cls");
	}
}
/*
* Name: Bruno Sanchez
* Function: manual_placement_coordinates(Cell player_board[][MAX_ROWS], Ship playerShips[])
* Date: 11/6/2021
* Inputs: Cell player_board[][MAX_ROWS], Ship playerShips[]
* Returns:
* Description: User manually places ships on board
*			This is done by prompting the user which coordinates they want to input onto the board. 
*			If they are within range, then they are verified to see if they are a valid ship
*			If so, then the cell board is updated accordingly, if not, then the process begins again
* Preconditions:
* Postconditions: player_board will have new ships, each Ship in playerShips will be updated as well
*/
void manual_placement_coordinates(Cell player_board[][MAX_ROWS], Ship playerShips[]) {
	int validShipLocation = 0, coordinatesValid = 0;
	Coordinate currentCoordinate[5] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} };
	for (int shipsPlaced = 0; shipsPlaced < 5; shipsPlaced++) {
		char rows[5] = { '\0', '\0', '\0', '\0', '\0' }, temp = '\0';
		int columns[5] = { 0, 0, 0, 0, 0 };
		//system("cls");
		display_board(player_board);
		do {
			do {
				coordinatesValid = 0;
				printf("Please enter the %d cells to place the %s across (Type in coordinates as Letter Num) EX: A 1\n", playerShips[shipsPlaced].size, playerShips[shipsPlaced].name);
				for (int i = 0; i < playerShips[shipsPlaced].size; i++) {
					scanf(" %c %d", &rows[i], &columns[i]);
					rows[i] = toupper(rows[i]);
				}

				for (int i = 0; i < playerShips[shipsPlaced].size; i++) {
					printf(" %c %d", rows[i], columns[i]);
				}
				puts("");

				for (int i = 0; i < playerShips[shipsPlaced].size; i++) {
					
					if (rows[i] < 'A' || rows[i] > 'J' || columns[i] < 1 || columns[i] > 10) {
						coordinatesValid = 1;
					}
				}
				if (coordinatesValid) {
					printf(RED "One or more of these coordinates are out of bounds. Try again!\n" reset);
				}
			} while (coordinatesValid);
			for (int i = 0; i < playerShips[shipsPlaced].size; i++) {
				currentCoordinate[i].row = rows[i] - 'A';
				currentCoordinate[i].column = columns[i] - 1;
			}

			validShipLocation = validate_group_coordinates(currentCoordinate, playerShips[shipsPlaced].size);
			if (validShipLocation == 0) {
				printf(RED "These coordinates are invalid! Please try again!" reset);
			}
			else {
				for (int i = 0; i < playerShips[shipsPlaced].size; i++) {
					playerShips[shipsPlaced].points[i] = currentCoordinate[i];
				}
				place_piece(player_board, playerShips[shipsPlaced]);
				printf("The values are valid!\n");
			}

		} while (validShipLocation == 0);
	}

}

/*
* Name: Bruno Sanchez
* Function: validate_group_coordinates(Coordinate check[5], int size)
* Date: 11/6/2021
* Inputs:Coordinate check[5], int size
* Returns: int 1 or 0
* Description:
* Preconditions: 
* Postconditions:
*/
int validate_group_coordinates(Coordinate check[5], int size) {
	int index = size - 1, validate = 1;

	if (check[index].column == (check[index - 1].column + 1)) {
		for (index -= 1; index > 0; index--) {
			if (check[index].column != check[index - 1].column + 1) {
				validate = 0;
			}
		}
	}
	else if (check[index].row == (check[index - 1].row + 1)) {
		for (index -= 1; index > 0; index--) {
			if (check[index].row != check[index - 1].row + 1) {
				validate = 0;
			}
		}
	}
	
	else {
		validate = 0;
	}
	return validate;
}

/*
* Name: Bruno Sanchez
* Function: random_player_placement(Cell player_board[][MAX_ROWS], Ship playerShips[])
* Date: 11/6/2021
* Inputs: Cell player_board[][MAX_ROWS], Ship playerShips[]
* Returns:
* Description: Randomly places ships on board
* Preconditions:
* Postconditions: player_board will have new ships, each Ship in playerShips will be updated as well
*/
void random_player_placement(Cell player_board[][MAX_ROWS], Ship playerShips[]) {
	char direction = '\0', yCoord = '\0';
	int validShipLocation, whichDirection = 0, xCoord = 0;
	Coordinate starting_point = { 0,0 };

	for (int ships_placed = 0; ships_placed < 5; ships_placed++) {
		do {
			//Gets starting point coordinates
			xCoord = rand() % 10;
			yCoord = (rand() % 10) + 'A';
			starting_point.row = yCoord - 'A'; 
			starting_point.column = xCoord; 

			//Gets direction in which to place ship in
			//random direction
			whichDirection = rand() % 2; //Will either be 1 or 0
			if (whichDirection == 0) {
				direction = 'D';
			}
			else if (whichDirection == 1) {
				direction = 'R';
			}
			fill_ship_coordinates(&playerShips[ships_placed], direction, starting_point);
			validShipLocation = validate_ship_coordinate(player_board, playerShips[ships_placed], direction);
		} while (validShipLocation == 0);
		place_piece(player_board, playerShips[ships_placed]);
	}
}

/*
* Name: Bruno Sanchez
* Function: print_game_pieces_to_file(FILE* outfile, Ship ships[5])
* Date: 11/7/2021
* Inputs: FILE* outfile, Ship ships[5]
* Returns:
* Description: Prints out each ship and it's coordinates to file "game_log.txt"
* Preconditions: outfile must be open
* Postconditions: outfile will print out new content
*/
void print_game_pieces_to_file(FILE* outfile, Ship ships[5]) {
	for (int index = 0; index < 5; index++) {
		fprintf(outfile, " %s's coordinates are: ", ships[index].name);
		for (int i = 0; i < ships[index].size; i++) {
			fprintf(outfile, "( %c , %d ) ", (ships[index].points[i].row + 'A'), ships[index].points[i].column);
		}
		fprintf(outfile, "\n");
	}
	
}
/*
* Name: Bruno Sanchez
* Function: get_aim(void)
* Date: 11/7/2021
* Inputs:
* Returns: Coordinate prompt
* Description: Prompts user for valid coordinates, if invalid, start again
* Preconditions:
* Postconditions:
*/
Coordinate get_aim(void) {
	Coordinate prompt = { 0,0 };
	int CoordX = 0;
	char CoordY = '\0';
	do {
		printf("Where do you want to aim?\n");
		scanf(" %c %d", &CoordY, &CoordX);
		CoordY = toupper(CoordY);
		if (CoordY < 'A' || CoordY > 'J' || CoordX < 1 || CoordX > 10) {
			printf(RED "That combination of Coordinates aren't in range, try again!\n" reset);
		}
		else {
			prompt.row = CoordY - 'A';
			prompt.column = CoordX - 1;
		}
	} while (CoordY < 'A' || CoordY > 'J' || CoordX < 1 || CoordX > 10);

	return prompt;
}
/*
* Name: Bruno Sanchez
* Function: aim_check(Cell boardChecker[][MAX_ROWS], Coordinate aim, int turn)
* Date: 11/7/2021
* Inputs: Cell boardChecker[][MAX_ROWS], Coordinate aim, int turn
* Returns: integer 1 or 0
* Description: Checks to see if Coordinate aim is hitting an already occupied cell on boardChecker, if so return 1, if not return 0
* Preconditions:
* Postconditions:
*/
int aim_check(Cell boardChecker[][MAX_ROWS], Coordinate aim, int turn) {
	int check = 0;
	if (boardChecker[aim.column][aim.row].occupied) {
		check = 1;
		if (turn == 0) {
			printf(RED "That combination of Coordinates has already been chosen!\n" reset);
		}
	}
	return check;
}
/*
* Name: Bruno Sanchez
* Function: hit_or_miss(Cell board[][MAX_ROWS], Coordinate aim)
* Date: 11/7/2021
* Inputs: Cell board[][MAX_ROWS], Coordinate aim
* Returns: int 1 or 0
* Description: Checks to see if hit cell at coordinate aim has a ship.symbol in it or not. If so return 1, if not 0
* Preconditions:
* Postconditions:
*/
int hit_or_miss(Cell board[][MAX_ROWS], Coordinate aim) {
	int hit_or_miss = 0; 
	char symbol = board[aim.column][aim.row].symbol;
		if (board[aim.column][aim.row].occupied == 1) {
			if (symbol == 'c' || symbol == 'b' || symbol == 'r' || symbol == 's' || symbol == 'd') {
				hit_or_miss = 1;
			}
		}
		return hit_or_miss;
}
/*
* Name: Bruno Sanchez
* Function: ship_symbol_to_number(char symbol)
* Date: 11/7/2021
* Inputs: char symbol
* Returns: integer between 0 - 4
* Description: Returns a number based on a character, which is a ship index for a specific character
* Preconditions: symbol must be 'c', 'b', 'r', 's', or 'd'
* Postconditions:
*/
int ship_symbol_to_number(char symbol) {
	switch (symbol) {
	case 'c': return 0;
	case 'b': return 1;
	case 'r': return 2;
	case 's': return 3;
	case 'd': return 4;
	}
}
/*
* Name: Bruno Sanchez
* Function: has_it_sunk(Ship ship)
* Date: 11/7/2021
* Inputs: Ship ship
* Returns: Integer 1 or 0
* Description: Checks to see if a ship has sunk
* Preconditions:
* Postconditions:
*/
int has_it_sunk(Ship ship) {
	int sunk = 0;
	if (ship.hits == ship.size) { 
		sunk = 1; 
	}
	return sunk;
}
/*
* Name: Bruno Sanchez
* Function: all_ships_sunk(Ship ships[5])
* Date: 11/6/2021
* Inputs: Ship ships[5]
* Returns: integer 1 or 0
* Description: Checks to see if all ships from an array of Ships have been sunk
* Preconditions: Ship ships[] must be size 5
* Postconditions:
*/
int all_ships_sunk(Ship ships[5]) {
	int index = 0, check = 0;
	for (; index < 5; index++) {
		if (ships[index].isSunk == 1) {
			check++;
		}
	}
	if (check == 5) {
		return 1;
	}
	else {
		return 0;
	}
}
/*
* Name: Bruno Sanchez
* Function: print_endgame_stats(FILE* outfile, int which_player, PlayerStats player)
* Date: 11/6/2021
* Inputs: FILE* outfile, int which_player, PlayerStats player
* Returns:
* Description: Prints the statistics of a player to a file "game_log"
* Preconditions:
* Postconditions: "game_log" will have new content in it.
*/
void print_endgame_stats(FILE* outfile, int which_player, PlayerStats player) {
	fprintf(outfile, "*** Player %d ***\n", which_player);
	fprintf(outfile, "Number of Hits: %d\n", player.hits);
	fprintf(outfile, "Number of Misses: %d\n", player.misses);
	fprintf(outfile, "Total Shots taken: %d\n", player.total_shots);
	fprintf(outfile, "Hit/Miss ratio: %.2lf\n", player.hit_miss_ratio);
}