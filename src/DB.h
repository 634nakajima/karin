#ifndef DB_hpp
#define DB_hpp

#include <stdio.h>
#include "Audio.h"

class DB : public Audio
{
public:
    DB();
    ~DB();
    void proc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer);
    void pushInput(float *inData, unsigned long size);
    float calcRMS();
    float calcdB();
private:
    float *input, rms, dB;
    int cnt = 0;
};
#endif /* DB.h */
