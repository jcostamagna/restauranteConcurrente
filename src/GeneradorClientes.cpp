//
// Created by juan on 17/10/16.
//

#include <string>
#include <iostream>
#include <SignalHandler.h>
#include "GeneradorClientes.h"

GeneradorClientes::GeneradorClientes (Pipe& clientes): clientes(clientes){}


void GeneradorClientes::run() {

    // se registra el event handler declarado antes
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );


    this->rutinaGenerador();

    // se recibio la senial SIGINT, el proceso termina
    SignalHandler :: destruir ();
    std::cout << "Escritor: fin del proceso" << std::endl;
    clientes.cerrar();
    exit(0);
}

void GeneradorClientes::rutinaGenerador(){
    std::string dato = "Hola mundo pipes!!";
    int i = 0;

    while ( sigint_handler.getGracefulQuit() == 0 ){
        std::string datoNew = dato + std::string("Numero ") +std::to_string(i);
        clientes.escribir(static_cast<const void *>(datoNew.c_str()), datoNew.size());
        std::cout << "Escritor: escribi el dato [" << datoNew << "] en el pipe" << std::endl;
        i++;
        sleep(1);
        if (i%10==0) i=0;
    }
}