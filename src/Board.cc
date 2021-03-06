#include <cstdlib>
#include <ios>
#include <limits>
#include <stdexcept>
#include "Board.hh"

using std::abort;
using std::logic_error;
using std::numeric_limits;
using std::size_t;
using std::streamsize;


Number PossibilitySet::uniqueValue() const {
    if (!isUnique())
        throw logic_error("multiple possibilities");

    for (Number n = 0; n < N; n++)
        if (mNumbers[n])
            return n;

    abort();
}

bool PossibilitySet::forAllPossibleNumbers(std::function<bool(Number)> f)
        const noexcept(noexcept(f(0))) {
    for (Number n = 0; n < N; n++)
        if (mNumbers[n])
            if (!f(n))
                return false;
    return true;
}

bool Area::contains(const Position &pos) const noexcept {
    return topLeft().i() <= pos.i() &&
            topLeft().j() <= pos.j() &&
            pos.i() < bottomRight().i() &&
            pos.j() < bottomRight().j();
}

bool Area::forAllPositions(std::function<bool(Position)> f)
        const noexcept(noexcept(f(Position()))) {
    for (Position pi = topLeft(); pi.i() < bottomRight().i(); pi.down())
        for (Position pj = pi; pj.j() < bottomRight().j(); pj.right())
            if (!f(pj))
                return false;
    return true;
}

Area blockArea(const Position &pos) {
    Position topLeft(pos.i() / Nsub * Nsub, pos.j() / Nsub * Nsub);
    Position bottomRight = Position(topLeft).down(Nsub).right(Nsub);
    return Area(topLeft, bottomRight);
}

bool forAllBlockAreas(std::function<bool(const Area&)> f)
        noexcept(noexcept(f(Area()))) {
    for (Position pi; pi.i() < N; pi.down(Nsub))
        for (Position pj = pi; pj.j() < N; pj.right(Nsub))
            if (!f(blockArea(pj)))
                return false;
    return true;
}

void convert(
        const Board<Number> &srcBoard,
        Board<PossibilitySet> &destBoard)
        noexcept {
    wholeArea.forAllPositions([&](Position pos) -> bool {
        Number n = srcBoard[pos];
        destBoard[pos] = (n < N) ? PossibilitySet(n) : PossibilitySet::full();
        return true;
    });
}

void convert(
        const Board<PossibilitySet> &srcBoard,
        Board<Number> &destBoard)
        noexcept {
    wholeArea.forAllPositions([&](Position pos) -> bool {
        const PossibilitySet &pset = srcBoard[pos];
        destBoard[pos] = pset.isUnique() ? pset.uniqueValue() : N;
        return true;
    });
}

static char separator(Position pos) {
    return pos.right().isValid() ? ' ' : '\n';
}

std::ostream &operator<<(std::ostream &os, const Board<Number> &board) {
    wholeArea.forAllPositions([&](Position pos) -> bool {
        Number n = board[pos];
        os << (n < N ? n + 1 : 0) << separator(pos);
        return os.good();
    });
    return os;
}

std::ostream &operator<<(
        std::ostream &os,
        const Board<PossibilitySet> &board) {
    wholeArea.forAllPositions([&](Position pos) -> bool {
        const PossibilitySet &pset = board[pos];
        if (pset.isEmpty())
            os << 'x';
        else if (pset.isUnique())
            os << pset.uniqueValue() + 1;
        else
            os << '?';
        os << separator(pos);
        return os.good();
    });
    return os;
}

std::istream &operator>>(std::istream &is, Board<Number> &board) {
    for (Position pi; pi.isValid(); pi.down()) {
        for (Position pj = pi; pj.isValid(); pj.right()) {
            unsigned n;
            is >> n;
            board[pj] = static_cast<Number>(n) - 1;
        }
        is.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return is;
}

/**
 * Computes the state of the given board. Only considers the number of possible
 * numbers in every position.
 */
BoardState classify(const Board<PossibilitySet> &board) noexcept {
    BoardState state = BoardState::SOLVED;

    wholeArea.forAllPositions([&](Position pos) -> bool {
        const PossibilitySet &pset = board[pos];
        if (pset.isEmpty()) {
            state = BoardState::INSOLVABLE;
            return false;
        }
        if (!pset.isUnique())
            state = BoardState::UNSOLVED;
        return true;
    });

    return state;
}

Position findPositionWithLeastPossibilities(
        const Board<PossibilitySet> &board) noexcept {
    Position positionWithLeastPossibilities;
    size_t leastCount = N + 1;

    wholeArea.forAllPositions([&](Position pos) -> bool {
        size_t count = board[pos].count();
        if (1 < count && count < leastCount) {
            leastCount = count;
            positionWithLeastPossibilities = pos;
        }
        return true;
    });

    return positionWithLeastPossibilities;
}


/* vim: set et sw=4 sts=4 tw=79: */
