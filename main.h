/* CHECK SOURCE CONTROL
 *  main.h
 *  ab-bugs
 *
 *  Created by Andrew Burgess on 26/01/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

//#include "main.h" //// anything that's not in cell.h etc. needs to go in main

extern const double DT;
extern const double DX;
extern const double DX2;
extern const double DTDX;
extern const double IDX2;
extern const double DTDX2; //these are done again in main.cpp any we may not need to do them here?


/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//declares as external variables the 4 probabilities of applying the diffusion movement rule  for the 4 quadrants
extern double const ProbQ1;
extern double const ProbQ2;
extern double const ProbQ3;
extern double const ProbQ4;
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

//extern const int Distance;
extern const double DefectDefect;
extern const double DefectCooperate;
extern const double CooperateDefect;
extern const double CooperateCooperate;