#ifndef INCLUDED_BOARD_HH
#define INCLUDED_BOARD_HH 1


#include <array>
#include <bitset>
#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>


using Number = std::size_t;

static constexpr Number Nsub = 3;
static constexpr Number N = Nsub * Nsub;


class PossibilitySet {

private:

    std::bitset<N> mNumbers;

public:

    constexpr PossibilitySet() = default;
    constexpr PossibilitySet(const PossibilitySet &pset) = default;
    constexpr PossibilitySet(PossibilitySet &&pset) = default;
    ~PossibilitySet() = default;
    PossibilitySet &operator=(const PossibilitySet &pset) = default;
    PossibilitySet &operator=(PossibilitySet &&pset) = default;

    explicit /*constexpr*/ PossibilitySet(Number n) : mNumbers() {
        mNumbers.set(n);
    }

private:

    explicit constexpr PossibilitySet(const std::bitset<N> &numbers) noexcept :
            mNumbers(numbers) { }

public:

    static constexpr PossibilitySet full() noexcept {
        return PossibilitySet(std::bitset<N>((1u << N) - 1));
    }

    bool operator==(const PossibilitySet &other) const {
        return mNumbers == other.mNumbers;
    }

    bool operator!=(const PossibilitySet &other) const {
        return !(*this == other);
    }

    bool isEmpty() const noexcept {
        return mNumbers.none();
    }

    bool isUnique() const noexcept {
        return mNumbers.count() == 1;
    }

    Number uniqueValue() const;

    PossibilitySet &add(Number n) {
        mNumbers.set(n);
        return *this;
    }

    PossibilitySet &remove(Number n) noexcept {
        mNumbers.reset(n);
        return *this;
    }

};


class Position {

private:

    Number mI, mJ;

public:

    constexpr Position(Number i, Number j) : mI(i), mJ(j) { }

    constexpr Position() : Position(0, 0) { }
    constexpr Position(const Position &pos) = default;
    constexpr Position(Position &&pos) = default;
    ~Position() = default;
    Position &operator=(const Position &pos) = default;
    Position &operator=(Position &&pos) = default;

    constexpr Number i() const noexcept { return mI; }
    Number &i() noexcept { return mI; }
    constexpr Number j() const noexcept { return mJ; }
    Number &j() noexcept { return mJ; }

    constexpr bool isValid() const noexcept {
        return mI < N && mJ < N;
    }

    Position &down(Number n = 1) noexcept {
        mI += n;
        return *this;
    }

    Position &right(Number n = 1) noexcept {
        mJ += n;
        return *this;
    }

};

static inline bool operator==(const Position &pos1, const Position &pos2) {
    return pos1.i() == pos2.i() && pos1.j() == pos2.j();
}

static inline bool operator!=(const Position &pos1, const Position &pos2) {
    return !(pos1 == pos2);
}


class Area {

private:

    Position mTopLeft, mBottomRight;

public:

    constexpr Area(Position topLeft, Position bottomRight) :
            mTopLeft(topLeft), mBottomRight(bottomRight) { }

    constexpr Area() : Area(Position(), Position()) { }
    constexpr Area(const Area &area) = default;
    constexpr Area(Area &&area) = default;
    ~Area() = default;
    Area &operator=(const Area &area) = default;
    Area &operator=(Area &&area) = default;

    constexpr const Position &topLeft() const noexcept {
        return mTopLeft;
    }

    Position &topLeft() noexcept {
        return mTopLeft;
    }

    constexpr const Position &bottomRight() const noexcept {
        return mBottomRight;
    }

    Position &bottomRight() noexcept {
        return mBottomRight;
    }

    bool contains(const Position &pos) const noexcept;

    bool forAllPositions(std::function<bool(Position)> f)
            const noexcept(noexcept(f(Position())));

};

static inline bool operator==(const Area &area1, const Area &area2) {
    return area1.topLeft() == area2.topLeft() &&
            area1.bottomRight() == area2.bottomRight();
}

static inline bool operator!=(const Area &area1, const Area &area2) {
    return !(area1 == area2);
}

constexpr Area wholeArea = Area(Position(), Position(N, N));

static inline Area rowArea(Number i) {
    return Area(Position(i, 0), Position(i, N).down());
}

static inline Area rowArea(const Position &pos) {
    return rowArea(pos.i());
}

static inline Area columnArea(Number j) {
    return Area(Position(0, j), Position(N, j).right());
}

static inline Area columnArea(const Position &pos) {
    return columnArea(pos.j());
}

extern Area blockArea(const Position &pos);


template <typename T>
class Board {

public:

    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

private:

    std::array<value_type, N * N> mValues;

    constexpr Number index(const Position &pos) const {
        return pos.i() * N + pos.j();
    }

public:

    constexpr Board() = default;
    constexpr Board(const Board &board) = default;
    constexpr Board(Board &&board) = default;
    ~Board() = default;
    Board &operator=(const Board &board) = default;
    Board &operator=(Board &&board) = default;

    const_reference at(const Position &pos) const {
        return mValues.at(index(pos));
    }

    reference at(const Position &pos) {
        return mValues.at(index(pos));
    }

    constexpr const_reference operator[](const Position &pos) const noexcept {
        return mValues[index(pos)];
    }

    reference operator[](const Position &pos) noexcept {
        return mValues[index(pos)];
    }

};

template <typename T>
bool operator==(const Board<T> &b1, const Board<T> &b2)
        noexcept(noexcept(b1[Position()] == b2[Position()])) {
    return wholeArea.forAllPositions([&](Position pos) {
        return b1[pos] == b2[pos];
    });
}

template <typename T>
bool operator!=(const Board<T> &b1, const Board<T> &b2)
        noexcept(noexcept(b1[Position()] == b2[Position()])) {
    return !(b1 == b2);
}

extern void convert(
        const Board<Number> &srcBoard,
        Board<PossibilitySet> &destBoard)
        noexcept;
extern void convert(
        const Board<PossibilitySet> &srcBoard,
        Board<Number> &destBoard)
        noexcept;

extern std::ostream &operator<<(
        std::ostream &os,
        const Board<Number> &board);
extern std::ostream &operator<<(
        std::ostream &os,
        const Board<PossibilitySet> &board);

extern std::istream &operator>>(std::istream &is, Board<Number> &board);


#endif // #ifndef INCLUDED_BOARD_HH

/* vim: set et sw=4 sts=4 tw=79: */
