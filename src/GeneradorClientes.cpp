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

GeneradorClientes::GeneradorClientes (Pipe& clientes): cantClientes(15), clientes(clientes){}


void GeneradorClientes::run() {

    // se registra el event handler declarado antes
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );


    this->rutinaGenerador();

    // se recibio la senial SIGINT, el proceso termina
    SignalHandler :: destruir ();
    std::cout << "Generador: fin del proceso" << std::endl;
    clientes.cerrar();
    exit(0);
}

void GeneradorClientes::rutinaGenerador(){
    //std::string dato = "Hola mundo pipes!!";
    int i = 0;

    while ( sigint_handler.getGracefulQuit() == 0 ){
        std::ostringstream ss;
        ss << std::setfill('0') << std::setw(8) << i;
        std::string dato(ss.str());
        clientes.escribir(static_cast<const void *>(dato.c_str()), dato.size());

        ss.str("");
        ss << "Generador: escribi el cliente [" << dato << "] en el pipe" << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "Generador: escribi el cliente [" << dato << "] en el pipe" << std::endl;


        i++;
        sleep(1);

        if (i>= cantClientes) break;
        //Reinicio contador
        //i = i % 100000000;//if (i == 99999999) i = 0;
    }
}