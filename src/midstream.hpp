#ifndef MIDSTREAM_HPP
#define MIDSTREAM_HPP
#include <libdragon.h>

struct MixEvent
{
    wav64_t *sample;
    float freq;
    float volume;
    int channel;
    bool start;
};

struct MixStream
{
    float delta_time;
    MixEvent *events;
    size_t num_events;
};

#endif
