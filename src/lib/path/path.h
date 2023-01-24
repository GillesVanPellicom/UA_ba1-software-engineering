//============================================================================
// Name        : path.h
// Description : Static class that holds paths to folders on the file system
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/04/08
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_PATH_H
#define SE_PROJECT_PATH_H

#include <iostream>
#include <queue>
#include <stack>
#include <string>

/// Helper class for path management
class path {
  private:
    static std::string simplify(const std::string &path);
    static int reverseFindNth(const std::string &str, char ch, unsigned int n);

    static const std::string _pathToCurrFile;

  public:
    /// absolute path to the main resource folder
    static const std::string resFolderPath;
};

#endif  // SE_PROJECT_PATH_H