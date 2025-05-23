#pragma once

#include <dlfcn.h>

#include "../../Concepts.hpp"

namespace DanTheMan827::Helpers::Pointers {
    template <Concepts::Logger L = void>
    inline void* getSymbolAddress(char const* library_name, char const* symbol_name, L* Logger = nullptr) {
        auto library = dlopen(library_name, RTLD_NOW);
        if (!library) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("Failed to load library: {}", library_name);
            }
            return nullptr;
        }
        auto symbol_address = dlsym(library, symbol_name);
        if (!symbol_address) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("Failed to find symbol '{}' in library: {}", symbol_name, library_name);
            }
        }
        return symbol_address;
    }
}  // namespace DanTheMan827::Helpers::Pointers
