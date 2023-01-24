//============================================================================
// Name        : Id.h
// Description : Auto increment ID
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be>
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/04/27
// Version     : 1.0
//============================================================================
#ifndef SE_PROJECT_ID_H
#define SE_PROJECT_ID_H

typedef unsigned int id;

/// Autoincrement class to generate a unique id
class Id {
    id curr = 0;

  public:
    Id() = default;

    /// Returns the current counter and increments it by one
    id next();
};

#endif  // SE_PROJECT_ID_H