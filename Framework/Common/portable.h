#pragma once
#include <cstdint>
#include <climits>
#include <memory>
#include <algorithm>
#include <assert.h>

typedef int32_t four_char_enum;

#if __cplusplus >= 201103L && !defined(__ORBIS__)
#define ENUM(e) enum class e : four_char_enum 
#else
#define ENUM(e) enum e : four_char_enum
#endif

#ifdef ALIGN
#undef ALIGN
#endif

#define ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))

namespace Corona {
    template <typename T>
    T endian_native_unsigned_int(T net_number)
    {
        T result = 0;

        for(size_t i = 0; i < sizeof(net_number); i++) {
            result <<= CHAR_BIT;
            result += ((reinterpret_cast<T*>(&net_number))[i] & UCHAR_MAX);
        }

        return result;
    }

    template <typename T>
    T endian_net_unsigned_int(T native_number)
    {
        T result = 0;

        size_t i = sizeof(native_number);
        do {
            i--;
            (reinterpret_cast<uint8_t*>(&result))[i] = native_number & UCHAR_MAX;
            native_number >>= CHAR_BIT;
        } while (i != 0);

        return result;
    }

    namespace details {
        constexpr int32_t i32(const char* s, int32_t v) {
            return *s ? i32(s+1, v * 256 + *s) : v;
        }

        constexpr uint16_t u16(const char* s, uint16_t v) {
            return *s ? u16(s+1, v * 256 + *s) : v;
        }

        constexpr uint32_t u32(const char* s, uint32_t v) {
            return *s ? u32(s+1, v * 256 + *s) : v;
        }
    }

    constexpr int32_t operator "" _i32(const char* s, size_t) {
        return details::i32(s, 0);
    }

    constexpr uint32_t operator "" _u32(const char* s, size_t) {
        return details::u32(s, 0);
    }

    constexpr uint16_t operator "" _u16(const char* s, size_t) {
        return details::u16(s, 0);
    }
}
