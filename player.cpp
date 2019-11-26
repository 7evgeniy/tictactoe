#include "player.hpp"
#include <cassert>

using namespace std;

Player::Player() {
	for (int i = 0; i < StateCount; ++ i) {
		Board board = induce_board(i);
		_states[i] = (who_moves(board) != '?') ? new State(board) : nullptr;
	}
	for (int i = 0; i < StateCount; ++ i)
		if (_states[i]) {
			std::array<int, 9> next_index = report_next(i);
			std::array<State*, 9> next_state;
			for (int k = 0; k < 9; ++ k)
				next_state[k] = (next_index[k] >= 0 ? _states[next_index[k]] : nullptr);
			_states[i]->set_next(next_state);
		}
}

State* Player::operator[] (Board board) {
	return _states[deduce_index(board)];
}

State* Player::default_state() {
	Board board;
	for (char &c : board)
		c = ' ';
	return _states[deduce_index(board)];
}

int Player::deduce_index(Board board) {
	int sum = 0;
	for (int i = 8; i >= 0; -- i) {
		switch(board[i]) {
		case ' ': sum = sum*3 + 0; break;
		case 'x': sum = sum*3 + 1; break;
		case '0': sum = sum*3 + 2; break;
		default: return -1;
		}
	}
	return sum;
}

Board Player::induce_board(int sum) {
	Board board;
	char cell[3] = {' ', 'x', '0'};
	assert (sum >= 0 && sum < StateCount);
	for (int i = 0; i < 9; ++ i) {
		board[i] = cell[sum % 3];
		sum /= 3;
	}
	return board;
}

char Player::who_moves(Board board) {
	unsigned int cross_count = 0, zero_count = 0;
	for (char c : board)
		switch(c) {
		case 'x': ++ cross_count; break;
		case '0': ++ zero_count; break;
		case ' ': break;
		default: return '?';
		}
	unsigned int diff = cross_count - zero_count;
	if (diff >= 2)
		return '?';
	if (cross_count + zero_count == 9)
		return ' ';
	char cell[2] = {'x', '0'};
	return cell[diff];
}

array<int, 9> Player::report_next(int i) {
	array<int, 9> next;
	for (int &a : next)
		a = -1;
	Board board = induce_board(i);
	char who = who_moves(board);
	if (who != 'x' && who != '0')
		return next;
	for (int i = 0; i < 9; ++ i)
		if (board[i] == ' ') {
			Board copy = board;
			copy[i] = who;
			next[i] = deduce_index(copy);
		}
	return next;
}
