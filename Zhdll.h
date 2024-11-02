
/*
ZhouSolver.h
Based on code posted to <http://forum.enjoysudoku.com/3-77us-solver-2-8g-cpu-testcase-17sodoku-t30470.html>
by user zhouyundong_2012.
The copyright is not specified.

this is a remorphing of the code to work in recursive mode and in a 128 bit field
funtions have been added to work in generation mode
*/

/* the BIT_SET_30 pattern is defined in a band for a digit
   it is a map of candidates plus 3 bits for unsolved rows
   bits 0-8 9-17 18-26 for the rows 27-29 for unsolved rows
*/

#include "h1zhtables.h"

#include "h2zhglobal.h"

/* 2 BF 128 per digit
	equivalent to F and Fcomp in Zhou
	Last 32 bits in FD[0] is a  bits field for unknown rows
	Last 32 bits in FD[1]] contain  digit mapped
*/
// class encapsulating the brute force 

#define ISFALSEON misc.bf.u32[0]

#include "h3zhou.h"
#include "h4zhgxnzhou2zhou3.h"
