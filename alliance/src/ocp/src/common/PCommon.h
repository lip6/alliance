#ifndef __PCOMMON_H
#define __PCOMMON_H

#include <iostream>
#include "mut.h"
#include "mlo.h"
#include "mph.h"

using namespace std;

const double SquareShape(const double margin, const double sumwidth,
	const double minwidth, int& nrows);

bool IsSpecialNet(const losig* const sig);
bool IsTie(const phins* ins);
ostream& PrintLocon(ostream& os, const locon* const con);
ostream& PrintLosig(ostream& os, const losig* const sig);
unsigned NbCons(const losig* const sig);

#define Max(i,j) (((i) > (j)) ? (i) : (j))
#define Min(i,j) (((i) < (j)) ? (i) : (j))

#endif /* __PCOMMON_H */
