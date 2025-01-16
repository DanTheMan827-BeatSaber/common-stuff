#include <string>
#include <utility>

/**
 * Replaces multiple substrings in a given string with their corresponding replacements.
 *
 * This function iterates through a list of target-replacement pairs and performs
 * the replacement of all occurrences of the target substring with the replacement
 * substring in the input string.
 *
 * Example:
 * ```
 * std::string text = "Hello world! Welcome to the C++ world!";
 * std::string updatedText = replaceMultiple(text, {
 *     {"world", "universe"},
 *     {"C++", "C++ programming"}
 * });
 * std::cout << updatedText;  // Output: "Hello universe! Welcome to the C++ programming universe!"
 * ```
 *
 * @param str The original string.
 * @param replacements An initializer list of pairs where each pair consists of a
 *        target substring (first) and a replacement substring (second).
 * @return A new string with all replacements applied.
 */
inline std::string replaceMultiple(const std::string& str, const std::initializer_list<std::pair<std::string, std::string>>& replacements) {
    std::string result;  // Resulting string after replacements

    // Loop through the string character by character
    for (size_t i = 0; i < str.size(); ) {
        // Iterate over replacement pairs
        for (const auto& replacement : replacements) {
            size_t targetLength = replacement.first.length();

            // Check if target substring fits within the remaining string length
            if (i + targetLength <= str.size() && str.substr(i, targetLength) == replacement.first) {
                result += replacement.second;  // Append the replacement to result
                i += targetLength;  // Move the position past the replaced substring
                goto character_loop_end;  // Continue to the next character loop
            }
        }

        // If no replacement was made, just append the current character to the result
        result += str[i];
        ++i;

        character_loop_end:;
    }

    return result;  // Return the newly constructed string with replacements
}
