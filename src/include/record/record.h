#pragma once
#include <iostream> // Functionality: COUT
#include "portaudio.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>  //Functionality: Sleep
#include <thread>   //Functionality: Sleep
#include <algorithm> //Functionality: fill_n
#include "AudioFile.h"
#define SAMPLE_RATE (44100)

typedef float SAMPLE;

#define NUM_SECONDS 10
#define NUM_CHANNELS  1
#define SAMPLE_SILENCE 0.0f
#define PA_SAMPLE_TYPE  paFloat32
#define FRAMES_PER_BUFFER (512)
#define TRUE (1==1)
#define FALSE (!TRUE)
#define WRITE_TO_FILE   TRUE


typedef struct
{
    int     frameIndex;
    int     maxFrameindex;
    SAMPLE  *recordedSamples;
}
paTestData;

class record {
public:
    record();
    void start_record();
    AudioFile<SAMPLE>           file;

private:
    PaStreamParameters  inputParameters,
                        outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;
    SAMPLE              max, val;
    double              average;
    int recordCallback(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags, void *userData);

    static int recordCallbackSub(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags, void *userData)
    {
        auto pThis = reinterpret_cast<record*>(userData);  // get back the this pointer.
        return pThis->recordCallback( inputBuffer, outputBuffer,framesPerBuffer, timeInfo,statusFlags, nullptr);

    }
};
