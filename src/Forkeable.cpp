//
// Created by ale on 16/10/16.
//

#include <wait.h>
#include <SignalHandler.h>
#include <cstdlib>
#include "Forkeable.h"

Forkeable::~Forkeable() {}

void Forkeable::start() {
    this->pid = fork (); //guardo el pid del padre
    if (this->pid == 0){

        // se registra el event handler declarado antes
        SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

        run();

        // se recibio la senial SIGINT, se sale del while y sigue aca,  el proceso termina
        SignalHandler::destruir();

        exit(0);
    }
}


void Forkeable::stop() {
    waitpid ( this->pid , NULL, 0); //espero a que run() termine
}

void Forkeable::run() {}

Forkeable::Forkeable() : pid(0) {}
