//
// Created by ale on 16/10/16.
//

#ifndef RESTAURANTCONCURRENTE_FORKEABLE_H
#define RESTAURANTCONCURRENTE_FORKEABLE_H

#include <unistd.h>

class Forkeable {
public:

    virtual ~Forkeable();
    virtual void run();
    void start();
    void stop();

private:
    static void* starter(void* datos);

protected:
    pid_t pid;
};


#endif //RESTAURANTCONCURRENTE_FORKEABLE_H
