#include <cassert>

#include "ring_buffer.h"

void TestRingBuffer() {
    dux::RingBuffer<int, 3> buffer;
    buffer.SaveToBuffer(1);
    buffer.SaveToBuffer(3);
    buffer.SaveToBuffer(6);
    
    auto const& raw = buffer.GetRawBuffer();
    assert(raw[0] == 1);
    assert(raw[1] == 3);
    assert(raw[2] == 6);

    buffer.SaveToBuffer(11);
    assert(raw[0] == 11);
}