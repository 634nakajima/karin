//
//  Ripple.hpp
//  ColorPattern
//
//  Created by Musashi NAKAJIMA on 2017/07/01.
//
//

#ifndef Ripple_h
#define Ripple_h
#define R_SIZE 35
#define VELOCITY 1.5
#define LAMBDA 10

#include <stdio.h>

class Ripple {
public:
    Ripple();
    Ripple(int x, int y);
    ~Ripple();
    float getDisplacement(int x, int y);
    void start();
    void update();
    int pos_x,pos_y;
    
private:
    float t;
};
#endif /* Ripple_h */
