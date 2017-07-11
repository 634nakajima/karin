// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
//
// This code is public domain
// (but note, once linked against the led-matrix library, this is
// covered by the GPL v2)
//
// This is a grab-bag of various demos and not very readable.
#include "led-matrix/led-matrix.h"
#include "led-matrix/threaded-canvas-manipulator.h"
#include "led-matrix/transformer.h"
#include "led-matrix/graphics.h"
#include "Ripple.h"
#include "SocketManager.h"

#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <random>

using std::min;
using std::max;

#define TERM_ERR  "\033[1;31m"
#define TERM_NORM "\033[0m"
#define NUM_RIPPLES 20

using namespace rgb_matrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

/*
 * The following are demo image generators. They all use the utility
 * class ThreadedCanvasManipulator to generate new frames.
 */

// Simple generator that pulses through RGB and White.
class ColorPulseGenerator : public ThreadedCanvasManipulator {
public:
    ColorPulseGenerator(RGBMatrix *m) : ThreadedCanvasManipulator(m), matrix_(m) {
        off_screen_canvas_ = m->CreateFrameCanvas();
        nextRipple = 0;
        width = 96;
        height = 96;
        for(int i=0;i<3;i++) {
          current_bg[i] = 0;
          current_ripple_color[i] = 0;
        }
    }
    
  void Run() {
        while (running() && !interrupt_received) {
            usleep(10 * 1000);
            off_screen_canvas_->Fill(0, 0, 0);
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    float a=0;
                    for(int z=0;z<NUM_RIPPLES;z++) {
                        a += ripple[z].getDisplacement(x,y);
                        if(a > 1) a = 1;
                        }

                    int xt = 0;
                    int yt = 0;
                    transform(x, y, &xt, &yt);
                    off_screen_canvas_->SetPixel(xt, yt,
                                                 a*current_ripple_color[0]+(1-a)*current_bg[0],
                                                 a*current_ripple_color[1]+(1-a)*current_bg[1],
                                                 a*current_ripple_color[2]+(1-a)*current_bg[2]);
                }
            }
            off_screen_canvas_ = matrix_->SwapOnVSync(off_screen_canvas_);
            updateColor();
            for(int i=0;i<NUM_RIPPLES;i++) ripple[i].update();
        }
    }
    
    void transform(int x, int y, int *x_t, int *y_t) {
        if(0<=x&&x<32&&32<=y&&y<96) {*x_t = 287-y; *y_t = x;}
        else if(32<=x&&x<96&&0<=y&&y<32) {*x_t = 95-x; *y_t = 31-y;}
        else if(32<=x&&x<96&&32<=y&&y<64) {*x_t = 159-x;*y_t = 63-y;}
        else if(32<=x&&x<96&&64<=y&&y<96) {*x_t = 223-x;*y_t = 95-y;}
        //else *x_t = width; *y_t = height;
    }
    
    void newRipple(){
        ripple[nextRipple%NUM_RIPPLES] = Ripple(rnd()%(width-5),rnd()%(height-5));
        ripple[(nextRipple++)%NUM_RIPPLES].start();
    }
  
  void updateColor() {
    for(int i=0;i<3;i++) {
      current_bg[i] = current_bg[i]*0.8 + bg[i]*0.2;
      current_ripple_color[i] = current_ripple_color[i]*0.8 + ripple_color[i]*0.2;
    }
  }
  
    void setColor(int num) {
        switch(num) {
            case 0:
                //晴れ朝
                bg[0] = 110;
                bg[1] = 200;
                bg[2] = 250;
                ripple_color[0] = 230;
                ripple_color[1] = 230;
                ripple_color[2] = 100;
                break;
            case 1:
                //晴れ昼
                bg[0] = 0;
                bg[1] = 190;
                bg[2] = 255;
                ripple_color[0] = 250;
                ripple_color[1] = 230;
                ripple_color[2] = 200;
                break;
            case 2:
                //晴れ夜
                bg[0] = 80;
                bg[1] = 100;
                bg[2] = 200;
                ripple_color[0] = 210;
                ripple_color[1] = 220;
                ripple_color[2] = 100;
                break;
            case 3:
                //曇り朝
                bg[0] = 110;
                bg[1] = 200;
                bg[2] = 250;
                ripple_color[0] = 230;
                ripple_color[1] = 230;
                ripple_color[2] = 190;
                break;
            case 4:
                //曇り昼
                bg[0] = 0;
                bg[1] = 190;
                bg[2] = 255;
                ripple_color[0] = 250;
                ripple_color[1] = 230;
                ripple_color[2] = 200;
                break;
            case 5:
                //曇り夜
                bg[0] = 80;
                bg[1] = 100;
                bg[2] = 200;
                ripple_color[0] = 210;
                ripple_color[1] = 220;
                ripple_color[2] = 100;
            break;
            case 6:
                //雨朝
                bg[0] = 110;
                bg[1] = 200;
                bg[2] = 250;
                ripple_color[0] = 230;
                ripple_color[1] = 230;
                ripple_color[2] = 190;
                break;
            case 7:
                //雨昼
                bg[0] = 0;
                bg[1] = 190;
                bg[2] = 255;
                ripple_color[0] = 250;
                ripple_color[1] = 230;
                ripple_color[2] = 200;
                break;
            case 8:
                //雨夜
                bg[0] = 80;
                bg[1] = 100;
                bg[2] = 200;
                ripple_color[0] = 210;
                ripple_color[1] = 220;
                ripple_color[2] = 100;
                break;
        }
    }

  //Socket Comm
  void socketCallback(int *d, int size){
    std::cout << "Received data: " << d[0] << ", " << d[1] << ", " << d[2] << std::endl;
    switch(d[0]){
    case 0:
      setColor(3*d[1]+d[2]);
      break;
    case 1:
      newRipple();
      break;
    }

  };
  
