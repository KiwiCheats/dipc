#include <dipc/dipc.hpp>

#include <iostream>
#include <Windows.h>

#define DIPC_SHARED_MEMORY_BUFFER_SIZE 0x3200014u

namespace dipc {
    bool connect(uint32_t pid, connection& connection) {
        const char format[] = "DiscordOverlay_Framebuffer_Memory_%d";

        int32_t size = snprintf(nullptr, 0, format, pid) + 1;

        if (size == -1)
            return false;

        std::string object;

        object.resize(size);

        snprintf((char*) object.data(), size, format, pid);

        HANDLE handle = OpenFileMappingA(
            FILE_MAP_READ | FILE_MAP_WRITE,
            FALSE,
            object.data()
        );

        do {
            if (handle == nullptr)
                break;

            auto mapping = (uint8_t*) MapViewOfFile(
                handle,
                FILE_MAP_READ | FILE_MAP_WRITE,
                0,
                0,
                DIPC_SHARED_MEMORY_BUFFER_SIZE
            );

            if (mapping == nullptr)
                break;

            connection.pid = pid;
            connection.handle = handle;
            connection.mapping = mapping;

            return true;
        } while (false);

        if (handle != nullptr)
            CloseHandle(handle);

        return false;
    }

    void disconnect(const connection& connection) {
        UnmapViewOfFile(connection.mapping);
        CloseHandle(connection.handle);
    }

    // RGBA only
    void commit_frame(uint8_t* mapping, const uint8_t* frame, size_t width, size_t height) {
        auto header = (dipc_header*) mapping;

        const size_t size = DIPC_FRAMEBUFFER_SIZE(width, height);

        memcpy(mapping + sizeof(dipc_header), frame, size);

        header->width = width;
        header->height = height;

        ++header->frame_time;
    }
}