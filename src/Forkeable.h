//
// Created by ale on 16/10/16.
//

#ifndef RESTAURANTCONCURRENTE_FORKEABLE_H
#define RESTAURANTCONCURRENTE_FORKEABLE_H

#include <unistd.h>
#include <SIGINT_Handler.h>
#define PID_LENGHT 10

class Forkeable {
public:
    Forkeable();

    virtual ~Forkeable();
    void start();
    void stop();
    pid_t get_pid() {return pid;}

protected:
    pid_t pid;
    SIGINT_Handler sigint_handler;
    virtual void run();
};


#endif //RESTAURANTCONCURRENTE_FORKEABLE_H
