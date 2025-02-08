#pragma once

#include <spdlog/fmt/fmt.h>
#include <sstream>

/**
 * Configure a basic templating for a type.
 */
#define ENGINE__FMT_UTILS__HANDLE_FMT_OSTREAM(T, TEMPLATE, ...) template<> struct fmt::formatter<T> { \
     constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.end(); } \
     template <typename FormatContext> \
     auto format(const T& object, FormatContext& ctx) -> decltype(ctx.out()) { \
        return format_to(ctx.out(), TEMPLATE, __VA_ARGS__); \
     }}
