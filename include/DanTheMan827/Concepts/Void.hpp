#pragma once

#include <concepts>

namespace DanTheMan827::Concepts {
    template <typename V>
    concept Void = std::same_as<V, void>;
}  // namespace DanTheMan827::Concepts
