//
// Created by nicolas on 26/10/16.
//

#ifndef RESTAURANTCONCURRENTE_APAGON_HANDLER_H
#define RESTAURANTCONCURRENTE_APAGON_HANDLER_H

#include <assert.h>
#include <csignal>
#include <Restaurante.h>
#include <sstream>
#include "EventHandler.h"

class Apagon_Handler : public EventHandler {

private:
    Restaurante& restaurante;

public:

    Apagon_Handler (Restaurante& resto) : restaurante(resto){
    }

    ~Apagon_Handler () {
    }

    virtual int handleSignal ( int signum ) {
        assert ( signum == SIGCONT );
        restaurante.apagonRestaurante();
        return 0;
    }
};

#endif //RESTAURANTCONCURRENTE_APAGON_HANDLER_H
