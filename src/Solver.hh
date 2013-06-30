#ifndef INCLUDED_SOLVER_HH
#define INCLUDED_SOLVER_HH 1


#include <functional>
#include "Board.hh"


extern void eliminateImpossibilities(Board<PossibilitySet> &board) noexcept;

extern void fixUniquePossibilities(Board<PossibilitySet> &board) noexcept;

extern void iterateSolutions(
        const Board<PossibilitySet> &board,
        std::function<void(const Board<Number> &solution)> resultCallback)
        noexcept(noexcept(resultCallback(Board<Number>())));


#endif // #ifndef INCLUDED_SOLVER_HH

/* vim: set et sw=4 sts=4 tw=79: */
