#pragma once

#include "scotland2/shared/loader.hpp"

#if __has_include("git-info.h")
#include "git-info.h"
#endif

#if defined(GIT_TAG) && defined(GIT_EXACT_TAG) && defined(GIT_COMMIT) && defined(GIT_MODIFIED) 
// Use the git info if available
// -----------------------------

    /// @brief Stores the ID and version of our mod, and is sent to the modloader upon startup
    inline const modloader::ModInfo modInfo{MOD_ID, []() {
        std::string output;

        // Check if GIT_TAG is empty
        if (GIT_TAG[0] == '\0') {
            output = VERSION;
        } else {
            // Skip the first character of GIT_TAG and find the first occurrence of '+' or '-'
            std::string tag(&GIT_TAG[1]);
            size_t pos = tag.find_first_of("+-");
            if (pos != std::string::npos) {
                output = tag.substr(0, pos);
            } else {
                output = tag;
            }
        }

        // Append commit if GIT_EXACT_TAG is empty
        if (GIT_EXACT_TAG[0] == '\0') {
            output += "-";
            output += GIT_COMMIT;
        }

        // Append '-dirty' if GIT_MODIFIED is 1
        if (GIT_MODIFIED) {
            output += "-dirty";
        }

        // Return the c_str with strdup
        return strdup(output.c_str());
    }(), 0};
#else
// Fallback to using the version defined by the mod
// ------------------------------------------------

    /// @brief Stores the ID and version of our mod, and is sent to the modloader upon startup
    inline const modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
#endif
