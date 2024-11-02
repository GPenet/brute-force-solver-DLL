

// tables specific to the brute force zh_tables
const extern int TblRowUnsolved[8];
const extern int TblRowMask[8];// rows where single  found  000 to 111
const extern int  Tblstartblock[27]; // (i%3) * 27 in zhou brute force
const extern int TblShrinkMask[512];// existing minirows 000 to 111
const extern int TblComplexMask[512]; // keep mini rows still valid optimised process
const extern int TblMaskSingle[512]; // kill in other blocks locked column /box
const extern int TblMaskDouble[512];// kill for locked in box / column
const extern int TblColumnSingle[512]; // single in column applied to shrinked bloc
const extern int TblShrinkSingle[512]; // keep only rows with single
const extern int TblRowUniq[512]; // 1 is row not defined in block  mode  to 111
const extern T128 AssignMask_Digit[81];
const extern T128 zhoustart[19];
//const extern T128 AssignMask_OtherDigits[81];
