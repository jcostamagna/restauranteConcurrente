//
// Created by ale on 16/10/16.
//

#ifndef RESTAURANTCONCURRENTE_FORKEABLE_H
#define RESTAURANTCONCURRENTE_FORKEABLE_H

#include <unistd.h>

class Forkeable {
public:
    Forkeable();

    virtual ~Forkeable();
    void start();
    void stop();
    pid_t get_pid() {return pid;}

protected:
    pid_t pid;
    virtual void run();
};


#endif //RESTAURANTCONCURRENTE_FORKEABLE_H
