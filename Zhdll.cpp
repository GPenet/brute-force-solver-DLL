/*
Based on code posted to <http://forum.enjoysudoku.com/3-77us-solver-2-8g-cpu-testcase-17sodoku-t30470.html>
by user zhouyundong_2012.
The copyright is not specified.
*/
//====================================
//#define ISBUIDON
// ________________________ select what has to be loaded in sk0__h.h
//#define SK0FDEBUGGING_EXT
#define SK0FLOOKUP
#define SK0FSMALLBANDFUNCTIONS_EXT
//#define SK0FOTHERS
// SK0FT128 contains include sk_bitfield.h
#define SK0FT128
// "obsolete combine cell-fix vv9
//#define SK0FZZ
#include "maingp_user.h"

//#include "maingp_cpp.h"
#include "sk0__h.h"
#include "sk0_loads_cpp.h"
#include "sk0__hext.h"
//#include "bf128_cpp.h"

#include "Zhdll.h"
#include "Zhtables_cpp.h" // also describes the pattern


//#define DIAG
 

//============================= ZH_GLOBAL code and workinfg areas
ZH_GLOBAL2 zh_g2;
ZH_GLOBAL zh_g;
ZHOU  zhou_ip;//
ZHOU zhou[50]; // must host main brute force plus minimality analysis and recursive generation
/*
extern "C" __declspec(dllexport) int SkbfCheckValidityQuick(char* puzzle);
extern "C" __declspec(dllexport) int SkbfGetFloorsSols(int* puzzle ,int floors);
extern "C" __declspec(dllexport) int SkbfCheckOkForSol(int* sol, GINT16* t, int n);
extern "C" __declspec(dllexport) ZHE* SkbfGetZhePointer();
extern "C" __declspec(dllexport) void SkbfKnown(BF128* t, int n);
*/

#ifdef ISBUIDON
#include "Zhn_doc_debug_cpp.h"

void ZHOU::GetFirstMap(char* puzzle) {
	zh_g.Init(1);// maxsols=1
	zh_g.Go_InitSudoku(puzzle);
	ImageCandidats();
}
void GetFirstMap(char* puzzle) {
	zhou[0].GetFirstMap(puzzle);
}
#else
extern "C" __declspec(dllexport) int SkbfCheckValidityQuick(char* puzzle);
extern "C" __declspec(dllexport) int SkbfGetFloorsSols(int* puzzle, int floors);
extern "C" __declspec(dllexport) int SkbfCheckOkForSol(int* sol, GINT16 * t, int n);
extern "C" __declspec(dllexport) ZHE * SkbfGetZhePointer();
extern "C" __declspec(dllexport) void SkbfKnown(BF128 * t, int n);
#include "bf128_cpp.h"

#endif 
//=============== entries 
int SkbfCheckValidityQuick(char* puzzle) {
	return zhou[0].CheckValidityQuick(puzzle);
}
void SkbfKnown(BF128* t, int n) {
	zh_g.tuak = t; zh_g.ntuak = n;
}
int SkbfGetFloorsSols(int* puzzle ,int floors,int withknown) {
	return zhou[0].GetFloorsSols(puzzle,floors,withknown);
}
int SkbfCheckOkForSol(int* sol, GINT16* t, int n, int withknown) {
	return zhou[0].CheckOkForSol(sol, t,n, withknown);
}


ZHE* SkbfGetZhePointer() {	return &zhe;}

ZH_GLOBAL::ZH_GLOBAL() {
	//diag = 0;
	modevalid = withknown = ntuak = 0;
	//modeguess = 1;
	zh_g2.zsol = 0; // no solution unless required buy the user
	zh_g2.ntu_user = 0; // no solution unless required buy the user
}

