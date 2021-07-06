#pragma once

#include <cstdint>

class be_uint16_t {
public:
    operator uint16_t() const {
        return static_cast<uint16_t>(data[0]) << 8 | data[1];
    }
private:
    uint8_t data[2];
} __attribute__ ((packed));

static_assert(sizeof(be_uint16_t) == 2, "be_uint16_t size is not correct");

class be_uint32_t {
public:
    operator uint32_t() const {
        return static_cast<uint32_t>(data[0]) << 24 | static_cast<uint32_t>(data[1]) << 16 | static_cast<uint32_t>(data[2]) << 8 | data[3];
    }
private:
    uint8_t data[4];
} __attribute__ ((packed));

static_assert(sizeof(be_uint32_t) == 4, "be_uint32_t size is not correct");

class be_int16_t {
private:
    uint8_t data[2];
} __attribute__ ((packed));

static_assert(sizeof(be_int16_t) == 2, "be_int16_t size is not correct");

class be_int32_t {
public:
    operator int32_t() const {
        return static_cast<int32_t>(data[0]) << 24 | static_cast<int32_t>(data[1]) << 16 | static_cast<int32_t>(data[2]) << 8 | data[3];
    }
private:
    uint8_t data[4];
} __attribute__ ((packed));

static_assert(sizeof(be_int32_t) == 4, "be_int32_t size is not correct");
