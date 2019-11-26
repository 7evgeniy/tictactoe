#ifndef AI_PLAY_HPP
#define AI_PLAY_HPP

#include <array>

typedef std::array<char, 9> Board;

bool full(Board board);
bool won(Board board, char who);
char other(char who);
int worst(char who);
bool better(int a, int b, char who);
int minimax(Board board, char who);
int choose(Board board, char who);

const int CrossWon = 1;
const int ZeroWon = -1;
const int Draw = 0;

#endif
