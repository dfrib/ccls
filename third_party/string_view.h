#if __cplusplus >= 201703L || defined(__has_include) && __has_include(<string_view>)
#include <string_view>
#else
#define STX_NAMESPACE_NAME std
#include "string_view.hpp"
#endif