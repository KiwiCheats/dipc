#ifndef _DIPC_DIPC_HPP_
#define _DIPC_DIPC_HPP_

#include <cstdint>

#ifdef __GNUC__
#define DIPC_DISABLE_PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define DIPC_DISABLE_PACK(__Declaration__) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#define DIPC_FRAMEBUFFER_SIZE(width, height) (4 * width * height)

DIPC_DISABLE_PACK(struct dipc_header {
    uint32_t unk;
    uint64_t frame_time;
    uint32_t width;
    uint32_t height;
});

namespace dipc {
    struct connection {
        uint32_t pid;
        void* handle;
        uint8_t* mapping;
    };

    bool connect(uint32_t pid, connection& connection);
    void disconnect(const connection& connection);

    void commit_frame(uint8_t* mapping, const uint8_t* frame, size_t width, size_t height);
}

#endif