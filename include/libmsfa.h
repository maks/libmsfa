#ifndef __LIBMSFA_H
#define __LIBMSFA_H

#include <stdint.h>

typedef enum
{
    MSFA_SUCCESS  =  0,
    MSFA_ERROR    = -1,  /* A generic error. */
    MSFA_AUDIO_OPEN_FAILED = -2,
    MSFA_AUDIO_START_FAILED = -2,
} msfa_result;

extern "C" msfa_result initEngine();

extern "C" void sendMidi(const uint8_t *bytes, int size);

extern "C" void shutdownEngine();

#endif  // __LIBMSFA_H