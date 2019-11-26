#include "state.hpp"
#include <cassert>

using namespace std;

State::State(Board board) : _board(board) {
	for (int i = 0; i < 3; ++ i)
		_count[i] = 0;
}

void State::set_next(array<State*, 9> next_list) {
	_next = next_list;
}

double State::quality(char ml) const {   // число достигнутых турнирных очков, делённое на число игр
	if (_count[0] + _count[1] + _count[2] == 0)
		return 0.0;
	double l = _count[CROSS_LOSS], d = _count[CROSS_DRAW], w = _count[CROSS_WIN];
	if (ml == '0')
		swap(l, w);
	return (d+2*w)/(l+d+w);
}

Board State::board() const {
	return _board;
}

State* State::next(int i) const {
	return _next[i];
}

int State::richest_next(char ml) const {
	int i_max = -1;
	double q_max = -1.0;
	for (int i = 0; i < 9; ++ i)
		if (next(i)) {
			double q = next(i)->quality(ml);
			if (q > q_max) {
				q_max = q;
				i_max = i;
			}
		}
	assert(i_max >= 0);
	return i_max;
}

int& State::count(int result) {
	return _count[result];
}

int State::get_result(Board board) {
	if (won(board, 'x'))
		return CROSS_WIN;
	if (won(board, '0'))
		return CROSS_LOSS;
	return CROSS_DRAW;
}

bool State::is_complete(Board board) {
	return won(board, 'x') || won(board, '0') || full(board);
}
