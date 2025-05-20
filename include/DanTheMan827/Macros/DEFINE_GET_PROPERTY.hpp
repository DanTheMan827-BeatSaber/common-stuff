/// @brief Macro to define a getter property for a static class member variable
#define DEFINE_GET_PROPERTY(name, type)                                  \
   public:                                                               \
    static inline __declspec(property(get = __GETTER_##name)) type name; \
                                                                         \
   private:                                                              \
    template <typename T = void>                                         \
    requires(std::is_void<T>::value)                                     \
    static inline type __GETTER_##name()
