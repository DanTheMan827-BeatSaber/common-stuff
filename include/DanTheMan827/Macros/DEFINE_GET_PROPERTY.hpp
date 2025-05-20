/// @brief Macro to define a getter property for a static class member variable
#define DEFINE_GET_PROPERTY(name, type, body)                            \
   private:                                                              \
    static inline type __GETTER_##name() body                            \
   public:                                                               \
    static inline __declspec(property(get = __GETTER_##name)) type name;
