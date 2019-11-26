#ifndef STATE_HPP
#define STATE_HPP

#include <array>
#include "ai_play.hpp"

class State {
public:
	State(Board board);
	void set_next(std::array<State*, 9> next_list);
	Board board() const;
	State* next(int i) const;
	int richest_next(char ml) const;
	int& count(int result);
private:
	double quality(char ml) const;
public:
	static const int CROSS_LOSS = 0;    // индексы поражений, ничей и побед, соответственно,
	static const int CROSS_DRAW = 1;    // а одновременно количества турнирных очков за них.
	static const int CROSS_WIN = 2;
public:
	static int get_result(Board board);
	static bool is_complete(Board board);
private:
	std::array<int, 3> _count;         // число поражений, ничей и побед, в таком порядке.
	std::array<State*, 9> _next;       // nullptr, если соответствующий ход неверен.
	Board _board;
};

#endif
