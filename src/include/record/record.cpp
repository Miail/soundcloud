#include "record.h"


record::record()
{
    std::cout << "Record object made" << std::endl;
    std::cout << "Portaudio Version: " << Pa_GetVersion() << std::endl;
    this->data.maxFrameindex = this->totalFrames = NUM_SECONDS * SAMPLE_RATE;
    this->data.frameIndex = 0;
    this->numSamples = this->totalFrames * NUM_CHANNELS;
    this->data.recordedSamples = new SAMPLE[this->numSamples]; /* From now on, recordedSamples is initialised. */
    if( this->data.recordedSamples == NULL )
    {
        std::cout << "Could not allocate record array" << std::endl;
        exit(1);
    }

    for(int i=0; i<numSamples; i++ )
    {
        this->data.recordedSamples[i] = 0;
    }

    int err = Pa_Initialize();

    if( err == paNoError )
    {
        std::cout << "No error in init" << std::endl;
        std::cout << "PortAudio init: "<< Pa_GetErrorText( err ) << std::endl;
    }
    else
    {
       printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
       exit(1);
    }

    this->inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */


    if (this->inputParameters.device == paNoDevice) {
        std::cout << "Error: No default input device" << std::endl;
        exit(1);
    }

    this->inputParameters.channelCount = 1;                    /* stereo input */
    this->inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    this->inputParameters.suggestedLatency = Pa_GetDeviceInfo( this->inputParameters.device )->defaultLowInputLatency;
    this->inputParameters.hostApiSpecificStreamInfo = NULL;

    std::cout << "Device name: "  <<Pa_GetDeviceInfo(this->inputParameters.device)->name << std::endl;
    std::cout << "Max inputChannels: "  <<Pa_GetDeviceInfo(this->inputParameters.device)->maxInputChannels << std::endl;

}

int record::recordCallback(const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags, void *userData)
{
    //std::cout << "Callback called" << std::endl;
    //this->data = (paTestData&) userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &this->data.recordedSamples[this->data.frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = this->data.maxFrameindex - this->data.frameIndex;

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    //(void) userData;

    if( framesLeft < framesPerBuffer )
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( inputBuffer == NULL )
    {
        for(int i=0; i<framesToCalc; i++ )
        {
            *wptr++ = SAMPLE_SILENCE;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
        }
    }
    else
    {
        for(int i=0; i<framesToCalc; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
    }
    this->data.frameIndex += framesToCalc;
    //std::cout << "out of callback" << std::endl;
    return finished;
}

void record::start_record()
{

    err = Pa_OpenStream(
              &this->stream,
              &this->inputParameters,
              NULL,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paFramesPerBufferUnspecified,      /* we won't output out of range samples so don't bother clipping them */
              &record::recordCallbackSub,
              this );
    if( err != paNoError )
    {
        std::cout << "Something wrong  - open_stream check" << std::endl;
        std::cout << "PortAudio error: "<< Pa_GetErrorText( err ) << std::endl;
        exit(1);
    }

    this->err = Pa_StartStream( this->stream );

    if( err != paNoError )
    {
        std::cout << "Something wrong in stream check" << std::endl;
        std::cout << "PortAudio error: "<< Pa_GetErrorText( err ) << std::endl;
        exit(1);
    }

    std::cout << "Waiting for playback to finish" << std::endl;

    while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
    {
        Pa_Sleep(1000);

        //std::cout << Pa_GetLastHostErrorInfo()->errorCode << std::endl;

        //if( err != paNoError )
        //{
        //    std::cout << "error check with isStreamActive - something wrong" << std::endl;
        //    std::cout << "PortAudio error: "<< Pa_GetErrorText( err ) << std::endl;
        //    std::cout << Pa_GetLastHostErrorInfo()->errorText << std::endl;

        //    exit(1);
        //}

       std::cout << this->data.frameIndex  << std::endl;

    }
    if( err != paNoError )
    {
        std::cout << "error check with isStreamActive - something wrong" << std::endl;
        std::cout << "PortAudio error: "<< Pa_GetErrorText( err ) << std::endl;
        exit(1);
    }

    err = Pa_CloseStream( stream );
    if( err != paNoError )
    {
        std::cout << "error check with close_stream- something wrong" << std::endl;
        std::cout << "PortAudio error: "<< Pa_GetErrorText( err ) << std::endl;
        exit(1);
    }

    std::cout << "Number of entries: " <<  sizeof(this->data.recordedSamples)/sizeof(this->data.recordedSamples[0]) << std::endl;

    /* Measure maximum peak amplitude. */
    max = 0;
    average = 0.0;
    for(int i=0; i<numSamples; i++ )
    {

        val = this->data.recordedSamples[i];
        //std::cout << "i: " << i << " : "<< val << std::endl;
        if( val < 0 ) val = -val; /* ABS */
        if( val > max )
        {
            max = val;
        }
        average += val;
    }

    average = average / (double)numSamples;

    std::cout<<"sample max amplitude = " << max << std::endl;
    std::cout<<"sample average = " <<  average << std::endl;

    if (WRITE_TO_FILE)
    {
        this->file.setNumChannels(NUM_CHANNELS);
        this->file.setAudioBufferSize(NUM_CHANNELS,numSamples);
        this->file.setSampleRate(SAMPLE_RATE);
        //this->file.setBitDepth(32);
        this->file.setNumSamplesPerChannel(numSamples);
        for(int i = 0; i < numSamples; i++)
        {
            this->file.samples[0][i] = this->data.recordedSamples[i];
        }
        std::cout << "Stored" << std::endl;
        this->file.save("recorded.wav");
    }



    std::cout << "Everythin done!" << std::endl;



}
