#include "player.hpp"
#include "state.hpp"
#include "ai_play.hpp"
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>

using namespace std;

const int TeachStep = 25;
const int TeachMax = 2000;
const int TestCount = 500;
const bool Verbose = true;

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

void teach(Player &player, int begin, int step, int max) {
	int end = min(begin+TeachStep, TeachMax);
	for (int c = begin; c < end; ++ c) {
		State *state = player.default_state();
		vector<State*> game;
		char ml = coin_rand(0.5) ? 'x' : '0';
		char who = 'x';
		do {
			if (who == ml) {
				double pref = static_cast<double>(c)/static_cast<double>(TeachMax);
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
	return static_cast<double>(s)/static_cast<double>(count);
}

int main(int argc, char **argv) {
	Player array;
	int count = 0;
	cout << setw(4) << count << ": " << flush;
	cout << fixed << setprecision(2) << test(array, TestCount) << endl << flush;
	while (count < TeachMax) {
		teach(array, count, TeachStep, TeachMax);
		count += TeachStep;
		clog << setw(4) << count << ": " << flush;
		clog << fixed << setprecision(2) << test(array, TestCount) << endl << flush;
	}
	return 0;
}
