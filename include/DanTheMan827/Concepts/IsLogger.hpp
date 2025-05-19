#pragma once

namespace DanTheMan827::Concepts {
    template <typename L>
    concept Logger = requires(L& l) {
        l.info("");
        l.debug("");
        l.error("");
        l.warn("");
        l.critical("");
    };
}  // namespace DanTheMan827::Concepts
