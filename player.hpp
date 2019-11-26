#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "state.hpp"
#include "ai_play.hpp"
#include <array>

class Player {
public:
	Player();
	State* operator[](Board board);
	State* default_state();
private:
	static int deduce_index(Board board);
	static Board induce_board(int i);
	static char who_moves(Board board);
	static std::array<int, 9> report_next(int i);
	static const int StateCount = 19683;        // pow(3, 9)
private:
	State *_states[StateCount];
};

#endif