int ZH_GLOBAL::Go_InitSudoku(char* ze, int lookvalid) {
	ze[81] = 0;
	strcpy(zh_g2.puz, ze);
	zh_g2.ngiven = 0;
	int digs = 0;
	for (int i = 0; i < 81; i++) {
		register int c = ze[i];
		if (c < '1' || c>'9') continue;
		c -= '1';
		digs |= 1 << c;
		zh_g2.tgiven[zh_g2.ngiven++].u16 = (uint16_t)(i | (c << 8));
	}
	if(lookvalid)// must be a valid puzzle quick exit
		if (_popcnt32(digs) < 8) return 1; 
	return zhou[0].InitSudoku(zh_g2.tgiven, zh_g2.ngiven);
}


void ZHE::GetGsol(ZHOU* z) {// must be solved in z
	BF128  fd;
	for (int digit = 0; digit < 9; digit++) {
		fd = z->FD[digit][0];
		int drow = 0;
		for (int ib = 0; ib < 3; ib++) {// 3 blocs per digit
			unsigned int band = fd.bf.u32[ib];
			for (int j = 0; j < 3; j++,band >>=9,drow+=9) {
				int row = band  & 0777,irow;
				bitscanforward(irow, row);
				gsol[drow + irow] = digit;
			}
		}
	}
}
int ZHE::GetWpat() {
	memset(&wpat, 0, sizeof wpat);
	int nc = 0;
	for (int i = 0; i < 81; i++) {
		if (gsol[i] != grid0[i]) {
			nc++;	wpat.Set_c(i);	}
	}
	return nc;
}

void ZHE::AddPat() {
	if (!npat) {InitPat(wpat); return;}
	BF128 temp[100];
	int ntemp = 0, pc = wpat.Count(), iadd = npat;
	for (int i = 0; i < npat; i++) {
		BF128& p = tpat[i];
		int nc = p.Count();
		if (nc <= pc) {// forget if subset
			if (p.isSubsetOf(wpat)) return;
		}
		else  {
			if (iadd > i)iadd = i;// insert point
			if (wpat.isSubsetOf(p)) continue;
			temp[ntemp++] = p;
		}
	}
	tpat[iadd] = wpat;
	if (ntemp)memcpy(&tpat[iadd+1], temp, 16 * ntemp);
	npat = iadd + ntemp+1;
	if (npat > 50)npat = 50;// cut biggest
}

void ZH_GLOBAL::ValidPuzzle(ZHOU * z){
	nsol++;
	//cout << " valid modevalid=" << modevalid << endl;
	if (!modevalid) {// standard process
		if (nsol == 1) 	zhe.GetGsol(z);
		
		if (zh_g2.zsol && (nsol==1)) {// store the first solution
			z->SetKnown(zh_g2.zsol);
			//for (int i = 0; i < 9; i++)zh_g2.digit_sol[i] = z->FD[i][0];
		}
		if (nsol > lim)go_back = 1;
		return;
	}
	if (modevalid == 2) {// mode find uas min list
		zhe.GetGsol(z);
		//cout << " b" << endl;
		if (zhe.GetWpat()) {
			if (withknown && ntuak) {// forget if not all uas hit
				for (int i = 0; i < ntuak; i++)
					if (tuak[i].isSubsetOf(zhe.wpat)) return; // known subset
				//if ((zhe.wpat & tuak[i]).isEmpty()) return; // known subset
			}
			//zhe.wpat.PrintUa();
			zhe.AddPat();
		}
		return;
	}

	/*
	if (modevalid==1) {// 17 search stop at first solution grid
		z->SetKnown(zh_g2.zsol);
		nsol++;
		go_back = 1;
		return;
	}
	if (nsol > lim)go_back = 1;
	*/
}

void ZHOU::Assign(int digit, int cell, int xcell) {
	FD[digit][0] &= AssignMask_Digit[cell];
	cells_unsolved.Clear(xcell);
}

void ZHOU::SetaCom(int digit, int cell, int xcell) { // single in cell
	BF128* Fd = FD[digit];
	*Fd &= AssignMask_Digit[cell];
	cells_unsolved.Clear(xcell);
	BF128* RF = FD[8];
	for (; RF >= FD[0]; RF -= 2)RF->Clear(xcell);
	Fd->setBit(xcell); // restore bit for digit assigned
}

