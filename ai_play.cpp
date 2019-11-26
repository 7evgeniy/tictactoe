#include "ai_play.hpp"
#include <cassert>

bool full(Board board) {
	for (int i = 0; i < 9; ++i)
		if (board[i] == ' ')
			return false;
	return true;
}

bool won(Board board, char who) {
	for (int i = 0; i < 3; ++ i) {
		bool h = true, v = true;
		for (int j = 0; j < 3; ++ j) {
			if (board[i*3+j] != who)
				h = false;
			if (board[j*3+i] != who)
				v = false;
		}
		if (h || v)
			return true;
	}
	bool r = true, l = true;
	for (int i = 0; i < 3; ++ i) {
		if (board[i*3+i] != who)
			r = false;
		if (board[i*3+2-i] != who)
			l = false;
	}
	return r || l;
}

char other(char who) {
	if (who == 'x')
		return '0';
	if (who == '0')
		return 'x';
	return who;
}

int worst(char who) {
	if (who == 'x')
		return ZeroWon-1;
	if (who == '0')
		return CrossWon+1;
	return Draw;
}

bool better(int a, int b, char who) {
	if (who == 'x')
		return a > b;
	if (who == '0')
		return a < b;
	return a == b;
}

int minimax(Board board, char who) {
	if (won(board, 'x'))
		return CrossWon;
	if (won(board, '0'))
		return ZeroWon;
	if (full(board))
		return Draw;
	int value = worst(who);
	for (int move = 0; move < 9; ++ move) {
		if (board[move] != ' ')
			continue;
		Board copy(board);
		copy[move] = who;
		int branch = minimax(copy, other(who));
		if (better(branch, value, who))
			value = branch;
	}
	return value;
}

int choose(Board board, char who) {
	int choice = -1, value = worst(who);
	for (int move = 0; move < 9; ++ move) {
		if (board[move] != ' ')
			continue;
		Board copy(board);
		copy[move] = who;
		int branch = minimax(copy, other(who));
		if (better(branch, value, who)) {
			choice = move;
			value = branch;
		}
	}
	assert(choice >= 0);
	return choice;
}
