#include "Solver.hh"
#include "Tester.inl"


static void setUnique(Board<PossibilitySet> &board, Position pos, Number n) {
    auto eliminate = [&](Position pos2) {
        board[pos2].remove(n);
        return true;
    };

    blockArea(pos).forAllPositions(eliminate);
    rowArea(pos).forAllPositions(eliminate);
    columnArea(pos).forAllPositions(eliminate);
    board[pos] = PossibilitySet(n);
}

static void testEliminateImpossibilities() {
    Board<PossibilitySet> actualBoard, expectedBoard;
    Position pos1(0, 0), pos2(8, 0), pos3(3, 7);
    Number n1 = 0, n2 = 3, n3 = 8;

    wholeArea.forAllPositions([&](Position pos) {
        actualBoard[pos] = expectedBoard[pos] = PossibilitySet::full();
        return true;
    });
    actualBoard[pos1] = PossibilitySet(n1);
    eliminateImpossibilities(actualBoard);
    setUnique(expectedBoard, pos1, n1);
    test_assert(actualBoard == expectedBoard);

    actualBoard[pos2] = PossibilitySet(n2);
    actualBoard[pos3] = PossibilitySet(n3);
    eliminateImpossibilities(actualBoard);
    setUnique(expectedBoard, pos2, n2);
    setUnique(expectedBoard, pos3, n3);
    test_assert(actualBoard == expectedBoard);
}

static void testAll() {
    testEliminateImpossibilities();
}

int main() {
    testAll();
    return Tester::errors != 0;
}


/* vim: set et sw=4 sts=4 tw=79: */
