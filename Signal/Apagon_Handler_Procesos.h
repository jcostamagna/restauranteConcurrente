//
// Created by nicolas on 26/10/16.
//

#ifndef RESTAURANTCONCURRENTE_APAGON_HANDLER_PROCESOS_H
#define RESTAURANTCONCURRENTE_APAGON_HANDLER_PROCESOS_H
#include <assert.h>
#include <csignal>
#include <sstream>
#include "EventHandler.h"

class Apagon_Handler_Procesos : public EventHandler {

private:
    sig_atomic_t apagon;

public:

    Apagon_Handler_Procesos () : apagon(0){
    }

    ~Apagon_Handler_Procesos () {
    }

    virtual int handleSignal ( int signum ) {
        assert ( signum == SIGCONT );
        apagon = 1;
        return 0;
    }

    sig_atomic_t getApagon () const {
        return this->apagon;
    }
};

#endif //RESTAURANTCONCURRENTE_APAGON_HANDLER_PROCESOS_H
