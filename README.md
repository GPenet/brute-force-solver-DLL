# brute-force-solver-DLL
This DLL solves a sudoku using a process derived from the code posted to <http://forum.enjoysudoku.com/3-77us-solver-2-8g-cpu-testcase-17sodoku-t30470.html>
by user zhouyundong_2012.
The copyright of this codeis not specified.

The basic case is limited to the delivery of the unique solution of a valid sudoku.

Other entries produce unavoidable sets of the solution grid in two main situations

All digits assigned but 2 or 3
Check a puzzle not too far from a valid sudoku.

In both cases, the code delivers the minimal list of unavoidable sets ordered in increasing cont of the number of cells of the unavoidable set.

Note, another DLL will come later, limited to the first 2 bands of a sudoku, givind  nearly exhaustive list of Unavoidable sets of reasonable size.
