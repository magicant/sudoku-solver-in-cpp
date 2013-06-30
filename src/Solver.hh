#ifndef INCLUDED_SOLVER_HH
#define INCLUDED_SOLVER_HH 1


#include "Board.hh"


extern void eliminateImpossibilities(Board<PossibilitySet> &board) noexcept;

extern void fixUniquePossibilities(Board<PossibilitySet> &board) noexcept;


#endif // #ifndef INCLUDED_SOLVER_HH

/* vim: set et sw=4 sts=4 tw=79: */
