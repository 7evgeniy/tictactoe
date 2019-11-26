#include "player.hpp"
#include "state.hpp"
#include "ai_play.hpp"
#include <vector>
#include <random>
#include <iostream>

using namespace std;

bool coin_rand(double pref) {
	static random_device rd;
	static default_random_engine dre(rd());
	static uniform_real_distribution<double> urd(0.0, 1.0);
	return urd(dre) < pref;
}

int move_rand(Board board) {
	static random_device rd;
	static default_random_engine dre(rd());
	static uniform_int_distribution<int> uid(0, 8);
	int move;
	do {
		move = uid(dre);
	} while (board[move] != ' ');
	return move;
}

int choose_weak(Board board, char who) {
	if (coin_rand(0.8))
		return choose(board, who);
	else
		return move_rand(board);
}

void teach(Player &player, int count) {
	for (int c = 0; c < count; ++ c) {
		State *state = player.default_state();
		vector<State*> game;
		char ml = coin_rand(0.5) ? 'x' : '0';
		char who = 'x';
		do {
			if (who == ml) {
				double pref = static_cast<double>(c)/static_cast<double>(count);
				if (coin_rand(pref))
					state = state->next(state->richest_next(ml));
				else
					state = state->next(move_rand(state->board()));
				game.push_back(state);
			}
			else
				state = state->next(choose_weak(state->board(), who));
			who = other(who);
		} while (!State::is_complete(state->board()));
		int result = State::get_result(state->board());
		for (State *state : game)
			++ state->count(result);
	}
}

int play(char ml, Player& player) {    // вернуть результат игры ML vs. WEAK
	State *state = player.default_state();
	char who = 'x';
	do {
		if (who == ml)
			state = state->next(state->richest_next(ml));
		else
			state = state->next(choose_weak(state->board(), who));
		who = other(who);
	} while (!State::is_complete(state->board()));
	int result = State::get_result(state->board());
	if (ml == '0')
		result = 2 - result;
	return result;
}

double test(Player& player, int count) {
	int s = 0;
	for (int c = 0; c < count; ++ c)
		s += play(coin_rand(0.5) ? 'x' : '0', player);
	return static_cast<double>(s)/static_cast<double>(2*count);
}

int main() {
	Player array;
	clog << test(array, 50) << endl;
	teach(array, 1000);
	clog << test(array, 50) << endl;
	return 0;
}
