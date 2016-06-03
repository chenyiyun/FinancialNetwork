#ifndef PARAM_H_INCLUDED
#define PARAM_H_INCLUDED


#define LARGE_NUM  1.0e10


// on graph structure
#define ThrHld  0.1

#define NumNodes 	10
#define CONNECT_PARAM	0.02

#define UPPER_WEIGHT	5
#define LOWER_WEIGHT	1


// on objective function
#define SIMU_NUM	500		//or 5000

//+++
//extern int TARGET_NUM;
//+++

// on problem type
#define TARGET_NUM 	 8


// on solve type
#define SOLVE_TYPE  1	// 1-7
						/***
							1  value greedy
							2  distance					(SPL with r=1)
							3  longpath greedy			(MPLR with r=1)
							4  forest weight greedy		(SPFWR with r=1)
							5  2+1						(SPL)
							6  3+1						(MPLR)
							7  4+1						(SPFWR)
						***/

#define DISSIMTYPE  3   // 1-3
						/***
							1  1/x
							2  sum x / x
							3  upper-x
						***/

#define TOPK  8


#endif // PARAM_H_INCLUDED
