//
// Created by ale on 27/10/16.
//

#include <SignalHandler.h>
#include <iostream>
#include "LimpiadorMesas.h"
#include "Log.h"

LimpiadorMesas::LimpiadorMesas(Pipe &pedidos, LockFd &lockLecturaMesas) :
        pedidos(pedidos), lockLecturaMesas(lockLecturaMesas) {}

LimpiadorMesas::~LimpiadorMesas() {

}

void LimpiadorMesas::run() {

    // se registra el event handler declarado antes
    //SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );


    this->rutinaLimpiador();

    // se recibio la senial SIGINT, el proceso termina
    SignalHandler :: destruir ();
    std::cout << "Limpiador: fin del proceso" << std::endl;
    pedidos.cerrar();
    exit(0);
}

void LimpiadorMesas::rutinaLimpiador() {
    std::string pedido = "";
    lockLecturaMesas.tomarLock();

    while (!pedido.compare(LIMPIAR_PEDIDOS)) {
        char buffer[BUFFSIZE];

        std::stringstream ss;
        ss << "Limpiador: esperando para limpiar pedido..." << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "Limpiador: esperando para limpiar pedido..." << std::endl;
        ss.str("");
        ss.flush();
        ss.clear();

        //Leemos con un lock de lectura

        ssize_t bytesLeidos = this->pedidos.leer(static_cast<void *>(buffer), BUFFSIZE);  // Leo pedidos de cualquier mesa


        if (bytesLeidos <= 0) return;
        std::string mensaje = buffer;
        mensaje.resize(bytesLeidos); //Si todos escribimos BUFFSIZE no haria falta...

        std::stringstream ss1, ss2;
        unsigned i;
        for (i = 0; i < PID_LENGHT; ++i) {
            ss1 << mensaje.at(i);
        }
        bool pedirCuenta = true;
        for (; i < mensaje.length(); ++i) {
            ss2 << mensaje.at(i);
            pedirCuenta &= mensaje.at(i) == '0';
        }

        ss2 >> pedido;
    }
    lockLecturaMesas.liberarLock();
}