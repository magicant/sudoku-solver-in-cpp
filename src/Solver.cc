#include "Solver.hh"


void eliminateImpossibilities(Board<PossibilitySet> &board) noexcept {
    // Collect unique values for speed.
    Board<Number> uniqueValues;
    wholeArea.forAllPositions([&](Position pos) {
        const PossibilitySet &pset = board[pos];
        uniqueValues[pos] = pset.isUnique() ? pset.uniqueValue() : N;
        return true;
    });

    // Eliminate!
    wholeArea.forAllPositions([&](Position pos1) {
        auto eliminate = [&](Position pos2) {
            if (pos2 != pos1 && uniqueValues[pos2] < N)
                board[pos1].remove(uniqueValues[pos2]);
            return true;
        };

        rowArea(pos1).forAllPositions(eliminate);
        columnArea(pos1).forAllPositions(eliminate);
        blockArea(pos1).forAllPositions(eliminate);
        return true;
    });
}


/* vim: set et sw=4 sts=4 tw=79: */
