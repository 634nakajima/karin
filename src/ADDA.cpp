#include "ADDA.h"

void ADDA::proc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer) {
  
}

int ADDA::ioCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData){
    ADDA *adda = (ADDA *)userData;
    float *input = (float *)inputBuffer;
    float *output = (float *)outputBuffer;
    float *process = (float *)adda->process;

    unsigned int i;

    //inputをprocessしてoutputにコピー
    //processをinput、outputをoutputにしてコールバック
    
    if( inputBuffer == NULL ){
        for( i=0; i<framesPerBuffer; i++ ){
            process[2*i] = 0;
            process[2*i+1] = 0;
        }
    }else{
        for( i=0; i<framesPerBuffer; i++ ){
            process[2*i]   = *input;
            process[2*i+1]   = *input++;
        }
    }
    for( i=0; i<framesPerBuffer; i++ ){
        *output++ = *process;
        *output++ = *process++;
    }
    
    adda->callback(adda->process, outputBuffer, framesPerBuffer);
    return paContinue;
}

ADDA::ADDA(){
    initADDA();
    if(preparePa()) printf("err_preparePa\n");
}

void ADDA::initADDA(){
    isPlaying   = false;
    process     = (float *)calloc(sizeof(float), MAX_PACKET);
}

int ADDA::preparePa(){
    err = Pa_Initialize();
    if( err != paNoError ) {
        Pa_Terminate();
        return 1;
    }
    
    outputParameters.device = Pa_GetDefaultOutputDevice(); // デフォルトアウトプットデバイス
    if (outputParameters.device == paNoDevice) {
        printf("Error: No default output device.\n");
        Pa_Terminate();
        return 1;
    }
    
    inputParameters.device = Pa_GetDefaultInputDevice(); //デフォルトインプットデバイス
	  printf("Default InputDevice: %d\n", inputParameters.device);
    if (inputParameters.device == paNoDevice) {
        printf("Error: No default input device.\n");
        Pa_Terminate();
        return 1;
    }
    inputParameters.channelCount = 1;       //モノラルインプット
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    
    outputParameters.channelCount = 2;       //ステレオアウトプット
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    err = Pa_OpenStream(&paStream,
                        &inputParameters,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0, //paClipOff,    we won't output out of range samples so don't bother clipping them
                        ioCallback,
                        this);
    
    if( err != paNoError ) {
        Pa_Terminate();
        printf("err_open\n");
    }
    start();
    return 0;
}

int ADDA::start(){
    err = Pa_StartStream( paStream );
    isPlaying = true;
    return 0;
}

int ADDA::stop(){
    err = Pa_StopStream( paStream );
    isPlaying = false;
    return 0;
}

void ADDA::finishPa(){
	  free(process);
    stop();
    Pa_CloseStream( paStream );
    Pa_Terminate();
}

ADDA::~ADDA(){
    finishPa();
}
