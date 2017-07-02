#ifndef __ADDA__
#define __ADDA__

#include "Audio.h"

class ADDA : public Audio
{
public:
    PaStreamParameters inputParameters, outputParameters;
    PaStream *paStream;
    PaError err;
    bool        isPlaying;
    float       *process;

    ADDA();
    ~ADDA();
    int start();
    int stop();
    virtual void proc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer);

private:
    void initADDA();
    int preparePa();
    void finishPa();

    static int ioCallback(const void            *inputBuffer,
                          void                  *outputBuffer,
                          unsigned long         framesPerBuffer,
                          const                 PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void                  *userData);
    
};
#endif /* defined(__ADDA__) */