int ZHOU::InitSudoku(GINT16* t, int n) {
	BF128 Digit_cell_Assigned[9];
	memset(Digit_cell_Assigned, 0, sizeof Digit_cell_Assigned);
	memcpy(this, zhoustart, sizeof zhoustart);
	for (int ic = 0; ic < n; ic++) {
		int digit = t[ic].u8[1], cell = t[ic].u8[0], xcell = C_To128[cell];
		if (FD[digit][0].Off(xcell))  return 1;// check not valid entry
		Assign(digit, cell, xcell);
		Digit_cell_Assigned[digit].Set(xcell);
	}
	BF128 w = cells_unsolved; w.bf.u32[3] = ~0;
	for (int i = 0; i < 9; i++)  FD[i][0] &= w | Digit_cell_Assigned[i];
	return 0;
}

int ZHOU::CheckValidityQuick(char* puzzle) {
	zh_g.Init(1);// maxsols=1
	if (zh_g.Go_InitSudoku(puzzle)) return 0;
	//ImageCandidats();
	int ir = FullUpdate();
	//ImageCandidats();
	if (!ir) return 0;// not valid
	if (ir == 2) {
		zh_g.ValidPuzzle(this);
		return 1;// solved can not be multiple
	}

	Guess();
	return zh_g.nsol;
}

int ZHOU::GetFloorsSols(int * sol, int floors,int withknown) {
	//cout << "_________________ floors " << floors << endl;
	zh_g.withknown = withknown;
	zhe.grid0 = sol;
	//if (_popcnt32(floors) > 3) return 0;// done for 2/digits at most
	zh_g.Init(100,2);// maxsols=100, mode=2
	zhe.npat = 0;
	BF128 Digit_cell_Assigned[9], pat;
	memset(Digit_cell_Assigned, 0, sizeof Digit_cell_Assigned);
	pat = Digit_cell_Assigned[0];// 
	memcpy(this, zhoustart, sizeof zhoustart);
	for (int ic = 0; ic < 81; ic++) {
		int dig = sol[ic], bcell = 1 <<dig,xcell= C_To128[ic];
		if (bcell & floors) {// one of the digits to keep
			pat.Set(xcell);// is in the full UA
			continue;
		}
		Assign(dig, ic, xcell);
		Digit_cell_Assigned[dig].Set(xcell);

	}
	BF128 w = cells_unsolved; w.bf.u32[3] = ~0;
	for (int i = 0; i < 9; i++)  FD[i][0] &= w | Digit_cell_Assigned[i];
	//ImageCandidats();
	//pat.Print81();
	//zhe.InitPat(pat);// to check add
	ApplySingleOrEmptyCells();

	Guess();
	//int ir = FullUpdate();
	//if(zhe.npat!=1)cout << "found npat" << zhe.npat << endl;
	return zhe.npat;
}
int  ZHOU::CheckOkForSol(int* sol, GINT16* t, int n, int withknown) {
	zh_g.withknown = withknown;
	zhe.grid0 = sol;
	zh_g.Init(100, 2);// maxsols=100, mode=2
	zhe.npat = 0;
	InitSudoku(t, n);
	//ImageCandidats();
	//return 0;
	int ir = FullUpdate();
	//ImageCandidats();
	if (!ir) return 0;// not valid
	if (ir == 2) {
		if (zh_g2.zsol)zh_g.ValidPuzzle(this);
		return zhe.npat;// solved can not be multiple
	}
	Guess();
	//cout << "found npat" << zhe.npat << endl;
	return zhe.npat;

}


int ZHOU::FullUpdate() {
	if (zh_g.go_back) return 0;
	while (1) {
		if (!Update()) return 0; // game locked in update
		if (!Unsolved_Count()) return 2;
		if (ApplySingleOrEmptyCells())	return 0; // locked empty cell or conflict singles in cells
		if (!zh_g.single_applied)	break;
	}
	return 1;
}

