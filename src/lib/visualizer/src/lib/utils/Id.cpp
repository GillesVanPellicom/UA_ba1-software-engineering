//============================================================================
// Name        : Id.cpp
// Description : Auto increment ID
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be>
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/04/27
// Version     : 1.0
//============================================================================
#include "Id.h"

id Id::next() { return curr++; }