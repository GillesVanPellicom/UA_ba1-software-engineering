//============================================================================
// Name        : Utils.h
// Description : Inline utility funtions used to convert types and other small functionality
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/04/01
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_UTILS_H
#define SE_PROJECT_UTILS_H

#include <sstream>

#include <sstream>
#include <string>

namespace Utils {
    /**
     * Converts a string to the integer value it represents
     * @param s
     * @return int
     */
    inline int stoi(const std::string &s) {
        int result = 0;
        int factor = 1;
        for (std::string::const_reverse_iterator it = s.rbegin(); it != s.rend(); ++it) {
            if (it == s.rend() - 1 && *it == 45) {
                result *= -1;
            } else if (!(48 <= *it && *it <= 57)) {
                throw std::runtime_error("[Utils::stoi] Conversion error, cannot convert '" + s + "' to an integer.");
            } else {
                result += (*it - '0') * factor;
                factor *= 10;
            }
        }
        return result;
    }

    inline double stod(const std::string &s) {
        std::stringstream str(s);
        double tmp = 0;
        str >> tmp;
        return tmp;
    }
}  // namespace Utils

#endif  // SE_PROJECT_UTILS_H
