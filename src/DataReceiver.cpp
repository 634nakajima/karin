//
//  main.cpp
//  
//
//  Created by Musashi NAKAJIMA on 2017/07/09.
//
//

#include <stdio.h>
#include "SocketManager.h"

class DataReceiver {
public:
    SocketManager socket;
    
    DataReceiver() {
        socket.setCallback(this, &DataReceiver::socketCallback);
    };
    ~DataReceiver(){};
    void socketCallback(int *d, int size){
        std::cout << "Received data: " << d[0] << ", " << d[1] << ", " << d[2] << std::endl;
    };
};

int main(int argc, char **argv) {
    DataReceiver dr;
    while(1);
    return 0;
}
