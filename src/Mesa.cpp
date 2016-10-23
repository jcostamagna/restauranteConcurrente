//
// Created by nicolas on 20/10/16.
//

#include <iomanip>
#include <sstream>
#include "Mesa.h"
#include "Log.h"

Mesa::Mesa(Pipe& living, Pipe& pedidos, LockFd& lockLiving, Semaforo* sEsperandoMozo, Semaforo& escrituraLiving) :
        living(living),pedidos(pedidos), lockLiving(lockLiving),estado(ESPERANDO_CLIENTE), sEsperandoMozo(sEsperandoMozo),
        escrituraLiving(escrituraLiving), idCliente(-1), cantClientesLiving("/bin/bash", 'z'), cuenta(0){}

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
            estado = CLIENTE_ESPERA_PEDIDO;
            break;
        case CLIENTE_ESPERA_PEDIDO:
            estado = CLIENTE_ESPERA_CUENTA;
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

    std::stringstream ss;
    ss << "Mesa("<< getpid() <<"): Esperando clientes..." << std::endl;
    Log::getInstance()->log(ss.str());
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

    ss.str("");
    ss << "Mesa("<< getpid() <<"): Tengo al cliente [" << idCliente << "] (" << bytesLeidos << " bytes) del pipe"
       << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa("<< getpid() <<"): Tengo al cliente [" << idCliente << "] (" << bytesLeidos << " bytes) del pipe"
              << std::endl;


    escrituraLiving.p();


    int cantClientes = this->cantClientesLiving.leer();
    cantClientes--;

    ss.str("");
    ss << "Mesa("<< getpid() <<"): Cantidad clientes en living [" << cantClientes << "] "<< std::endl;
    Log::getInstance()->log(ss.str());

    std::cout << "Mesa("<< getpid() <<"): Cantidad clientes en living [" << cantClientes << "] "<< std::endl;
    this->cantClientesLiving.escribir(cantClientes);

    escrituraLiving.v();
    avanzarEstado();
}

void Mesa::clienteSentado() {
    //Hacer pedido
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(6) << getpid();
    ss << std::setfill('0') << std::setw(4) << 50;
    std::string dato(ss.str());
    pedidos.escribir(static_cast<const void *>(dato.c_str()), dato.size());
    // tomar comida al azar del menu y su precio

    ss.str("");
    ss << "Mesa("<< getpid() <<"): Hago pedido de [" << dato << "] en el pipe " << "del cliente [" << idCliente << "]" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa("<< getpid() <<"): Hago pedido de [" << dato << "] en el pipe " << "del cliente [" << idCliente << "]" << std::endl;


    this->cuenta += 50;
    avanzarEstado();
}

void Mesa::clienteEsperaPedido() {
    std::stringstream ss;
    ss <<  "Mesa("<< getpid() <<"): Esperando Comida " << "del cliente [" << idCliente << "]"<< std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa("<< getpid() <<"): Esperando Comida " << "del cliente [" << idCliente << "]"<< std::endl;


    this->sEsperandoMozo->p();
    avanzarEstado();
}

void Mesa::clienteEsperaCuenta() {
//Hacer pedido
    //static const int BUFFSIZE = 10;
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(6) << getpid();
    //Pedir algo de 0 pesos es pedir la cuenta
    ss << std::setfill('0') << std::setw(4) << 0;
    std::string dato(ss.str());
    pedidos.escribir(static_cast<const void *>(dato.c_str()), dato.size());

    ss.str("");
    ss << "Mesa("<<getpid()<<"): Hago pedido de cuenta en el pipe " << "del cliente [" << idCliente << "]"<< std::endl;
    Log::getInstance()->log(ss.str());

    std::cout << "Mesa("<<getpid()<<"): Hago pedido de cuenta en el pipe " << "del cliente [" << idCliente << "]"<< std::endl;

    this->sEsperandoMozo->p();

    avanzarEstado();
    avanzarEstado();
}

void Mesa::apagon() {

}
