#include <cstdlib>
#include <stdexcept>
#include "Board.hh"

using std::abort;
using std::logic_error;


Number PossibilitySet::uniqueValue() const {
    if (!isUnique())
        throw logic_error("multiple possibilities");

    for (Number n = 0; n < N; n++)
        if (mNumbers[n])
            return n;

    abort();
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

static char separator(Position pos) {
    return pos.right().isValid() ? ' ' : '\n';
}

std::ostream &operator<<(std::ostream &os, const Board<Number> &board) {
    wholeArea.forAllPositions([&](Position pos) -> bool {
        Number n = board[pos];
        return os << (n < N ? n + 1 : 0) << separator(pos);
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
        return os << separator(pos);
    });
    return os;
}


/* vim: set et sw=4 sts=4 tw=79: */
