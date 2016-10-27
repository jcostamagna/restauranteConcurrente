//
// Created by juan on 17/10/16.
//

#include <string>
#include <iostream>
#include <SignalHandler.h>
#include <iomanip>
#include <sstream>
#include "GeneradorClientes.h"
#include "Log.h"

GeneradorClientes::GeneradorClientes (Pipe& clientes, int cantClientes): cantClientes(cantClientes), puerta(clientes){}


void GeneradorClientes::run() {

    // se registra el event handler declarado antes
    //SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );


    this->rutinaGenerador();

    // se recibio la senial SIGINT, el proceso termina
    SignalHandler :: destruir ();
    std::cout << "Generador: fin del proceso" << std::endl;
    puerta.cerrar();
    exit(0);
}

void GeneradorClientes::rutinaGenerador() {
    int i = 0;

    while ( sigint_handler.getGracefulQuit() == 0 ){
        std::ostringstream ss;
        ss << std::setfill('0') << std::setw(PID_LENGHT) << i;
        ss << std::setfill('0') << std::setw(BUFFSIZE-PID_LENGHT) << 0;
        std::string dato(ss.str());

        ss.str("");
        ss << "Generador: escribo el cliente [" << dato << "] en el pipe puerta" << std::endl;
        Log::getInstance()->log(ss.str());
        puerta.escribir(static_cast<const void *>(dato.c_str()), dato.size());


        std::cout << "Generador: escribi el cliente [" << dato << "] en el pipe" << std::endl;


        i++;
        unsigned int sleeep = 500000;
        usleep(sleeep);

        if (i>= cantClientes) break;

        if (apagon_handler_procesos.getApagon() == 1) {
            std::stringstream ss;
            ss.str("");
            ss << "APAGON GENERADOR CLIENTES"<< std::endl;
            Log::getInstance()->log(ss.str());
            std::cout << "APAGON GENERADOR CLIENTES"<< std::endl;
            sleep(TIEMPO_APAGON);
            apagon_handler_procesos.stopApagon();
        }
    }
}