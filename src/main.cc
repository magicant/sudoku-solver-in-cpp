#include <cstdlib>
#include <iostream>
#include "Board.hh"
#include "Solver.hh"


int main() {
    Board<Number> problem;
    if (!(std::cin >> problem)) {
        std::cerr << "Cannot parse problem board.\n";
        return EXIT_FAILURE;
    }

    Board<PossibilitySet> problemPSBoard;
    convert(problem, problemPSBoard);

    bool foundSolution = false;
    iterateSolutions(problemPSBoard, [&](const Board<Number> &solution) {
        foundSolution = true;
        std::cout << solution;
    });
    return foundSolution ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* vim: set et sw=4 sts=4 tw=79: */
