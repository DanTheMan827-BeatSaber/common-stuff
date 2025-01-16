#pragma once

#include <string>
#include <cctype>

namespace Helpers {
    /**
     * @brief Converts a given string to lowercase.
     *
     * This function takes a string as input and returns a new string
     * where all the characters are converted to their lowercase equivalents.
     *
     * @param str The input string to be converted.
     * @return A new string with all characters in lowercase.
     */
    inline std::string toLowercase(const std::string& str) {
        std::string result;

        for (char c : str) {
            result += std::tolower(static_cast<unsigned char>(c));
        }

        return result;
    }
}
