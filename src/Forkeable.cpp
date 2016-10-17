//
// Created by ale on 16/10/16.
//

#include <wait.h>
#include "Forkeable.h"

Forkeable::~Forkeable() {}

void Forkeable::start() {
    this->pid = fork (); //guardo el pid del padre
    if (this->pid == 0){
        run();
    }
}


void Forkeable::stop() {
    waitpid ( this->pid , NULL, 0); //espero a que run() termine
}

void Forkeable::run() {}