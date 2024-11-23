
/*
My standard start using maingp as command line analyser
contains the entry G0() called by main
*/
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define INCLUDE_DEBUGGING_CODE
#define SK0FDEBUGGING
#include <stdlib.h>
#include <sys/timeb.h>
using namespace std;
// nothing to do here
#include "maingp_cpp.h"
#include "sk0\sk0_cpp_lookup.h"
#include "sk0\sk0_cpp_floors.h"
#include "sk0\sk0_cpp_debugging.h"
#include "bf128.h"
#include "bf128_cpp.h"

#include "dllusers\u05skvcar_user.h"
#include "dllusers\u06skbf_user.h"
#include "dllusers\skgrid_minlex_user.h"


#define DO27CtoI(S,D) for (int i = 0; i < 27; i++)D[i] = S[i] - '1';
#define DO27ItoC(S,D) for (int i = 0; i < 27; i++)D[i] = S[i] + '1';
#define DO81CtoI(S,D) for (int i = 0; i < 81; i++)D[i] = S[i] - '1';
#define DO81ItoC(S,D) for (int i = 0; i < 81; i++)D[i] = S[i] + '1';


void C0() { // 
	cout << " C0_ sequential mode ind auto morphs" << endl; int cpt = 0;
	ptpgc= SkbsSetModeWithAutos();
	char ws[82]; ws[81] = 0;
	int ggg[81];
	cout << " C0_ virtual file bands -v0-  -v1- print if -v2-" << endl;
	if (SkvcatSetModeGetVCDESK(1, &pvcdesc))
		cout << " set mode failed" << endl;
	if (SkvOpennBands(sgo.vx[0], sgo.vx[1])) {
		cout << "could not open bands range " << sgo.vx[0] << " " << sgo.vx[1] << endl;
		return;
	}
	else cout << "open " << sgo.vx[0] << " " << sgo.vx[1] << " done " << endl;
	while (!SkvGetNext()) {
		if (sgo.vx[2]) {
			char ws[82]; ws[81] = 0;
			memcpy(ws, pvcdesc->g.b1, 81);
			cout << ws << " sol ";
			cout << pvcdesc->rank << " __ " << endl;
		}
		SkbsGetMinChar(pvcdesc->g.b1, ggg);
		if (sgo.vx[2])cout << ptpgc->nt << " automorphs" << endl;
		if (ptpgc->nt>1) {
			fout1 << pvcdesc->rank << ";" << ptpgc->nt << endl;
		}
		//if (++cpt > 100) return;
	}


}

void C1() {
	char ws[82]; ws[81] = 0;
	cout << " C1_ find sol from rank -v0-" << endl;
	if (SkvcatSetModeGetVCDESK(1, &pvcdesc))
		cout << " set mode failed" << endl;
	if (SkvcatFinSolForRank(sgo.vx64[0]))
		cout << "find failed" << endl;
	else {
		memcpy(ws, pvcdesc->g.b1, 81);
		cout << ws << " sol" << endl;
	}
}

void C10() {


}
/* ascii values
0-9		48-57		0-9
a-z		97-122		10-35
A-Z		65-90		36-61
%&		37-38		62-63

*/

const char* bit6 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ%&";
 //                 1234567890123456789012345678901234567890123456789012345678901234
void PuzzleInBitField(char* puz, char* d) {
	// d is 14*6=84 bits in 14 char
	for (int i = 0, k = 0; i < 14; i++) {
		register int v = 0;
		for (int j = 0; j < 6; j++,k++) {
			if (puz[k] != '.')v |= 1 << j;
			d[i] = bit6[v];
			if(k==80) break;
		}
	}
}
int Get6Bits(int i, char* bf14) {
	register int v = bf14[i];
	if (v >= 97 && v <= 122)v -= 87; // A Z
	else if (v >= 65 && v <= 90)v -= 29; // a z
	else if (v >= 48 && v <= 57)v -= 48; // 0-9
	else if (v == 37) v = 62; // %
	else if (v == 38) v = 63; // &
	else v = 0; // not valid put to empty
	return v;
}
void Expand14to81(char* puz, char* bf14, char * sol) {
	int k = 0;
	for (int i = 0, k = 0; i < 14; i++) {// 6 bits to expand
		register int v = Get6Bits(i,bf14); 
		int nd = (i == 13) ? 3 : 6;
		for (int j = 0; j < nd; j++,v>>=1,k++) {
			if (v & 1)puz[k] = sol[k] ;
			else puz[k] = '.';
		}
	}
}

