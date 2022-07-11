/*******************************************************************************************
* Programmer: Bruno Sánchez
* Class: CptS 121, Fall 2021; 3
* Programming Assignment: PA6
* Date: November 9, 2021
* Description: Writes a program that implements a game of Battleship against a computer
*******************************************************************************************/
#include "pa6.h"
#include "ansi_color.h"

int main(void) {
    srand((unsigned int)time(NULL));
    FILE* outfile = NULL;
    Cell player1Board[MAX_COLUMS][MAX_ROWS], player1BoardCheck[MAX_COLUMS][MAX_ROWS], player2Board[MAX_COLUMS][MAX_ROWS], player2BoardCheck[MAX_COLUMS][MAX_ROWS];
    Ship player1Ships[5], player2Ships[5], tempShip = { '\0', '\0', 0, 0, 0, {0,0} };
    PlayerStats player1 = { 0, 0, 0, 0.0}, player2 = { 0, 0, 0, 0.0};
    Coordinate aim = { 0 , 0 };
    int placementChoice = 0, p1shipsSunk = 0, p2shipSunk = 0, gameRunning = 0, turns = 0, isHit = 0;
    char tempShipSymbol = '\0';

    //Initializes Player Ships
    init_pieces(player1Ships);
    init_pieces(player2Ships);

    //Initializes Player Boards
    init_board(player1Board);
    init_board(player1BoardCheck);
    init_board(player2Board);
    init_board(player2BoardCheck);  

    //Opens "game_log.txt"
    
    outfile = fopen("game_log.txt", "w");


    welcome_screen();
    //place_piece(player1Board, player1Ships[0], list); 
    //display_board(player1Board);
    do {
        printf("How would you like to place your pieces?\n 1) Manually\n 2) Let the game do it for me\n");
        scanf("%d", &placementChoice);
    } while (placementChoice != 1 && placementChoice != 2);
    switch (placementChoice) {
    case 1:
        fprintf(outfile, "Player 1's pieces\n");
        manual_placement(player1Board, player1Ships); 
        break;
    case 2:
        fprintf(outfile, "Player 1's pieces\n");
        random_player_placement(player1Board, player1Ships); 
        break;
    }
     
    printf("Player 1's Board\n");
    display_board(player1Board);
    press_continue(2);
    random_player_placement(player2Board, player2Ships);
    printf("Player 2's Board\n");
    display_board(player2Board);
    press_continue(1); //Debugging purposes:

    fprintf(outfile, "Player 1 Ships!\n");
    print_game_pieces_to_file(outfile, player1Ships);
    fprintf(outfile, "Player 2 Ships!\n");
    print_game_pieces_to_file(outfile, player2Ships); 

    fprintf(outfile, "Game start! \n \n" );
    fflush(outfile);
    
    //The game itself playing
    for (turns = rand() % 2; gameRunning == 0; turns++, turns %= 2) {
        system("cls");
        printf("It is currently Player %d's turn!\n", (turns + 1));
        fprintf(outfile, "Player %d:", (turns + 1)); 
        //Gets coordinates where it'll be aimed at + prints out boards
        if (turns == 0) { 
            aim.row = 0;
            aim.column = 0;
            printf("Player 2's Board\n");
            display_board(player2BoardCheck); 
            do {
                aim = get_aim();
            } while (aim_check(player2BoardCheck, aim, turns)); //Stops if there's nothing in the coordinate that's aimed at / chosen
            printf("You aimed at %c,%d\n", (aim.row + 'A'), (aim.column + 1));
            press_continue(1);
        }
        else {
            aim.row = 0;
            aim.column = 0;
            printf("Player 1's Board\n");
            display_board(player1Board);
            press_continue(3);
            do{
                aim.column = rand() % 10;
                aim.row = (rand() % 10);
            } while (aim_check(player1BoardCheck, aim, turns)); //Stops if there's nothing in the coordinate that's aimed at / chosen
            printf("The computer aimed at %c,%d\n", (aim.row + 'A'), (aim.column + 1));
            press_continue(1);
        }
        fprintf(outfile, " %c %d ", (aim.row + 'A'), aim.column);

        //Player 2 Hit check
        if (turns) {
            tempShipSymbol = player1Board[aim.column][aim.row].symbol;
            if(hit_or_miss(player1Board, aim)){
                printf(RED "The computer hit!\n" reset);
                //Updates board that will be seen
                player1Board[aim.column][aim.row].symbol = '*';
                player1Board[aim.column][aim.row].occupied = 1;
                //Updates board that tracks whether you can shoot somewhere again
                player1BoardCheck[aim.column][aim.row].symbol = '*';
                player1BoardCheck[aim.column][aim.row].occupied = 1;
                //Updates ship stats
                player1Ships[ship_symbol_to_number(tempShipSymbol)].hits++;
                tempShip = player1Ships[ship_symbol_to_number(tempShipSymbol)];
                //Prints to log that Player 2 hit
                fprintf(outfile, " Hit ");
                //Updates on whether the ship that was recently hit has sunk
                if (has_it_sunk(tempShip)) {
                    player1Ships[ship_symbol_to_number(tempShipSymbol)].isSunk = 1;
                    p1shipsSunk++;
                    printf(RED "Player 1's %s has sunk!\n" reset, player1Ships[ship_symbol_to_number(tempShipSymbol)].name);
                    fprintf(outfile, "sunk Player 1's %s!", player1Ships[ship_symbol_to_number(tempShipSymbol)].name);
                }
                //Updates player stats
                player2.total_shots++;
                player2.hits++;
            }
            else {
                printf(WHT "The computer missed!\n" reset);
                //Updates board that will be seen
                player1Board[aim.column][aim.row].symbol = 'm';
                //Updates board that tracks whether you can shoot somewhere again
                player1BoardCheck[aim.column][aim.row].symbol = 'm';
                player1BoardCheck[aim.column][aim.row].occupied = 1;
                //Updates player stats
                player2.total_shots++;
                player1.misses++;
                //Prints to log that Player 2 missed
                fprintf(outfile, " Miss ");
            }
            press_continue(1);
            fprintf(outfile, "\n");
        }
        //Player 1 hit check
        else {
            tempShipSymbol = player2Board[aim.column][aim.row].symbol;
            if (hit_or_miss(player2Board, aim)) {
                printf(RED "It's a hit!\n" reset);
                //Updates board that will be seen
                player2Board[aim.column][aim.row].symbol = '*';
                player2Board[aim.column][aim.row].occupied = 1;
                //Updates board that tracks whether you can shoot somewhere again
                player2BoardCheck[aim.column][aim.row].symbol = '*';
                player2BoardCheck[aim.column][aim.row].occupied = 1;
                //Updates ship stats
                player2Ships[ship_symbol_to_number(tempShipSymbol)].hits++;
                tempShip = player2Ships[ship_symbol_to_number(tempShipSymbol)];
                //Prints to log that Player 1 hit
                fprintf(outfile, " Hit ");
                //Updates on whether the ship that was recently hit has sunk
               
                if (has_it_sunk(tempShip)) {
                    player2Ships[ship_symbol_to_number(tempShipSymbol)].isSunk = 1;
                    p2shipSunk++;
                    printf(RED "Player 2's %s has sunk!\n" reset, player2Ships[ship_symbol_to_number(tempShipSymbol)].name);
                    fprintf(outfile, "sunk Player 2's %s!", player2Ships[ship_symbol_to_number(tempShipSymbol)].name);
                }
                //Updates player stats
                player1.total_shots++;
                player1.hits++;
            }
            else {
                printf(WHT "It's a miss!\n" reset);
                //Updates board that will be seen
                player2Board[aim.column][aim.row].symbol = 'm';
                //Updates board that tracks whether you can shoot somewhere again
                player2BoardCheck[aim.column][aim.row].symbol = 'm';
                player2BoardCheck[aim.column][aim.row].occupied = 1;
                //Updates player stats
                player1.total_shots++;
                player1.misses++;
                //Prints to log that Player 1 missed
                fprintf(outfile, " Miss ");
            }
            press_continue(1);
            fprintf(outfile, "\n");
        }
        //Displays board one more time
        if (turns) {
            display_board(player1Board);
        }
        else {
            display_board(player2BoardCheck);
        }
        press_continue(1);
        
        //Ends a player's turn
        printf("Player %d's turn has ended!\n", (turns+1));

        //Checks to see if a player has sunk all of their opponent's ships
        if (all_ships_sunk(player1Ships)) {
            printf(RED "Player 2 has won the game!" reset);
            fprintf(outfile, "Player 2 Wins! Player 1 Loses\n");
            gameRunning = 1;
        }
        else if (all_ships_sunk(player2Ships)) {
            printf(GRN "Player 1 has won the game!" reset);
            fprintf(outfile, "Player 1 Wins! Player 2 Loses\n");
            gameRunning = 1;
        }
        player1.hit_miss_ratio = player1.hits / (double)player1.total_shots;
        player2.hit_miss_ratio = player2.hits / (double)player2.total_shots;
        fflush(outfile);
    }
    print_endgame_stats(outfile, 1, player1);
    print_endgame_stats(outfile, 2, player2); 
    fclose(outfile);
    return 1;
}