#pragma once
#ifndef SUDOKU
#define SUDOKU

#include<iostream>
#include<algorithm>
#include<ctime>
#include <fstream>
#include<string>
#include "pch.h"

#define RESPATH "sudoku.txt"
#define QUESPATH "question.txt"
#define SIZE 9

extern void generate_c(int num);
extern void generategame(int num, int num1 = -1, int num2 = -1);
extern int getnumber(char* str);
extern void getdig(char* str, int& digup, int& digdown);
extern bool read_file(std::ifstream& infile, char* path);
extern void write_file(char* path);
extern void write_fail(char* path);
extern bool is_valid(int row, int col, int num);
extern int people_check(int row, int col);
extern bool dfs(int start, int& ifsolve);

extern int test_main(int myac, char** myav);

extern char board[9][9];


#endif // !SUDOKU

