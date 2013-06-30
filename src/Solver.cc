#include <array>
#include <cstddef>
#include "Solver.hh"

using std::array;


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

/**
 * Counts the number of positions in the given area where a number can occur.
 * Makes the possibility unique if the number can occur at exactly one
 * position.
 */
static void fixUniquePossibilities(
        Board<PossibilitySet> &board,
        const Area &area) {
    struct Possibility {
        Position position;
        size_t count = 0;
    };

    array<Possibility, N> possibilities;

    area.forAllPositions([&](Position pos) {
        board[pos].forAllPossibleNumbers([&, pos](Number n) {
            possibilities[n].position = pos;
            possibilities[n].count++;
            return true;
        });
        return true;
    });

    for (Number n = 0; n < N; n++)
        if (possibilities[n].count == 1)
            board[possibilities[n].position] = PossibilitySet(n);
}

void fixUniquePossibilities(Board<PossibilitySet> &board) noexcept {
    for (Number n = 0; n < N; n++) {
        fixUniquePossibilities(board, rowArea(n));
        fixUniquePossibilities(board, columnArea(n));
    }

    forAllBlockAreas([&](const Area &area) {
        fixUniquePossibilities(board, area);
        return true;
    });
}


/* vim: set et sw=4 sts=4 tw=79: */
