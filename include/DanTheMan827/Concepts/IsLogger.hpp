#pragma once

#include <cstddef>
#include <type_traits>

namespace DanTheMan827::Concepts {
    template <typename L>
    concept Logger = std::is_same<L, void>::value || requires(L l) {
        l.info("");
        l.debug("");
        l.error("");
        l.warn("");
        l.critical("");
    };
}  // namespace DanTheMan827::Concepts
