#pragma once

#include <cctype>
#include <string>

#include "../../Concepts.hpp"

namespace DanTheMan827::Helpers::String {
    /**
     * @brief Converts a given string to lower case.
     *
     * This function takes a string as input and returns a new string
     * where all the characters are converted to their lower case equivalents.
     *
     * @param str The input string to be converted.
     * @return A new string with all characters in lower case.
     */
    template <Concepts::Void T = void>
    inline std::string toLowerCase(std::string const& str) {
        std::string result;

        for (char c : str) {
            result += std::tolower(static_cast<unsigned char>(c));
        }

        return result;
    }
}  // namespace DanTheMan827::Helpers::String
