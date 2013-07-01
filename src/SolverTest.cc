#include "Solver.hh"
#include "Tester.inl"


static void setUnique(Board<PossibilitySet> &board, Position pos, Number n) {
    auto eliminate = [&](Position pos2) -> bool {
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

    wholeArea.forAllPositions([&](Position pos) -> bool {
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

static void testApplyUniquePossibilities() {
    Board<PossibilitySet> actualBoard, expectedBoard;
    wholeArea.forAllPositions([&](Position pos) -> bool {
        actualBoard[pos] = PossibilitySet::full();
        return true;
    });

    expectedBoard = actualBoard;
    fixUniquePossibilities(actualBoard);
    test_assert(actualBoard == expectedBoard);

    Position pos1(0, 0), pos2(3, 4), pos3(3, 7), pos4(2, 1), pos5(6, 5);
    Number n1 = 0, n2 = 2, n3 = 8, n4 = 5, n5 = n1;

    rowArea(pos1).forAllPositions([&](Position pos) -> bool {
        actualBoard[pos].remove(n1);
        return true;
    });
    actualBoard[pos1].add(n1);
    rowArea(pos2).forAllPositions([&](Position pos) -> bool {
        actualBoard[pos].remove(n2);
        return true;
    });
    actualBoard[pos2].add(n2);
    columnArea(pos3).forAllPositions([&](Position pos) -> bool {
        actualBoard[pos].remove(n3);
        return true;
    });
    actualBoard[pos3].add(n3);
    blockArea(pos4).forAllPositions([&](Position pos) -> bool {
        actualBoard[pos].remove(n4);
        return true;
    });
    actualBoard[pos4].add(n4);
    blockArea(pos5).forAllPositions([&](Position pos) -> bool {
        actualBoard[pos].remove(n5);
        return true;
    });
    actualBoard[pos5].add(n5);

    expectedBoard = actualBoard;
    fixUniquePossibilities(actualBoard);
    expectedBoard[pos1] = PossibilitySet(n1);
    expectedBoard[pos2] = PossibilitySet(n2);
    expectedBoard[pos3] = PossibilitySet(n3);
    expectedBoard[pos4] = PossibilitySet(n4);
    expectedBoard[pos5] = PossibilitySet(n5);
    test_assert(actualBoard == expectedBoard);
}

static void testAll() {
    testEliminateImpossibilities();
    testApplyUniquePossibilities();
}

int main() {
    testAll();
    return Tester::errors != 0;
}


/* vim: set et sw=4 sts=4 tw=79: */
