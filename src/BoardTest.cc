#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include "Board.hh"
#include "Tester.inl"

using std::istringstream;
using std::ostringstream;
using std::string;


static void testPossibilitySet1() {
    PossibilitySet pset1, pset2(pset1);
    test_assert(pset1 == pset2);
    test_assert(pset1.isEmpty());

    const Number n1 = 2;
    pset1.add(n1);
    test_assert(pset1 == PossibilitySet(n1));
    test_assert(pset2 != PossibilitySet(n1));
    test_assert(pset1.isUnique());
    test_assert(pset1.uniqueValue() == n1);
    test_assert(!pset2.isUnique());

    const Number n2 = 5;
    pset1.add(n2);
    test_assert(!pset1.isEmpty());
    test_assert(!pset1.isUnique());

    pset1.remove(n1);
    pset2.add(n2);
    test_assert(pset1 == pset2);

    const Number n3 = 0, n4 = 3, n5 = 8, n6 = 7, n7 = 1, n8 = 4, n9 = 6;
    pset1.add(n3);
    test_assert(pset1.count() == 2);
    pset1.add(n1);
    test_assert(pset1.count() == 3);
    pset1.add(n4);
    test_assert(pset1.count() == 4);
    pset1.add(n5);
    test_assert(pset1.count() == 5);
    pset1.add(n6);
    test_assert(pset1.count() == 6);
    pset1.add(n7);
    test_assert(pset1.count() == 7);
    pset1.add(n8);
    test_assert(pset1.count() == 8);
    pset1.add(n9);
    test_assert(pset1.count() == 9);
}

static void testPossibilitySet2() {
    PossibilitySet pset;

    for (Number n = 0; n < N; n++)
        pset.add(n).add(n);
    test_assert(pset == PossibilitySet::full());
    for (Number n = 0; n < N; n++)
        pset.remove(n).remove(n);
    test_assert(pset == PossibilitySet());
}

static void testPossibilitySetNumbers(
        const PossibilitySet &pset,
        const string expected) {
    ostringstream os;
    pset.forAllPossibleNumbers([&](Number n) -> bool { return os << n; });
    test_assert(os.str() == expected);
}

static void testPossibilitySet3() {
    PossibilitySet pset;
    testPossibilitySetNumbers(pset, "");
    pset.add(7);
    pset.add(3);
    pset.add(6);
    testPossibilitySetNumbers(pset, "367");
    pset.remove(3);
    pset.add(1);
    testPossibilitySetNumbers(pset, "167");

    pset = PossibilitySet::full();
    testPossibilitySetNumbers(pset, "012345678");
    pset.remove(4);
    pset.remove(8);
    pset.remove(1);
    testPossibilitySetNumbers(pset, "023567");
    pset.add(8);
    pset.remove(5);
    testPossibilitySetNumbers(pset, "023678");
}

static void testPosition1() {
    test_assert(Position().i() == 0);
    test_assert(Position().j() == 0);
    test_assert(Position() == Position(0, 0));
    test_assert(Position(3, 7).i() == 3);
    test_assert(Position(3, 7).j() == 7);

    test_assert(Position().isValid());
    test_assert(Position(N - 1, 0).isValid());
    test_assert(Position(0, N - 1).isValid());
    test_assert(Position(N - 1, N - 1).isValid());
    test_assert(!Position(N, N - 1).isValid());
    test_assert(!Position(N - 1, N).isValid());
    test_assert(!Position(N, N).isValid());

    test_assert(Position().right().down() == Position(1, 1));
    test_assert(Position(1, 2).right(5).down(0).right(1).down(8) ==
            Position(9, 8));
}

static void testArea1() {
    const Area a1(Position(1, 2), Position(2, 3));
    wholeArea.forAllPositions([&](Position pos) {
        test_assert(a1.contains(pos) == (pos == a1.topLeft()));
        return true;
    });
    a1.forAllPositions([&](Position pos) {
        test_assert(pos == a1.topLeft());
        return true;
    });

    const Position p1(3, 4);
    const Area a2 = rowArea(p1), a3 = columnArea(p1);
    wholeArea.forAllPositions([&](Position pos) {
        test_assert(a2.contains(pos) == (pos.i() == a2.topLeft().i()));
        test_assert(a3.contains(pos) == (pos.j() == a3.topLeft().j()));
        return true;
    });
    a2.forAllPositions([&](Position pos) {
        test_assert(pos.i() == a2.topLeft().i());
        return true;
    });
    a3.forAllPositions([&](Position pos) {
        test_assert(pos.j() == a3.topLeft().j());
        return true;
    });

    const Area a4 = blockArea(Position(4, 7));
    a4.forAllPositions([&](Position pos) {
        test_assert(3 <= pos.i() && pos.i() < 6);
        test_assert(6 <= pos.j() && pos.j() < 9);
        return true;
    });
}

