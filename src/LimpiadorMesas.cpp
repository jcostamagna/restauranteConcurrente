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
    bool terminaLimpieza = false;

    //Leemos con un lock de lectura
    lockLecturaMesas.tomarLock();

    while (!terminaLimpieza) {
        char buffer[BUFFSIZE];

        std::stringstream ss;
        ss << "Limpiador: esperando para limpiar pedido..." << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "Limpiador: esperando para limpiar pedido..." << std::endl;
        ss.str("");
        ss.flush();
        ss.clear();

        ssize_t bytesLeidos = this->pedidos.leer(static_cast<void *>(buffer), BUFFSIZE);  // Leo pedidos de cualquier mesa


        if (bytesLeidos <= 0) {
            lockLecturaMesas.liberarLock();
            return;
        }
        std::string mensaje = buffer;
        mensaje.resize(bytesLeidos); //Si todos escribimos BUFFSIZE no haria falta...

        std::stringstream ss2;
        unsigned i;
        terminaLimpieza = true;
        for (i=PID_LENGHT; i < mensaje.length(); ++i) {
            ss2 << mensaje.at(i);
            terminaLimpieza &= mensaje.at(i) == LIMPIAR_PEDIDOS;
        }
        //Si todos los mensajes son X, se termino la limpieza

        ss2 >> pedido;

        ss << "Limpiador: tirando el pedido..." << pedido << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "Limpiador: tirando el pedido..." << pedido << std::endl;
    }
    lockLecturaMesas.liberarLock();
}