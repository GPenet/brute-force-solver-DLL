#pragma once
//skbf_user.h - find solution of a puzzle and UAs in specific contitions

/*
Based on code posted to <http://forum.enjoysudoku.com/3-77us-solver-2-8g-cpu-testcase-17sodoku-t30470.html>
by user zhouyundong_2012.
The copyright is not specified.

This DLL first and main task is to  find the unique solution of a valid puzzle.

The DLL is also used, when the solution grid is known,
to find the minimal set of unavoidable setsin specific conditions
   for any pair or triplet of digits not assigned (the rest fully assigned)
   when the puzzle still have more than one solution (then the smallest UAs only).

The limit of 2/3 digits and the limit to the smallest UAs fits with the uses done on my side in the 17/18 clues scan.
Unavoidable sets with more than 2 digits are searched, but only with 2 bands and using UAs produced with less digits.
And the search of UAs with 17/18 clues is done with sets of clues hitting all known UAs of a big list.

As in other DLLs, an exchange area is there to send back results to the user.

struct ZHE {// exchange area for the caller
	BF128 tpat[100],wpat;
	int* grid0;// user grid
	int npat;
	int gsol[82];
	}
}

gsol is the solution grid for a sudoku (one solution for the puzzle)
tpat /npat delivers the list of UAs in a 128 bits fields (4 x 32 bits)
where the three bands (3 x 27 bits) are located in the 3 first 32 bits integers.
This is the way puzzles are processed in this process.
Note: tpat is open to 100 UAs, but the limit in the code is currently 50


The DLL has only four entries 
=======================
ZHE* SkbfGetZhePointer(); returning the pointer to the exchange area
==================
int SkbfCheckValidityQuick(char* puzzle);
  return 	0 if the puzzle is not valid, 
		1 if the solution is unique
		2 i the puzzle has mre than one solution
===================
int SkbfGetFloorsSols(int* sol ,int floors);
floors is a 9 bits field telling which digits are unknown (2 or 3 digits)
 eg .1..1...  digits 2 and 5  
	integer value 	2^1+2^4 or 
			1<<0+1<<4 or
    			18
int * sol is  int [81] set of values 0-8 for the solution grid

At the end, tpat contains the list of unavoidable sets orders from the smallest (in size) to the biggest, with no subset/superset.
========================
int SkbfCheckOkForSol(int* sol, GINT16* t, int n);
This is a process to call with a puzzle very close to a valid one.
I made the test with 17 clues valid puzzles reduced to 16, this can have a very big number of solutions. 
int* sol is the known solution as above.

GINT16* t, int n  is the list of clues given in the internal format of the process (after analysis of char * puzzle)
GINT16 is a 16 bits field having here 8 low bits for the cell and 8 high bits for the digit
	eg for a cell c (0-80) and adigit d (0-8) the clue is (uint16_t)(c | (d << 8));

the return value is the number of UAs
and the UAs are as above, in the limit of the maximum (50) number of UAs
*/
#include "bf128.h"
struct ZHE {// exchange area for the caller
	BF128 tpat[100],wpat;
	int* grid0;// user grid
	int npat;
	int gsol[82];
	
}*pzhe;
/*
// if needed
typedef union GINT16_t {
	uint16_t   u16;
	uint8_t    u8[2];
} GINT16
*/


extern "C" __declspec(dllimport) int SkbfCheckValidityQuick(char* puzzle);
extern "C" __declspec(dllimport) int SkbfGetFloorsSols(int* sol ,int floors);
extern "C" __declspec(dllimport) int SkbfCheckOkForSol(int* sol, GINT16* t, int n);
extern "C" __declspec(dllimport) ZHE* SkbfGetZhePointer();