static void testBoard1() {
    struct Int {
        int value;
        constexpr Int() : value() { }
        operator int() const { return value; }
        operator int &() { return value; }
    };

    Board<Int> b1;

    wholeArea.forAllPositions([&](Position pos) {
        test_assert(b1.at(pos) == 0);
        test_assert(b1[pos] == 0);
        return true;
    });
    test_assert(b1 == b1);

    Board<Int> b2 = b1;
    test_assert(b1 == b2);
    test_assert(b2 == b1);

    wholeArea.forAllPositions([&](Position pos) {
        b1[pos] += pos.i();
        return true;
    });
    test_assert(b1 == b1);
    wholeArea.forAllPositions([&](Position pos) {
        test_assert(b1[pos] == pos.i());
        return true;
    });
    wholeArea.forAllPositions([&](Position pos) {
        b1[pos] += pos.j();
        return true;
    });
    test_assert(b1 == b1);
    wholeArea.forAllPositions([&](Position pos) {
        test_assert(b1[pos] == pos.i() + pos.j());
        return true;
    });
    test_assert(b1 == b1);
    test_assert(b1 != b2);
}

template <typename T>
static void testString(const Board<T> &board, const string expected) {
    ostringstream os;
    os << board;
    test_assert(os.str() == expected);
}

static void testParseAndOutput(const string input, const string output) {
    istringstream is(input);
    Board<Number> board;
    is >> board;
    test_assert(is.good());
    testString(board, output);
}

static void testBoard2() {
    Board<Number> b1;
    wholeArea.forAllPositions([&](Position pos) {
        b1[pos] = 0;
        return true;
    });
    testString(b1,
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                );

    b1[Position(2, 4)] = 1;
    b1[Position(7, 3)] = 5;
    testString(b1,
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 2 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                "1 1 1 6 1 1 1 1 1\n"
                "1 1 1 1 1 1 1 1 1\n"
                );

    wholeArea.forAllPositions([&](Position pos) {
        b1[pos] = pos.i() * pos.j();
        return true;
    });
    testString(b1,
                "1 1 1 1 1 1 1 1 1\n"
                "1 2 3 4 5 6 7 8 9\n"
                "1 3 5 7 9 0 0 0 0\n"
                "1 4 7 0 0 0 0 0 0\n"
                "1 5 9 0 0 0 0 0 0\n"
                "1 6 0 0 0 0 0 0 0\n"
                "1 7 0 0 0 0 0 0 0\n"
                "1 8 0 0 0 0 0 0 0\n"
                "1 9 0 0 0 0 0 0 0\n"
                );

    Board<PossibilitySet> b2;
    testString(b2,
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                );

    b2[Position(3, 3)].add(8);
    b2[Position(5, 4)].add(0);
    testString(b2,
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x 9 x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x 1 x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                );

    b2[Position(3, 3)].add(0);
    b2[Position(5, 4)].remove(0);
    testString(b2,
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x ? x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                "x x x x x x x x x\n"
                );
}

static void testBoard3() {
    testParseAndOutput(
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n",
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n");
    testParseAndOutput(
            "0  0 1 0  0  0 0 0  0 dummy\n"
            "0 0  0  0 0 0  0  7 0\n"
            "0  0 0 0  0  2 0 0  0\n"
            "0 0  0  0 0 0  0  0 0\n"
            "0  6 0 0  0  0 0 0  8\n"
            "0 0  0  3 0 0  0  0 0\n"
            "0  0 0 0  0  0 9 0  0\n"
            "0 0  0  0 0 4  0  0 0\n"
            "5  0 0 0  0  0 0 0  0 dummy\n"
            "dummy\n",
            "0 0 1 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 7 0\n"
            "0 0 0 0 0 2 0 0 0\n"
            "0 0 0 0 0 0 0 0 0\n"
            "0 6 0 0 0 0 0 0 8\n"
            "0 0 0 3 0 0 0 0 0\n"
            "0 0 0 0 0 0 9 0 0\n"
            "0 0 0 0 0 4 0 0 0\n"
            "5 0 0 0 0 0 0 0 0\n");
}

static void testBoard4() {
    Board<Number> nBoard;
    Board<PossibilitySet> pBoard;

    wholeArea.forAllPositions([&](Position pos) {
        nBoard[pos] = N;
        return true;
    });
    nBoard[Position(6, 2)] = 7;
    nBoard[Position(8, 0)] = 3;
    convert(nBoard, pBoard);
    wholeArea.forAllPositions([&](Position pos) {
        if (pos == Position(6, 2))
            test_assert(pBoard[pos].uniqueValue() == 7);
        else if (pos == Position(8, 0))
            test_assert(pBoard[pos].uniqueValue() == 3);
        else
            test_assert(pBoard[pos] == PossibilitySet::full());
        return true;
    });

    wholeArea.forAllPositions([&](Position pos) {
        nBoard[pos] = N + 1;
        return true;
    });
    pBoard[Position(5, 3)] = PossibilitySet(4);
    pBoard[Position(8, 0)] = PossibilitySet::full();
    convert(pBoard, nBoard);
    wholeArea.forAllPositions([&](Position pos) {
        if (pos == Position(6, 2))
            test_assert(nBoard[pos] == 7);
        else if (pos == Position(5, 3))
            test_assert(nBoard[pos] == 4);
        else
            test_assert(nBoard[pos] >= N);
        return true;
    });
}

static void testAll() {
    testPossibilitySet1();
    testPossibilitySet2();
    testPossibilitySet3();
    testPosition1();
    testArea1();
    testBoard1();
    testBoard2();
    testBoard3();
    testBoard4();
}

int main() {
    testAll();
    return Tester::errors != 0;
}


/* vim: set et sw=4 sts=4 tw=79: */
