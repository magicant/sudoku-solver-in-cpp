bin_PROGRAMS = sudoku
check_PROGRAMS = BoardTest SolverTest
TESTS = $(check_PROGRAMS)

sudoku_SOURCES = main.cc \
		 Board.cc Board.hh \
		 Solver.cc Solver.hh

BoardTest_SOURCES = Board.cc Board.hh BoardTest.cc Tester.inl
SolverTest_SOURCES = Solver.cc Solver.hh SolverTest.cc Tester.inl \
		     Board.cc Board.hh
