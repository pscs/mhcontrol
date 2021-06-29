#include "mybuffer.h"

MyBuffer::MyBuffer(size_t size_): size(size_) {
    buffer = ps_malloc(size_);
}

MyBuffer::~MyBuffer() {
    free(buffer);
}