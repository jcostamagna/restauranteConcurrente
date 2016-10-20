//
// Created by nicolas on 20/10/16.
//

#include "Mesa.h"

Mesa::Mesa(Pipe& living, Pipe& pedidos, LockFd& lockLiving, Semaforo& sEsperandoMozo) :
        living(living),pedidos(pedidos), lockLiving(lockLiving),sEsperandoMozo(sEsperandoMozo), estado(ESPERANDO_CLIENTE),{}

void Mesa::run() {
    this->rutinaMesa();

    this->living.cerrar();
    this->pedidos.cerrar();
    this->living.cerrar();
    this->sEsperandoMozo.eliminar();
    std::cout << "Termino el proceso " << getpid() << std::endl;

}

void Mesa::rutinaMesa() {
    while (sigint_handler.getGracefulQuit() == 0) {
        switch (estado) {
            case ESPERANDO_CLIENTE:
                esperandoCliente();
                break;
            case CLIENTE_SENTADO:
                clienteSentado();
                break;
            case CLIENTE_ESPERA_PEDIDO:
                clienteEsperaPedido();
                break;
            case CLIENTE_ESPERA_CUENTA:
                clienteEsperaCuenta();
                break;
            case APAGON_MESA:
                apagon();
                break;
            default:
                esperandoCliente();
                break;
        }
    }
}

void Mesa::avanzarEstado() {
    switch (estado) {
        case ESPERANDO_CLIENTE:
            estado = ESPERANDO_CLIENTE;
            break;
        case CLIENTE_SENTADO:
            estado = CLIENTE_SENTADO;
            break;
        case CLIENTE_ESPERA_PEDIDO:
            estado = CLIENTE_SENTADO;
            break;
        case CLIENTE_ESPERA_CUENTA:
            estado = CLIENTE_SE_VA;
            break;
        case CLIENTE_SE_VA:
            estado = ESPERANDO_CLIENTE;
            break;
        default:
            estado = ESPERANDO_CLIENTE;
            break;
    }
}

void Mesa::esperandoCliente() {
}

void Mesa::clienteSentado() {
}

void Mesa::clienteEsperaPedido() {

}

void Mesa::clienteEsperaCuenta() {

}

void Mesa::apagon() {

}
