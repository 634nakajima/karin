//
//  FFIFilter.hpp
//  Audio
//
//  Created by Musashi NAKAJIMA on 2017/07/02.
//  Copyright © 2017年 kannolab1. All rights reserved.
//

#ifndef FFTFilter_h
#define FFTFilter_h

#include <algorithm>
#include <functional>
#include <memory>
#include "aquila/aquila.h"
#include "Audio.h"

class FFTFilter : Audio {
public:
    Aquila::SampleType *input;
    Aquila::SpectrumType spectrum;
    float *process;
    unsigned long packetSize;
    
    FFTFilter();
    ~FFTFilter();
    
    int prepareAudioSource(const char *soundL, const char *soundR);
    void proc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer);
    void resetData();
    
private:
    int n;
    unsigned long ps2n;
};

#endif /* FFTFilter_h */
