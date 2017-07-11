//
//  Ripple.cpp
//  ColorPattern
//
//  Created by Musashi NAKAJIMA on 2017/07/01.
//
//

#include "Ripple.h"
#include <math.h>

Ripple::Ripple(){
    pos_x = 0;
    pos_y = 0;
    t = R_SIZE/VELOCITY+1;
}

Ripple::Ripple(int x, int y){
    pos_x = x;
    pos_y = y;
    t = R_SIZE/VELOCITY+1;
}

Ripple::~Ripple(){
    
}

void Ripple::start() {
    t = 0;
}

float Ripple::getDisplacement(int x, int y){
    float dist = pow((pos_x-x)*(pos_x-x)+(pos_y-y)*(pos_y-y), 0.5);
    if(VELOCITY*t >= R_SIZE) return 0;
    else if(dist >= VELOCITY*t) return 0;
    else {
        return (R_SIZE/VELOCITY-t)/(5*(dist+1))*(1-cos(2*M_PI*VELOCITY*t/LAMBDA - 2*M_PI*dist/LAMBDA));
    }
}

void Ripple::update(){
    if(VELOCITY*t < R_SIZE)
        t++;
}
