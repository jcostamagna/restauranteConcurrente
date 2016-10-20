//
// Created by nicolas on 20/10/16.
//

#include "Mesa.h"

Mesa::Mesa(Pipe& living, Pipe& pedidos, LockFd& lockLiving, Semaforo& sEsperandoMozo, Semaforo& escrituraLiving) :
        living(living),pedidos(pedidos), lockLiving(lockLiving),estado(ESPERANDO_CLIENTE), sEsperandoMozo(sEsperandoMozo),
        escrituraLiving(escrituraLiving), idCliente(-1), cantClientesLiving("/bin/bash", 'z'){}

void Mesa::run() {
    this->rutinaMesa();

    this->living.cerrar();
    //this->pedidos.cerrar();
    //this->living.cerrar();
    //this->escrituraLiving.eliminar();
    //this->sEsperandoMozo.eliminar();
    std::cout << "Termino el proceso mesa " << getpid() << std::endl;

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
            estado = CLIENTE_SENTADO;
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
    static const int BUFFSIZE = 8;

    // lector
    char buffer[BUFFSIZE];

    std::cout << "Mesa("<< getpid() <<"): Esperando clientes..." << std::endl;


    //Leemos con un lock de lectura
    this->lockLiving.tomarLock();
    ssize_t bytesLeidos = this->living.leer(static_cast<void *>(buffer), BUFFSIZE);
    this->lockLiving.liberarLock();

    if (bytesLeidos <= 0) return;
    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos);
    std::string::size_type sz;

    this->idCliente = std::stoi(mensaje,&sz);

    std::cout << "Mesa("<< getpid() <<"): Tengo al cliente [" << idCliente << "] (" << bytesLeidos << " bytes) del pipe"
              << std::endl;


    escrituraLiving.p();


    int cantClientes = this->cantClientesLiving.leer();
    std::cout << "Mesa("<< getpid() <<"): Cantidad clientes en living [" << cantClientes << "] "<< std::endl;
    cantClientes--;
    this->cantClientesLiving.escribir(cantClientes);

    escrituraLiving.v();
}

void Mesa::clienteSentado() {
}

void Mesa::clienteEsperaPedido() {

}

void Mesa::clienteEsperaCuenta() {

}

void Mesa::apagon() {

}