int ZHOU::ApplySingleOrEmptyCells() {
	zh_g.single_applied = 0;
	// singles and up to 3 in cells searched
	BF128 R1, R2, R3, R4;
	{
		R1 = FD[0][0];
		BF128 R = FD[1][0]; //current Digit pattern 
		R2 = R1 & R; R1 |= R;
		R = FD[2][0];	R3 = R2 & R; R2 |= R1 & R; R1 |= R;
		R = FD[3][0]; R4 = R3 & R; R3 |= R2 & R; R2 |= R1 & R; R1 |= R;
		for (register int i = 4; i < 9; i++) {
			R = FD[i][0]; R4 |= R3 & R; R3 |= R2 & R;
			R2 |= R1 & R; R1 |= R;
		}
	}
	if ((cells_unsolved - R1).isNotEmpty()) 	return 1; // empty cells
	R1 -= R2; // now true singles
	R1 &= cells_unsolved; // these are new singles
	if (R1.isNotEmpty()) {
		int tcells[80], ntcells = R1.Table3X27(tcells);
		for (int i = 0; i < ntcells; i++) {
			int cell = tcells[i];
			for (int idig = 0; idig < 9; idig++) {
				if (FD[idig][0].On_c(cell)) {
					Assign(idig, cell, C_To128[cell]);
					goto nextr1;
				}
			}
			return 1; // conflict with previous assign within this lot
		nextr1:;
		}
		zh_g.single_applied = 1;
		return 0;
	}
	// no single store pairs triplets or more 
	zh_g.pairs = R2 - R3;
	zh_g.triplets = R3 - R4;
	zh_g.more3 = R4;
	return 0;
}

