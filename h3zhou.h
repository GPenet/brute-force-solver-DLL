
struct ZHOU{// size 32 bytes 
	BF128 FD[9][2];
	BF128 cells_unsolved,misc;// misc used to check false
//________________________________________
	int CheckValidityQuick(char *puzzle);
	int GetFloorsSols(int* puzzle,int floors,int withknown=0);
	int CheckOkForSol(int* sol, GINT16* t, int n, int withknown = 0);
	int FullUpdate();
	int ApplySingleOrEmptyCells();
	void Guess();
	//void GuessGo(int dig, BF128& s);
	void GuessInCell(BF128 & f,int nd);
	int GuessHiddenBivalue();
	void Assign(int digit, int cell, int xcell);
	int Update();
	int InitSudoku(GINT16 * t, int n);
	char * SetKnown(char * zs);
	void SetaCom(int digit, int cell, int xcell);

	inline int Unsolved_Count(){ return cells_unsolved.Count(); }
	inline void ComputeNext(){ 	if (FullUpdate())Guess();}

#ifdef ISBUIDON
	// debugging code or print code
	int GetAllDigits(int cell);
	void ImageCandidats();
	void GetFirstMap(char* puzzle);
#endif 


 };
 