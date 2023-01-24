//============================================================================
// Name        : path.cpp
// Description : Definition of the path class
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/04/08
// Version     : 1.0
//============================================================================

#include "path.h"

const std::string path::_pathToCurrFile = simplify(std::string(__FILE__));

const std::string path::resFolderPath = _pathToCurrFile.substr(0, reverseFindNth(_pathToCurrFile, '/', 3)) + "/res/";

/// Converts the given path to its most simple absolute form.
/// The path needs to be absolute and is required to be syntactically valid.
/// If it is not, this function will exhibit undefined behaviour.
/// Note: This function only works on unix systems (directories seperated by: '/').
std::string path::simplify(const std::string &path) {
    // split string into segments
    std::queue<std::string> segments;
    std::string currentPart;

    for (std::string::const_iterator it = path.begin(); it != path.end(); ++it) {
        switch (*it) {
            case '/':
                // skip if it is the first char from the string
                if (currentPart.empty()) continue;

                // push the currentPart onto the queue
                segments.push(currentPart);
                currentPart.clear();
                break;
            default:
                currentPart.push_back(*it);
                break;
        }
    }

    // go over all the parts and simplify
    std::stack<std::string> resultSt;
    while (!segments.empty()) {
        const std::string &curr = segments.front();

        if (curr == ".") {
            segments.pop();
            continue;
        } else if (curr == "..")
            resultSt.pop();
        else
            resultSt.push(curr);

        segments.pop();
    }

    // reverse stack
    std::stack<std::string> reverse;
    while (!resultSt.empty()) {
        const std::string &curr = resultSt.top();

        reverse.push(curr);

        resultSt.pop();
    }

    // convert stack to string
    std::string result;
    while (!reverse.empty()) {
        const std::string &curr = reverse.top();

        result += '/', result += curr;

        reverse.pop();
    }

    result += '/';

    return result;
}

/// Finds the nth occurence of a character starting from the back
/// Returns '-1' if it can't find the nth occurence
/// Note: the 'n' is zero based (because we are real programmers :p )
int path::reverseFindNth(const std::string &str, char ch, unsigned int n) {
    int index = str.size() - 1;
    unsigned int nth = 0;
    for (std::string::const_reverse_iterator it = str.rbegin(); it != str.rend(); ++it) {
        if (*it == ch) {
            if (nth == n)
                return index;
            else
                ++nth;
        }

        --index;
    }

    return -1;
}