#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include "Board.hh"
#include "Tester.inl"

using std::size_t;
using std::ostringstream;
using std::string;


static void testPossibilitySet1() {
    PossibilitySet pset1, pset2(pset1);
    test_assert(pset1 == pset2);
    test_assert(pset1.isEmpty());

    const size_t n1 = 2;
    pset1.add(n1);
    test_assert(pset1 == PossibilitySet(n1));
    test_assert(pset2 != PossibilitySet(n1));
    test_assert(pset1.isUnique());
    test_assert(pset1.uniqueValue() == n1);
    test_assert(!pset2.isUnique());

    const size_t n2 = 5;
    pset1.add(n2);
    test_assert(!pset1.isEmpty());
    test_assert(!pset1.isUnique());

    test_assert(pset1.remove(n1) == pset2.add(n2));
}

static void testPossibilitySet2() {
    PossibilitySet pset;

    for (size_t n = 0; n < N; n++)
        pset.add(n).add(n);
    test_assert(pset == PossibilitySet::full());
    for (size_t n = 0; n < N; n++)
        pset.remove(n).remove(n);
    test_assert(pset == PossibilitySet());
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

static void testAll() {
    testPossibilitySet1();
    testPossibilitySet2();
    testPosition1();
    testArea1();
    testBoard1();
    testBoard2();
}

int main() {
    testAll();
    return Tester::errors != 0;
}


/* vim: set et sw=4 sts=4 tw=79: */
