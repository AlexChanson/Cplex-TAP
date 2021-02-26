﻿#include "instance.h"
#include "solver.h"
#include "combo.h"
#include <iostream>
#include <string>


// combo will not compile with sdl check sue to several C4703 warnings when initializing stuff in switches
// item : profit/wheight/keep
bool* exclude_nodes(cplex_tap::Instance tap, int lb_interest) {
	std::cout << "Filtering Nodes" << endl;
	int maxDist = 0;
	for (auto i = 0; i < tap.size(); ++i) {
		for (auto j = i + 1; j < tap.size(); ++j) {
			int d = tap.dist(i, j);
			if (d > maxDist)
				maxDist = d;
		}
	}
	maxDist++;

	// Debug for combo
	item items[3];
	items[0] = { 2, 2, false };
	items[1] = { 5, 3, false };
	items[2] = { 1, 5, false };

	item* start = items;
	item* end = (item*)(&items + 1) - 1;
	// run combo (pointer to first item, pointer to last item, max whieght, starting lb, starting ub, set x in items, no relaxation)
	stype s = combo(start, end, 6, 0, 99, true, false);

	
	bool out[3]; // out[tap.size()]
	for (auto i = 0; i < 3; ++i) { //i < tap.size()
		out[i] = items[i].x;
		std::cout << out[i];
	}
	std::cout << endl;
	return out;
}

int main() {
	using namespace cplex_tap;

	
	int sizes[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };

	for(const int &size : sizes){
		std::cout << "Loading TAP instance " << size << endl;
		std::stringstream fname;
		fname << "C:\\Users\\achan\\source\\repos\\cplex_test\\instances\\tap_6_" << size << ".dat";
		const auto tap = Instance(fname.str());
		//const auto tap = Instance("C:\\Users\\achan\\source\\repos\\cplex_test\\small_test_instance.txt");

		//exclude_nodes(tap, 5);

		const auto solver = Solver(tap);

		int budget = 5 * size;
		int interestingness_lb = 100 * size;

		double time = solver.solve_and_print(interestingness_lb, budget);
		std::cout << endl << "TIME TO SOLVE " << time << endl;
	}

	return 0;
}

