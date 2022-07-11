/*******************************************************************************************
* Programmer: Bruno Sánchez
* Class: CptS 121, Fall 2021; 3
* Programming Assignment: PA6
* Date: November 9, 2021
* Description: Writes a program that implements a game of Battleship against a computer
*******************************************************************************************/

#ifndef PA6_H
#define PA6_H
#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define MAX_ROWS 10
#define MAX_COLUMS 10

//Made to clear screen
#define CLS "\x1b[2J"

#include <stdio.h>  //adds printf() and scanf()
#include <math.h>    
#include <stdlib.h> //adds rand()
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <Windows.h>
#include <use_ansi.h>


typedef struct coordinate {
	int row; // Y axis
	int column; // X axis
}Coordinate;

typedef struct cell {
	int occupied; //1 if symbols are in cell; 0 otherwise
	char symbol; //c for Carrier, b for Battleship, r for Cruiser, s for Submarine, d for destroyer, ~ if none
	Coordinate location;
}Cell;

typedef struct ship {
	char* name;
	char symbol;
	int size;
	int hits; //if hits == size, ship is sunk
	int isSunk;
	Coordinate points[5];
}Ship;

typedef struct playerStats {
	int hits;
	int misses;
	int total_shots;
	double hit_miss_ratio;
}PlayerStats;

void press_continue(int choice);
void welcome_screen();
void init_pieces(Ship pieces[5]);
void init_board(Cell board_ptr[][MAX_ROWS]); 
void init_player_stats(PlayerStats player);
void display_board(Cell board_ptr[][MAX_ROWS]);
void get_starting_coordinates(Coordinate* starting);
void fill_ship_coordinates(Ship *ship, char direction, Coordinate starting_point);
int validate_ship_coordinate(Cell board[][MAX_ROWS], Ship ship, char direction);
void place_piece(Cell board_ptr[][MAX_ROWS], Ship ship);
void manual_placement_direction(Cell player_board[][MAX_ROWS], Ship playerShips[]);
void manual_placement_coordinates(Cell player_board[][MAX_ROWS], Ship playerShips[]);
int validate_group_coordinates(Coordinate check[5], int size);
void random_player_placement(Cell player_board[][MAX_ROWS], Ship playerShips[]);
void print_game_pieces_to_file(FILE* outfile, Ship ships[5]);
Coordinate get_aim(void);
int aim_check(Cell boardChecker[][MAX_ROWS], Coordinate aim, int turn);
int hit_or_miss(Cell board[][MAX_ROWS], Coordinate aim);
int ship_symbol_to_number(char symbol);
int all_ships_sunk(Ship ships[5]);
void print_endgame_stats(FILE* outfile, int which_player, PlayerStats player);

#endif PA6_H