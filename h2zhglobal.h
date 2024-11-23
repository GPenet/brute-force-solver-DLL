
struct ZHOU;
/* ZH_GLOBAL2  and ZH_GLOBAL are both "static variables for the brute force
to have a better cache effect, ZH_GLOBAL is strictly limited to what is required 
for the basic brute force; 
data used in other brute force uses are in ZH_GLOBAL2
*/
struct ZHE {// exchange area for the caller
	BF128 tpat[100],wpat;
	int* grid0;// user grid
	int npat;
	int gsol[82];
	void GetGsol(ZHOU* z);
	int GetWpat();
	void AddPat();
	inline void InitPat(BF128& pat) {
		tpat[0] = pat; npat = 1;
	}
}zhe;
struct ZH_GLOBAL2 {
	BF128 Digit_cell_Assigned[9];// init sequence
	//BF128 digit_sol[9]; // final solution per digit original sort sequence
	BF128  cells_assigned;// to avoid redundancy in new assignments 
	//uint64_t  npuz;
	int	xcell_to_guess;// , isfalse_on;
	GINT16 tgiven[81];
	int ngiven, digitsbf;// digitsbf to check minimum 8 digits
	int s17_b3_mini;// 17 search mode, 1 if minirows b3 not tested
	char * zsol,
		stdfirstsol[82],
		zerobased_solx[81];
	char  *puzfinal, *pat;
	char puz[82]; // the solved puzzle (after morph)
	// user area to receive results
	BF128* tu;
	int ntu_user, ntu;
	
};
struct ZH_GLOBAL { // global variables for the core brute force
	int nsol, // count of sols found
		lim, modevalid,// mode and limit for nsols
		withknown,ntuak,// in mode 2 (get uas) work with known uas
		single_applied,// new_single_in_Update,
		icount, ntsol,
		go_back;
		//rdigit, loop, diag, modeguess , maxindex;
	BF128  pairs,triplets,more3,*tuak;
	ZH_GLOBAL();
	inline void Init(int maxsols,int mvalid=0){
		nsol = go_back=0;
		lim = maxsols;
		modevalid = mvalid;
	}
	int Go_InitSudoku(char * ze,int lookvalid=1);
	void ValidPuzzle(ZHOU * z);

};