void CountPairPerBox(int* gbit, int* gbitd, int fl, int* b) {
	memset(b, 0, 4 * 9); // all count to 0
	for (int iband = 0, ic = 0; iband < 3; iband++) {
		for (int irow = 0; irow < 3; irow++) {
			for (int imini = 0; imini < 3; imini++) {
				int n = 0;
				for (int i = 0; i < 3; i++, ic++) {// 3 cells
					if (gbit[ic] & fl)n++;
				}
				if (n >= 2)b[3 * iband + imini]++;
			}
		}
	}
	for (int iband = 0, ic = 0; iband < 3; iband++) {
		for (int irow = 0; irow < 3; irow++) {
			for (int imini = 0; imini < 3; imini++) {
				int n = 0;
				for (int i = 0; i < 3; i++, ic++) {// 3 cells
					if (gbitd[ic] & fl)n++;
				}
				if (n >= 2)b[3 * imini + iband]++;
			}
		}
	}
}

void PrintFl(int* gbit, int* g, int fl) {
	for (int iband = 0, ic = 0; iband < 3; iband++) {
		for (int irow = 0; irow < 3; irow++) {
			for (int imini = 0; imini < 3; imini++) {
				for (int i = 0; i < 3; i++, ic++) {// 3 cells
					if (gbit[ic] & fl)cout << g[ic]+1;
					else cout <<'.';
				}
				cout << "  ";
			}
			cout << endl;
		}
		cout << endl;
	}
	cout << endl;
}


