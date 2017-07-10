#ifndef __Audio__
#define __Audio__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "portaudio.h"
#include "portsf.h"
#include "global.h"
#include "Exceptions.h"
#include "functions.h"
#include "source.h"
#include "transform.h"
#include "filter.h"
#include "ml.h"
#include "tools.h"

#define PA_SAMPLE_TYPE      paFloat32
#define FRAMES_PER_BUFFER   (1024)
#define Log2N               8u;
#define MAX_PACKET          44100
#define SAMPLE_RATE         44100

//メソッドのポインタをイベントリスナとするためのテンプレートクラス
class ListenerBase {
public:
    virtual ~ListenerBase(){}
    virtual void callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer) = 0;
};

template <class T>
class MemFuncListener : public ListenerBase {
public:
    MemFuncListener(T* obj, void (T::*cbf)(const void *, void *, unsigned long)){
        m_obj = obj;
        m_cbf = cbf;
    };
    virtual void callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer){
        (m_obj->*m_cbf)(inputBuffer, outputBuffer, framesPerBuffer);
    };
private:
    T* m_obj;
    void (T::*m_cbf)(const void *, void *, unsigned long);
};

class Audio {
public:
    //コールバック用
    template <class T>
    void setCallback(T* obj, void (T::*cbf)(const void *, void *, unsigned long)){
        if(m_listener) delete m_listener;
        m_listener = new MemFuncListener<T>(obj, cbf);
    };
    void callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer){
        if(m_listener)  m_listener->callback(inputBuffer, outputBuffer, framesPerBuffer);
    }
    virtual void proc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer) = 0;
private:
    ListenerBase *m_listener;
};
#endif