void ZHOU::Guess() {
	if (zh_g.go_back) return;
	if (cells_unsolved.isEmpty()) {
		zh_g.ValidPuzzle(this);
		return;
	}
	uint64_t step = this - &zhou[0];
	if (zh_g.pairs.isNotEmpty()) { GuessInCell(zh_g.pairs,2);	return; }
	if (GuessHiddenBivalue()) return;
	//if (step > 0)ImageCandidats();
	//cout << "try triplet"<< step << endl;
	if (zh_g.triplets.isNotEmpty()) { GuessInCell(zh_g.triplets, 3);	return; }
	//ImageCandidats();
	//cout << "try more than triplet step"<< step << endl;
	{ GuessInCell(zh_g.more3, 4);	return; }
}
void ZHOU::GuessInCell(BF128& f, int nd) {
	// nd can be 2digits, 3 digits ,4 "more digits"
	// first do it in the band with less free cells 
	int freecells = 30, ilow,cr;
	uint32_t  b, bw;
	for (int i = 0; i < 3; i++) {
		if( (bw = f.bf.u32[i])){
			register int nw= _popcnt32(cells_unsolved.bf.u32[i]);
			if (nw < freecells) {	ilow = i;	b = bw;		}
		}
	}
	bitscanforward(cr, b);// get first relative cell to process
	int cell = 27 * ilow + cr, xcell = 32 * ilow + cr;// cell to process 
	int ndig = 0;
	if (nd < 4) {
		for (int idig = 0; idig < 9; idig++)if (FD[idig][0].On(xcell)) {
			if (++ndig < nd) {
				ZHOU* mynext = this + 1; // start next guess
				*mynext = *this;
				mynext->SetaCom(idig, cell, xcell);
				mynext->ComputeNext();
				if (zh_g.go_back) return;
			}
			else {
				SetaCom(idig, cell, xcell);
				ComputeNext();
				return;
			}
		}
	}
	//cout << " guess on cells >=4 digits" << endl;
	// nearly impossible do all in next "zhou"
	for (int idig = 0; idig < 9; idig++)if (FD[idig][0].On(xcell)) {
		ZHOU* mynext = this + 1; // start next guess
		*mynext = *this;
		mynext->SetaCom(idig, cell, xcell);
		mynext->ComputeNext();
		if (zh_g.go_back) return;
	}
}
int ZHOU::GuessHiddenBivalue() {// look a hidden pair in row or box
	uint32_t hidden;
	int idig;
	int dcell, dxcell;
	for (idig = 0; idig < 9; idig++) {
		BF128& fd = FD[idig][0];
		register int Rows;
		if (!(Rows = fd.bf.u32[3])) continue;
		if (Rows & 7) {//try row band1
			dcell = dxcell = 0;
			register int  band = fd.bf.u32[0];
			hidden = band & 0777;			if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0777000;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0777000000;		if (_popcnt32(hidden) == 2)goto exitok;
		}
		if (Rows & 070) {// try row band2
			dcell = 27; dxcell = 32;
			register int  band = fd.bf.u32[1];
			hidden = band & 0777;			if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0777000;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0777000000;		if (_popcnt32(hidden) == 2)goto exitok;
		}
		if (Rows & 0700) {//try row band3
			dcell = 54; dxcell = 64;
			register int  band = fd.bf.u32[2];
			hidden = band & 0777;			if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0777000;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0777000000;		if (_popcnt32(hidden) == 2)goto exitok;
		}
	}
	// no bi value row, try bi value box
	for (idig = 0; idig < 9; idig++) {// priority to high digits last done
		BF128& fd = FD[idig][0];
		register int Rows;
		if (!(Rows = fd.bf.u32[3])) continue;
		if (Rows & 7) {//try bow band1
			dcell = dxcell = 0;
			register int  band = fd.bf.u32[0];
			hidden = band & 07007007;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 070070070;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0700700700;		if (_popcnt32(hidden) == 2)goto exitok;
		}
		if (Rows & 070) {// try box band2
			dcell = 27; dxcell = 32;
			register int  band = fd.bf.u32[1];
			hidden = band & 07007007;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 070070070;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0700700700;		if (_popcnt32(hidden) == 2)goto exitok;
		}
		if (Rows & 0700) {//try bow band3
			dcell = 54; dxcell = 64;
			register int  band = fd.bf.u32[2];
			hidden = band & 07007007;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 070070070;		if (_popcnt32(hidden) == 2)goto exitok;
			hidden = band & 0700700700;		if (_popcnt32(hidden) == 2)goto exitok;
		}
	}
	return 0;
exitok:
	//cout << " biv " << idig + 1 << " dxc " << dxcell << endl;
	uint32_t res;
	bitscanforward(res, hidden);
	ZHOU* mynext = this + 1; // start next guess
	*mynext = *this;
	mynext->SetaCom(idig, res + dcell, res + dxcell);
	mynext->ComputeNext();
	if (zh_g.go_back) return 1;
	bitscanreverse(res, hidden);
	SetaCom(idig, res + dcell, res + dxcell);
	ComputeNext();
	return 1;
}


//=================== defines 
#define UPD_AND_CL(W,X)	last_assigned = W+1;cur_assigned = 0;\
B = ~(A & TblRowMask[S]);\
cells_unsolved.bf.u32[X] &= B;\
FD[0][0].bf.u32[X] &= B; FD[1][0].bf.u32[X] &= B; FD[2][0].bf.u32[X] &= B; \
FD[3][0].bf.u32[X] &= B; FD[4][0].bf.u32[X] &= B; FD[5][0].bf.u32[X] &= B; \
FD[6][0].bf.u32[X] &= B; FD[7][0].bf.u32[X] &= B; FD[8][0].bf.u32[X] &= B; }\
FD[W][0].bf.u32[X] = FD[W][1].bf.u32[X] = A;