void C11() {
	cout << " C11_ find canonical for a file of sudokus sgo.vx[4]= " << sgo.vx[4] << endl;
	char* ze = finput.ze,zem[82], zem2[82]; zem[81]=0; zem2[81] = 0;
	int smin[81];
	ptpgc= SkbsSetModeWithAutos();
	pzhe= SkbfGetZhePointer();
	int* vv = pzhe->gsol;
	if (SkvcatSetModeGetVCDESK(2, &pvcdesc)) {
		cout << " set mode failed" << endl;
		return;
	}
	while (finput.GetLigne()) {
		if (sgo.vx[4] <2 )
			if (strlen(ze) < 81)continue;// no empty line
		if (!sgo.vx[4]) {
			cout << ze << " puzzle to caniconalize" << endl;
			{
				int ir1 = SkbfCheckValidityQuick(ze);
				if (ir1 != 1) {
					cout << " invalid puzzle ir1=" << ir1 << endl;
					continue;
				}

				for (int i = 0; i < 81; i++) cout << vv[i] + 1;
				cout << " solback" << endl;
				//cout << pzhe->gsol << " sol to do min lex" << endl;
				SkbsGetMin(vv, smin);
				for (int i = 0; i < 81; i++) cout << smin[i] + 1;
				cout << " min solback" << endl;

			}
			GRIDPERM wgperm;
			char* pbase = ptpgc->t[0];
			for (int i = 0; i < 19; i++) cout << pbase[i];
			cout << " pbase nperm=" << ptpgc->nt << endl;
			wgperm.Import19(vv, pbase);
			wgperm.MorphPuzzle(ze, zem);
			cout << zem << " puz morphed" << endl;

			uint64_t r = SkvcatGetRankFromSolMin(smin);
			if (!r) {
				cout << " failed to find rank  " << endl;
				continue;
			}
			for (int i = 0; i < 81; i++) cout << smin[i] + 1;
			cout << " solmin rank  " << r << endl;
			char zbit[15]; zbit[14] = 0;
			PuzzleInBitField(zem, zbit);
			cout << zbit << "  bbit pattern for the puzzle" << endl;
			cout << r << ";" << zbit << " proposal for canonical" << endl;
			if (ptpgc->nt > 1) {
				cout << " auto morphs seen, check more" << endl;
				for (int i = 1; i < ptpgc->nt; i++) {// check auto morph get the smallest
					wgperm.Import19(vv, pbase);
					wgperm.MorphPuzzle(ze, zem2);
					register int x = 0;
					for (; x < 81; x++)if (zem[i] != zem2[i])break;
					if (i < 81 && zem2[i] == '.')memcpy(zem, zem2, 81);
				}
			}
		}
		//========================================= sudoku => can r+14
		if (sgo.vx[4] == 1) {
			//cout << ze << " puzzle to caniconalize" << endl;
			int ir1 = SkbfCheckValidityQuick(ze);
			if (ir1 != 1) {
				cout << ze << " puzzle to caniconalize" << endl;
				cout << " invalid puzzle ir1=" << ir1 << endl;
				continue;
			}
			SkbsGetMin(vv, smin);
			GRIDPERM wgperm;
			char* pbase = ptpgc->t[0];
			wgperm.Import19(vv, pbase);
			wgperm.MorphPuzzle(ze, zem);
			uint64_t r = SkvcatGetRankFromSolMin(smin);
			char zbit[15]; zbit[14] = 0;
			PuzzleInBitField(zem, zbit);
			//cout << r << ";" << zbit << " proposal for canonical" << endl;
			fout1 << r << ";" << zbit  << endl;
			if (ptpgc->nt > 1) {
				cout << ze << " puzzle to caniconalize" << endl;
				cout << " auto morphs seen, check more" << endl;
				continue;
				for (int i = 1; i < ptpgc->nt; i++) {// check auto morph get the smallest
					wgperm.Import19(vv, pbase);
					wgperm.MorphPuzzle(ze, zem2);
					register int x = 0;
					for (; x < 81; x++)if (zem[i] != zem2[i])break;
					if (i < 81 && zem2[i] == '.')memcpy(zem, zem2, 81);
				}
			}
			continue;
		}
		//========================================= can=> sudoku
		if (sgo.vx[4] == 2) {
			int i = 0;
			for (; i < 15; i++)if (ze[i] == ';') break;
			if (i > 10) {
				cout<<ze << "not expected format" <<endl;
				continue;
			}
			ze[i] = 0;
			uint64_t rank= strtoll(ze, 0, 10);
			//cout<<ze << endl << rank << " rank seen" << endl;
			char* zeb = &ze[i + 1];
			int ll =(int) strlen(zeb);
			if (ll != 14) {
				cout << "not rigth length"	<< endl;
				continue;
			}
			char zout[82]; zout[81] = 0;
			char zsol[82]; zsol[81] = 0;
			//valid to expand
			if (SkvcatFinSolForRank(rank) < 0) {
				cout << "rank false" << endl;
			}
			else {
				memcpy(zsol, pvcdesc->g.b1, 81);
				//cout << zsol << " sol found" << endl;
			}
			Expand14to81(zout, zeb, zsol); 
			//cout << zout << endl;
			fout1 << zout << endl;
			continue;
		}
		//======================================== can r+14 =>19
		if (sgo.vx[4] == 3) {// convert to 19
			cout << ze << " to convert" << endl;
			int i = 0;
			for (; i < 15; i++)if (ze[i] == ';') break;
			ze[i] = 0;
			uint64_t rank = strtoll(ze, 0, 10);
			char* zeb = &ze[i + 1];
			char z19[20]; memset(z19, 0, 20);
			register uint64_t r = rank, r2;
			{
				r2 = (r >> 27) & 0x3f; z19[0] = bit6[r2];//  highest bits
				r2 = (r >> 21) & 0x3f; z19[1] = bit6[r2];
				r2 = (r >> 15) & 0x3f; z19[2] = bit6[r2];
				r2 = (r >> 9) & 0x3f; z19[3] = bit6[r2];
				r2 = (r >> 3) & 0x3f; z19[4] = bit6[r2];
			}
			register int rx=(int)r2 & 7;// last 3  bits
			for (int i = 0; i < 14; i++) {
				register int ry = Get6Bits(i, zeb);
				rx = (rx << 3) | ry >> 3;
				z19[i + 5] = bit6[rx];
				rx = ry & 7;
			}
			fout1 << z19 << endl;
			continue;
		}
		//=============================== 19 to sudoku
		if (sgo.vx[4] == 4) {// convert to 19
			cout << ze << " to convert to sudoku" << endl;
			uint64_t rank;
			for (int i = 0; i < 5; i++) {
				register int ry = Get6Bits(i, ze);
				rank = (rank <<= 6) | ry;
			}
			register int ry = Get6Bits(5, ze);// 3 bits rank
			rank = (rank <<= 3) | (ry>>3);
			cout << rank << " expanded rank"<<endl;
			ry &= 7;// residual bits for the 14 bytes field
			char zeb[14];
	/*
			register uint64_t r = rank, r2;
			register int rx = (int)r2 & 7;// last 3  bits
			for (int i = 0; i < 14; i++) {
				register int ry = Get6Bits(i, zeb);
				rx = (rx << 3) | ry >> 3;
				z19[i + 5] = bit6[rx];
				rx = ry & 7;
			}
			fout1 << z19 << endl;
	*/
			continue;
		}
		// ======================  more tests
		if (sgo.vx[4]==10) {
			cout << ze << " check rank" << endl;
			uint64_t r = SkvcatGetRankFromSolCharMin(ze);
			if (!r) {
				cout << " failed to find rank  " << endl;
				continue;
			}
			cout << " solmin rank  " << r << endl;
			char zsol[82]; zsol[81] = 0;
			//valid to expand
			if (SkvcatFinSolForRank(r) < 0) {
				cout << "rank false" << endl;
			}
			else {
				memcpy(zsol, pvcdesc->g.b1, 81);
				cout << zsol << " sol found" << endl;
			}

		}
	}
	cout << "end of file   " << endl;
}
int cellsInGroup[27][9] =
{
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8},{ 9,10,11,12,13,14,15,16,17},{18,19,20,21,22,23,24,25,26},
	{27,28,29,30,31,32,33,34,35},{36,37,38,39,40,41,42,43,44},{45,46,47,48,49,50,51,52,53},
	{54,55,56,57,58,59,60,61,62},{63,64,65,66,67,68,69,70,71},{72,73,74,75,76,77,78,79,80},
	{ 0, 9,18,27,36,45,54,63,72},{ 1,10,19,28,37,46,55,64,73},{ 2,11,20,29,38,47,56,65,74},
	{ 3,12,21,30,39,48,57,66,75},{ 4,13,22,31,40,49,58,67,76},{ 5,14,23,32,41,50,59,68,77},
	{ 6,15,24,33,42,51,60,69,78},{ 7,16,25,34,43,52,61,70,79},{ 8,17,26,35,44,53,62,71,80},
	{ 0, 1, 2, 9,10,11,18,19,20},{ 3, 4, 5,12,13,14,21,22,23},{ 6, 7, 8,15,16,17,24,25,26},
	{27,28,29,36,37,38,45,46,47},{30,31,32,39,40,41,48,49,50},{33,34,35,42,43,44,51,52,53},
	{54,55,56,63,64,65,72,73,74},{57,58,59,66,67,68,75,76,77},{60,61,62,69,70,71,78,79,80}
}; 
int IsVali81(char* z) {
	int g[81];
	for (int i = 0; i < 81; i++) {
		char c = z[i];
		if (c < '1' || c>'9') return 0;
		g[i] = c - '1';
	}
	for (int i1 = 0; i1 < 27; i1++) {// r,c,b
		register int r = 0, * tc = cellsInGroup[i1];
		for (int i2 = 0; i2 < 9; i2++) // cells
			r |= 1 << g[tc[i2]];
		if (r != 0777) return 0;
	}
	return 1;
}
int MovCheckGrid(char* z, int* g) {
	if (!IsVali81(z)) return 1;
	for (int i = 0; i < 81; i++)
		g[i] = z[i] - '1';
	return 0;
}
void C12() {
	cout << "C12() case " << sgo.vx[4] << endl;
	pzhe = SkbfGetZhePointer();
	if (SkvcatSetModeGetVCDESK(1, &pvcdesc)) {
		cout << " set mode failed" << endl;
		return;
	}
	char* ze = finput.ze;
	uint64_t npuz = 0, maxauto = 0;
	while (finput.GetLigne()) {
		//if (strlen(ze) < 81)continue;
		if (npuz++ < sgo.vx[0])continue;
		cout << "+++++++++++++++++++++++++++";
		cout << "---------------------------";
		cout << "+++++++++++++++++++++++++++" << endl;
		//cout << ze << endl;
		switch (sgo.vx[4]) {
		case 0: {// classique
			ze[81] = 0;
			cout << ze << endl;
			int ir = SkbfCheckValidityQuick(ze);
			if (ir != 1)
				cout << "\t\t\t back ir=" << ir << endl;
			break;
		}// end case 0
		case 1: {// sol to search 2 digs uas
			ze[81] = 0;
			cout << ze << endl;
			int g0[82];
			if (MovCheckGrid(ze, g0)) {
				cout << " not valid entry" << endl;
				continue;
			}
			for (int i = 0; i < 36; i++) {
				int fl = floors_2d[i];
				int ir = SkbfGetFloorsSols(g0, fl);
				if (ir != 1) {
					cout << Char9out(fl) << " " << pzhe->npat << " pat in zhe" << endl;
					for (int i = 0; i < pzhe->npat; i++) {
						pzhe->tpat[i].PrintUa();
					}

				}
			}
			break;
		}//end case 1
		case 2: { // uas start from the 49158 file
			ze[81] = 0;
			cout << ze << endl;
			int ir = SkbfCheckValidityQuick(ze);
			if (ir != 1) {
				cout << "\t\t\t back ir=" << ir << endl;
				continue;
			}
			//cout << "  back ok " << endl;
			int* vv = pzhe->gsol;
			int mys[81];
			memcpy(mys, vv, sizeof mys);
			for (int i = 0; i < 81; i++) cout << mys[i] + 1;
			cout << " solback" << endl;
			GINT16 tc[25]; int ntc = 0;
			for (int i = 1; i < 81; i++) {// skip the first
				if (ze[i] != '.')
					tc[ntc++].u16 = (uint16_t)(i | (mys[i] << 8));
			}
			{
				int ir = SkbfCheckOkForSol(mys, tc, ntc);
				cout << " return check 16 clues " << ir << endl;
				if (ir > 10)ir = 10;
				for (int i = 0; i < ir; i++)
					pzhe->tpat[i].Print81();

			}
			break;
		}// end case 2
		case 3: {// sol to search 3 digs uas
			ze[81] = 0;
			cout << ze << endl;
			int g0[82];
			if (MovCheckGrid(ze, g0)) {
				cout << " not valid entry" << endl;
				continue;
			}
			for (int i = 0; i < 84; i++) {
				int fl = floors_3d[i];
				int ir = SkbfGetFloorsSols(g0, fl);
				if (ir != 1) {
					cout << Char9out(fl) << " " << pzhe->npat << " pat in zhe" << endl;
					for (int i = 0; i < pzhe->npat; i++) {
						pzhe->tpat[i].Print81();
					}

				}
			}
			break;
		}//end case 1

		case 4: { // study loki case 
			//ze[81] = 0;
			cout << ze << endl;
			int ir = SkbfCheckValidityQuick(ze);
			if (ir != 1) {
				cout << "\t\t\t back ir=" << ir << endl;
				continue;
			}
			//cout << "  back ok " << endl;
			int* vv = pzhe->gsol;
			int mys[81], mysb[81],mysbd[81],bcpairs[9];
			memcpy(mys, vv, sizeof mys);
			for (int i = 0; i < 81; i++) mysb[i] = 1 << mys[i];
			for (int i = 0; i < 81; i++)
				mysbd[i] =mysb[C_transpose_d[i]];

			for (int i = 0; i < 81; i++) cout << mys[i] + 1;
			cout << " solback" << endl;
			//cout << "study all 2 digits cases" << endl;
			int tone[36], ntone = 0;
			for (int i = 0; i < 36; i++) {
				int fl = floors_2d[i];
				int ir = SkbfGetFloorsSols(mys, fl);
				if (pzhe->npat == 1) {
					//cout << Char9out(fl) << endl;
					tone[ntone++] = fl;
				}
				//cout << Char9out(fl) << " " << pzhe->npat << " pat in zhe" << endl;
				//for (int i = 0; i < pzhe->npat; i++) {
				//	pzhe->tpat[i].Print81();
				//}
			}
			int t3d[100], nt3d = 0;
			cout << "look for triplets ntone=" << ntone << endl;
			if (ntone < 3)continue;// not triplet
			for (int i1 = 0; i1 < ntone-2; i1++) {
				register int r1 = tone[i1];
				for (int i2 = i1+1; i2 < ntone - 1; i2++) {
					register int r2 = tone[i2];
					if (!(r1 & r2)) continue;
					for (int i3 = i2 + 1; i3 < ntone; i3++) {
						register int r3 = tone[i3];
						if (!(r1 & r3)) continue;
						if (!(r2 & r3)) continue;
						if (_popcnt32(r1 | r2 | r3) != 3) continue;
						t3d[nt3d++] = r1 | r2;
					}
				}
			}
			if (nt3d) {
				cout << " seen triplets" << nt3d << endl;
				for (int i = 0; i < nt3d; i++) {
					CountPairPerBox(mysb, mysbd, t3d[i], bcpairs);
					int nnul = 0, brows = 0, bcols = 0;
					for (int j = 0; j < 9; j++)if (!bcpairs[j]) {
						nnul++;
						brows |= 1 << (j / 3);
						bcols |= 1 << (j % 3);
					}				

					if (nnul > 2) {
						if (_popcnt32(brows) != 2 || _popcnt32(bcols) != 2) continue;
						// must be 
						for (int j = 0; j < 9; j++) cout << bcpairs[j];
						cout << " " << Char9out(t3d[i]) << endl;
						for (int j = 0; j < 81; j++) {
							if (mysb[j] & t3d[i]) cout << mys[j] + 1;
							else cout << '.';
						}
						cout << " sol 3 digits" << endl;

					}
				}

			}
			/*
			{
				int fl = 0124; // digits 357
				int ir = SkbfGetFloorsSols(mys, fl);
				if (ir != 1) {
					cout << Char9out(fl) << " " << pzhe->npat << " pat in zhe" << endl;
					for (int i = 0; i < pzhe->npat; i++) {
						pzhe->tpat[i].Print81();
					}
				}
			}

			break;			*/
			continue;

		}// end case 4

		case 5: { // same  case 4 entry rank
			//ze[81] = 0;
			cout << ze << endl;
			int nok = 0;
			int i = 0;
			for (; i < 15; i++)if (ze[i] == ';') break;
			ze[i] = 0;
			uint64_t rank = strtoll(ze, 0, 10);
			char* zesol = pvcdesc->g.b1;
			//rank to expand
			int mys[81], mysb[81], mysbd[81], bcpairs[9];
			{
				if (SkvcatFinSolForRank(rank) < 0) {
					cout << "rank false" << endl;
					continue;
				}
				for (int i = 0; i < 81; i++) {
					mys[i] = zesol[i] - '1';
					mysb[i] = 1 << mys[i];
				}
				for (int i = 0; i < 81; i++)
					mysbd[i] = mysb[C_transpose_d[i]];

				for (int i = 0; i < 81; i++) cout << mys[i] + 1;

			}
			cout << " solback" << endl;
			//cout << "study all 2 digits cases" << endl;
			int tone[36], ntone = 0;
			for (int i = 0; i < 36; i++) {
				int fl = floors_2d[i];
				int ir = SkbfGetFloorsSols(mys, fl);
				if (pzhe->npat == 1) tone[ntone++] = fl;
			}
			int t3d[100], nt3d = 0;
			cout << "look for triplets ntone=" << ntone << endl;
			if (ntone < 3)continue;// not triplet
			for (int i1 = 0; i1 < ntone - 2; i1++) {
				register int r1 = tone[i1];
				for (int i2 = i1 + 1; i2 < ntone - 1; i2++) {
					register int r2 = tone[i2];
					if (!(r1 & r2)) continue;
					for (int i3 = i2 + 1; i3 < ntone; i3++) {
						register int r3 = tone[i3];
						if (!(r1 & r3)) continue;
						if (!(r2 & r3)) continue;
						if (_popcnt32(r1 | r2 | r3) != 3) continue;
						t3d[nt3d++] = r1 | r2;
					}
				}
			}
			if (nt3d) {
				cout << " seen triplets" << nt3d << endl;
				for (int i = 0; i < nt3d; i++) {
					CountPairPerBox(mysb, mysbd, t3d[i], bcpairs);
					int nnul = 0, brows = 0, bcols = 0;
					for (int j = 0; j < 9; j++)if (!bcpairs[j]) {
						nnul++;
						brows |= 1 << (j / 3);
						bcols |= 1 << (j % 3);
					}

					if (nnul > 2) {
						if (_popcnt32(brows) != 2 || _popcnt32(bcols) != 2) continue;
						// must be 
						for (int j = 0; j < 9; j++) cout << bcpairs[j];
						cout << " " << Char9out(t3d[i]) << endl;
						for (int j = 0; j < 81; j++) {
							if (mysb[j] & t3d[i]) cout << mys[j] + 1;
							else cout << '.';
						}
						cout << " sol 3 digits" << endl;
						nok++;
					}
				}
			}
			if (nok != 1)fout1 << ze <<";"<<nok << endl;
			/*
			{
				int fl = 0124; // digits 357
				int ir = SkbfGetFloorsSols(mys, fl);
				if (ir != 1) {
					cout << Char9out(fl) << " " << pzhe->npat << " pat in zhe" << endl;
					for (int i = 0; i < pzhe->npat; i++) {
						pzhe->tpat[i].Print81();
					}
				}
			}

			break;			*/
			continue;

		}// end case 5

		case 6: { // entry rank test all 3 digits for 3 diags
			cout << ze << endl;
			int nok = 0;
			int i = 0;
			for (; i < 15; i++)if (ze[i] == ';') break;
			ze[i] = 0;
			uint64_t rank = strtoll(ze, 0, 10);
			char* zesol = pvcdesc->g.b1;
			//rank to expand
			int mys[81], mysb[81], mysbd[81], bcpairs[9];
			{
				if (SkvcatFinSolForRank(rank) < 0) {
					cout << "rank false" << endl;	continue;	}
				for (int i = 0; i < 81; i++) {
					mys[i] = zesol[i] - '1';	mysb[i] = 1 << mys[i];	}
				for (int i = 0; i < 81; i++)	mysbd[i] = mysb[C_transpose_d[i]];
				for (int i = 0; i < 81; i++) cout << mys[i] + 1;
				cout << " solback" << endl;
			}
			//cout << "study all 3 digits cases" << endl;
			int nx = 0;
			for (int i = 0; i < 84; i++) {
				int fl = floors_3d[i];
				//int ir = SkbfGetFloorsSols(mys, fl);
				CountPairPerBox(mysb, mysbd, fl, bcpairs);
				int nnul = 0, brows = 0, bcols = 0;
				for (int j = 0; j < 9; j++)if (!bcpairs[j]) {
					nnul++;
					brows |= 1 << (j / 3);	bcols |= 1 << (j % 3);	}
				if (nnul < 3)continue;
				if (nnul == 3) {// tridagon pattern ??
					if (_popcnt32(brows) != 2 || _popcnt32(bcols) != 2) continue;
				}
				if (!nx++) {
					for (int j = 0; j < 81; j++)cout << mys[j] + 1;
					cout << "===========" << endl;
				}
				for (int j = 0; j < 9; j++) cout << bcpairs[j];
				cout << "\t\t " << Char9out(fl) << endl;
				PrintFl(mysb, mys, fl);
			}
			if (npuz > 10) return;
			continue;
			int t3d[100], nt3d = 0;

			if (nt3d) {
				int nnul = 0;
				cout << " seen triplets" << nt3d << endl;
				for (int i = 0; i < nt3d; i++) {
					if (nnul > 2) {
						// must be 
						for (int j = 0; j < 9; j++) cout << bcpairs[j];
						cout << " " << Char9out(t3d[i]) << endl;
						for (int j = 0; j < 81; j++) {
							if (mysb[j] & t3d[i]) cout << mys[j] + 1;
							else cout << '.';
						}
						cout << " sol 3 digits" << endl;
						nok++;
					}
				}
			}
			if (nok != 1)fout1 << ze << ";" << nok << endl;

			continue;

		}// end case 6

		}// end switch
		if (npuz >= sgo.vx[1])break;;
	}

}

