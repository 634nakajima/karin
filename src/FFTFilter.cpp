//
//  FFIFilter.cpp
//  Audio
//
//  Created by Musashi NAKAJIMA on 2017/07/02.
//  Copyright © 2017年 kannolab1. All rights reserved.
//

#include "FFTFilter.hpp"

FFTFilter::FFTFilter()
{
    packetSize = 0;
    n = 1;
    ps2n = 0;
    process = (float *) calloc(FRAMES_PER_BUFFER*2, sizeof process);
}

int FFTFilter::prepareAudioSource(const char *soundL, const char *soundR) {
    Aquila::WaveFile data(soundL);
    packetSize = data.getWaveSize()/data.getBytesPerSample();
    if(packetSize <= 0) {
        printf("cannot find file size\n");
        return 1;
    }
    printf("File size = %ld frames\n", packetSize);
    
    while(pow(2.0, n) < packetSize) n++;
    //n++;
    ps2n = pow(2.0, n);
    auto fft = Aquila::FftFactory::getFft(ps2n);
    spectrum = fft->fft(data.toArray());
    input = (Aquila::SampleType *)calloc(ps2n, sizeof input);
    return 0;
}

void FFTFilter::proc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer) {
    float *intmp = (float *)inputBuffer;
    float *outtmp = (float *)outputBuffer;
    double  inSig[framesPerBuffer], outSig[ps2n];
    for(int i=0; i<framesPerBuffer; i++) {
        inSig[i] = *intmp++;
        *intmp++;
    }
    memmove(input, &input[framesPerBuffer], (ps2n-framesPerBuffer) * sizeof input);
    memcpy(&input[ps2n-framesPerBuffer], inSig, framesPerBuffer * sizeof input);
    Aquila::SignalSource inSigSource(input, ps2n, SAMPLE_RATE);

    auto fft = Aquila::FftFactory::getFft(ps2n);
    Aquila::SpectrumType inSpectrum = fft->fft(inSigSource.toArray());

    std::transform(
                   std::begin(inSpectrum),
                   std::end(inSpectrum),
                   std::begin(spectrum),
                   std::begin(inSpectrum),
                   [] (Aquila::ComplexType x, Aquila::ComplexType y) { return x * y / 5000.0; }
                   );

    fft->ifft(inSpectrum, outSig);
    for (int i=0; i<framesPerBuffer; i++) {
        process[2*i] = (float)outSig[ps2n-framesPerBuffer+i];
        process[2*i+1] = process[2*i];
        outtmp[2*i] = process[2*i];
        outtmp[2*i+1] = process[2*i+1];
    }
    callback(process, outputBuffer, framesPerBuffer);
}

void FFTFilter::resetData() {
    packetSize = 0;
    n = 1;
    ps2n = 0;
}

FFTFilter::~FFTFilter(){
    if (input != NULL) {
        free(input);
    }
    free(process);
}