private:
    RGBMatrix *const matrix_;
    FrameCanvas *off_screen_canvas_;
    Ripple ripple[NUM_RIPPLES];
    int current_bg[3], current_ripple_color[3], bg[3], ripple_color[3], width, height, nextRipple;
    std::random_device rnd;
};

int main(int argc, char *argv[]) {
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;
    
    // These are the defaults when no command-line flags are given.

    matrix_options.rows = 32;
    matrix_options.chain_length = 8;
    matrix_options.parallel = 1;
    ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt);
    RGBMatrix *matrix = CreateMatrixFromOptions(matrix_options, runtime_opt);
    if (matrix == NULL)	return 1;
    printf("Size: %dx%d. Hardware gpio mapping: %s\n", matrix->width(), matrix->height(), matrix_options.hardware_mapping);
    
    Canvas *canvas = matrix;
    
    // The ThreadedCanvasManipulator objects are filling
    // the matrix continuously.
    ThreadedCanvasManipulator *image_gen = NULL;
    image_gen = new ColorPulseGenerator(matrix);
    
    // Set up an interrupt handler to be able to stop animations while they go
    // on. Note, each demo tests for while (running() && !interrupt_received) {},
    // so they exit as soon as they get a signal.
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);
    
    // Image generating demo is crated. Now start the thread.
    image_gen->Start();
    ColorPulseGenerator *ripple = (ColorPulseGenerator *)image_gen;
    SocketManager sockm;
    
    ripple->setColor(1);
    sockm.setCallback(ripple, &ColorPulseGenerator::socketCallback);
    // Now, the image generation runs in the background. We can do arbitrary
    // things here in parallel. In this demo, we're essentially just
    // waiting for one of the conditions to exit.
    printf("Press <CTRL-C> to exit and reset LEDs\n");
    while (!interrupt_received) {
      ripple->newRipple();
        sleep(1); // Time doesn't really matter. The syscall will be interrupted.
    }
    
    // Stop image generating thread. The delete triggers
    delete image_gen;
    delete canvas;
    
    printf("\%s. Exiting.\n",
           interrupt_received ? "Received CTRL-C" : "Timeout reached");
    return 0;
}