/* strtoll example */

/*
#include <stdio.h>      // printf, NULL 
#include <stdlib.h>     // strtoll 

int main()
{
	char szNumbers[] = "1856892505 17b00a12b -01100011010110000010001101100 0x6fffff";
	char* pEnd;
	long long int lli1, lli2, lli3, lli4;
	lli1 = strtoll(szNumbers, &pEnd, 10);
	lli2 = strtoll(pEnd, &pEnd, 16);
	lli3 = strtoll(pEnd, &pEnd, 2);
	lli4 = strtoll(pEnd, NULL, 0);
	printf("The decimal equivalents are: %lld, %lld, %lld and %lld.\n", lli1, lli2, lli3, lli4);
	return 0;
}*/

void C20() {
	cout << "C20() case work on virtual seq file" << sgo.vx[4] << endl;
	cout << " C20_ find potential for tridagon" << endl; 
	pzhe = SkbfGetZhePointer();
	ptpgc = SkbsSetModeWithAutos();
	if (SkvcatSetModeGetVCDESK(0, &pvcdesc))
		cout << " set mode failed" << endl;
	if (SkvOpennBands(sgo.vx[0], sgo.vx[1])) {
		cout << "could not open bands range " << sgo.vx[0] << " " << sgo.vx[1] << endl;
		return;
	}
	else cout << "open " << sgo.vx[0] << " " << sgo.vx[1] << " done " << endl;
	cout << " C0_ virtual file bands -v0-  -v1- print if -v2-" << endl;
	int pctl = 0;
	while (!SkvGetNext()) {
		uint64_t rank = pvcdesc->rank;
		//if (++pctl <= 100)	cout << " sol rank" << pvcdesc->rank << endl;
		//else return;
		//if(rank !=76)continue;
		char ws[82]; ws[81] = 0;
		memcpy(ws, pvcdesc->g.b1, 81);
		int mys[81], mysb[81], mysbd[81], bcpairs[9];
		for (int i = 0; i < 81; i++) {
			mys[i] = ws[i] - '1';		mysb[i] = 1 << mys[i];		}
		for (int i = 0; i < 81; i++)
			mysbd[i] = mysb[C_transpose_d[i]];
		cout<<ws << "study all 2 digits cases" << endl;
		int tone[36], ntone = 0;
		for (int i = 0; i < 36; i++) {
			int fl = floors_2d[i];
			int ir = SkbfGetFloorsSols(mys, fl);
			if (pzhe->npat == 1)tone[ntone++] = fl;
		}
		int t3d[100], nt3d = 0;
		cout << "look for triplets ntone=" << ntone << endl;
		if (ntone < 3)continue;// not triplet
		for (int i1 = 0; i1 < ntone - 2; i1++) {
			register int r1 = tone[i1];
			for (int i2 = i1 + 1; i2 < ntone - 1; i2++) {
				register int r2 = tone[i2];
				if (!(r1 & r2)) continue;
				for (int i3 = i2 + 1; i3 < ntone; i3++) {
					register int r3 = tone[i3];
					if (!(r1 & r3)) continue;
					if (!(r2 & r3)) continue;
					if (_popcnt32(r1 | r2 | r3) != 3) continue;
					t3d[nt3d++] = r1 | r2;
				}
			}
		}
		int aig = 0;
		if (nt3d) {
			//cout << " seen triplets" << nt3d << endl;
			for (int i = 0; i < nt3d; i++) {
				CountPairPerBox(mysb, mysbd, t3d[i], bcpairs);
				int nnul = 0, brows = 0, bcols = 0;
				for (int j = 0; j < 9; j++)if (!bcpairs[j]) {
					nnul++;
					brows |= 1 << (j / 3);
					bcols |= 1 << (j % 3);
				}

				if (nnul > 2) {
					if (_popcnt32(brows) != 2 || _popcnt32(bcols) != 2) continue;
					// must be 
					if (!aig) {
						for (int i = 0; i < 81; i++) {
							cout << mys[i] + 1;
							fout1 << mys[i] + 1;
						}
						cout << " sol rank" << pvcdesc->rank << endl;
						fout1 << ";"<< pvcdesc->rank<<";";
						for (int j = 0; j < 9; j++) 	fout1 << bcpairs[j];
						fout1 << endl;
					}
					for (int j = 0; j < 9; j++) 	cout << bcpairs[j];
					cout << " " << Char9out(t3d[i]) << endl;
					for (int j = 0; j < 81; j++) {
						if (mysb[j] & t3d[i]) cout << mys[j] + 1;
						else cout << '.';
					}
					cout << " sol 3 digits" << endl;
					aig = 1;
				}
			}
		}
		if (!aig) continue;
		if (++pctl > 10000) break;
	}
}



void Go_0() {
	int need_input_file[3] = { 10,11,12 }, need = 0;
	for (int i = 0; i < 3; i++)
		if (sgo.command == need_input_file[i]) { need = 1; break; }


	// open  outputs files 1.txt
	if (sgo.foutput_name) {
		char zn[200];
		strcpy(zn, sgo.foutput_name);
		int ll = (int)strlen(zn);
		strcpy(&zn[ll], "_file1.txt");
		fout1.open(zn);
	}
	if (need) {// input file expected
		if (!sgo.finput_name) {
			cerr << "missing input file name" << sgo.finput_name << endl; return;
		}
		finput.open(sgo.finput_name);
		if (!finput.is_open()) {
			cerr << "error open file " << sgo.finput_name << endl;
			return;
		}
	}
	cerr << "running command " << sgo.command << endl;


	switch (sgo.command) {
	case 0: C0(); break;// virtual file sequential mode 
	case 1:C1(); break;// get sol from rank
	case 10: C10(); break;// get sol for a file of ranks
	case 11: C11(); break;
	case 12: C12(); break;
	case 20: C20(); break;

	}
	cerr << "go_0 return" << endl;
}