#define UPD_012(W,X,Y,Z)A = FD[W][0].bf.u32[X];\
if (A != FD[W][1].bf.u32[X]){\
Shrink = (TblShrinkMask[A & 0x1FF]\
| TblShrinkMask[(A >> 9) & 0x1FF] << 3\
| TblShrinkMask[(A >> 18)] << 6);\
if ((A &= TblComplexMask[Shrink]) == 0)  return 0;\
S = ((A | (A >> 9) | (A >> 18)) & 0x1FF);\
B = FD[W][0].bf.u32[Y];\
FD[W][0].bf.u32[Z] &= TblMaskSingle[S] & TblMaskDouble[S | ((B | (B >> 9) | (B >> 18)) & 0x1FF)];\
B = FD[W][0].bf.u32[Z];\
FD[W][0].bf.u32[Y] &= TblMaskSingle[S] & TblMaskDouble[S | ((B | (B >> 9) | (B >> 18)) & 0x1FF)];\
S = TblRowUniq[TblShrinkSingle[Shrink] & TblColumnSingle[S]];

#define UPD_ONE_DIGIT(W) if (cur_assigned > W+1)goto exit_digits;\
	if (FD[W][0].bf.u64[0] != FD[W][1].bf.u64[0]\
|| FD[W][0].bf.u32[2] != FD[W][1].bf.u32[2]){\
r_free = FD[W][0].bf.u32[3];\
UPD_012(W, 0, 1, 2)	if ((r_free & 7) != S){\
r_free &= 0770 | S;	UPD_AND_CL(W, 0)}\
UPD_012(W, 1, 0, 2)	if (((r_free >> 3) & 7) != S){\
r_free &= 0707 | (S << 3);	UPD_AND_CL(W, 1)}\
UPD_012(W, 2, 0, 1)	if (((r_free >> 6) & 7) != S){\
r_free &= 077 | (S << 6);	UPD_AND_CL(W, 2)}\
FD[W][0].bf.u32[3] = r_free;}


//============ update init ...

int ZHOU::Update() {
	register uint32_t Shrink = 1, r_free, B, A, S, last_assigned = 0, cur_assigned;
loop_upd:
	cur_assigned = last_assigned; last_assigned = 0;
	if (FD[8][0].bf.u32[3]) { UPD_ONE_DIGIT(8) }
	if (FD[7][0].bf.u32[3]) { UPD_ONE_DIGIT(7) }
	if (FD[6][0].bf.u32[3]) { UPD_ONE_DIGIT(6) }
	if (FD[5][0].bf.u32[3]) { UPD_ONE_DIGIT(5) }
	if (FD[4][0].bf.u32[3]) { UPD_ONE_DIGIT(4) }
	if (FD[3][0].bf.u32[3]) { UPD_ONE_DIGIT(3) }
	if (FD[2][0].bf.u32[3]) { UPD_ONE_DIGIT(2) }
	if (FD[1][0].bf.u32[3]) { UPD_ONE_DIGIT(1) }
	if (FD[0][0].bf.u32[3]) { UPD_ONE_DIGIT(0) }
exit_digits:
	if (last_assigned) goto loop_upd;// nothing to do in the next cycle
	return 1;
}



char * ZHOU::SetKnown(char * zs){
	memset(zs, '.', 81); zs[81] = 0;
	BF128  fd;
	for (int digit = 0; digit < 9; digit++){
		fd = FD[digit][0]; // unsolved or partially solved, still in zhou
		if (fd.bf.u32[3] == 0777) continue;
		for (int ib = 0; ib < 3; ib++){// 3 blocs per digit
			int arows = (fd.bf.u32[3] >> (3 * ib)) & 7;
			if (arows == 7) continue; // not assigned
			unsigned int band = fd.bf.u32[ib];
			for (int j = 0; j<3; j++) if (!(arows & (1 << j))) {
				int row = (band >> TblMult9[j]) & 0x1ff;
				uint32_t  irow;
				bitscanforward(irow, row);
				int	cell = Tblstartblock[ib] + TblMult9[j] + irow;
				zs[cell] = digit + '1';
			}
		}
	}
	return zs;
}

