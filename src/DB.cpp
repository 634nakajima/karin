#include "DB.h"

DB::DB() {
  input = (float *)calloc(SAMPLE_RATE*2, sizeof(float));
}

void DB::proc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer) {
    float *intmp = (float *)inputBuffer;
    pushInput(intmp, framesPerBuffer*2);
    if(cnt++%100 == 0) printf("DB: %f dB\n", calcdB());
    callback(inputBuffer, outputBuffer, framesPerBuffer);
}

void DB::pushInput(float *inData, unsigned long size) {
    if(size > SAMPLE_RATE*2) {printf("too large size\n"); return;}
    memmove(input, &input[size], (SAMPLE_RATE*2-size) * sizeof(float));
    memcpy(&input[SAMPLE_RATE*2-size], inData, size * sizeof(float));
}

float DB::calcRMS() {
  rms = 0.0;
  for(int i=0; i<SAMPLE_RATE; i++) {
    rms += input[2*i]*input[2*i];
  }
  return rms;
}

float DB::calcdB() {
  dB = 10*log10f(calcRMS());
  return dB;
}

DB::~DB() {
  free(input);
}