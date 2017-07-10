// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
//
// This code is public domain
// (but note, once linked against the led-matrix library, this is
// covered by the GPL v2)
//
// This is a grab-bag of various demos and not very readable.
#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"
#include "transformer.h"
#include "graphics.h"

#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <algorithm>

using std::min;
using std::max;

#define TERM_ERR  "\033[1;31m"
#define TERM_NORM "\033[0m"

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
  }
  void Run() {
        const int sub_blocks = 16;
        const int width = off_screen_canvas_->width();
        const int height = off_screen_canvas_->height();
        const int x_step = max(1, width / sub_blocks);
        const int y_step = max(1, height / sub_blocks);
        uint8_t count = 0;
        
        while (running() && !interrupt_received) {
            usleep(5 * 1000000);
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                  off_screen_canvas_->SetPixel(x, y, 255, 255, 255);
                }
            }
	    
            off_screen_canvas_ = matrix_->SwapOnVSync(off_screen_canvas_);
        }
  }
private:
  RGBMatrix *const matrix_;
  FrameCanvas *off_screen_canvas_;
};

int main(int argc, char *argv[]) {
  int runtime_seconds = -1;
  const char *demo_parameter = NULL;
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;

  // These are the defaults when no command-line flags are given.
  matrix_options.rows = 32;
  matrix_options.chain_length = 4;
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

  // Now, the image generation runs in the background. We can do arbitrary
  // things here in parallel. In this demo, we're essentially just
  // waiting for one of the conditions to exit.
  if (runtime_seconds > 0) {
    sleep(runtime_seconds);
  } else {
    // The
    printf("Press <CTRL-C> to exit and reset LEDs\n");
    while (!interrupt_received) {
      sleep(1); // Time doesn't really matter. The syscall will be interrupted.
    }
  }

  // Stop image generating thread. The delete triggers
  delete image_gen;
  delete canvas;

  printf("\%s. Exiting.\n",
         interrupt_received ? "Received CTRL-C" : "Timeout reached");
  return 0;
}
