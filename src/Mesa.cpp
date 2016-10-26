//
// Created by nicolas on 20/10/16.
//

#include <iomanip>
#include <sstream>
#include "Mesa.h"
#include "Log.h"

Mesa::Mesa(Pipe &living, Pipe &pedidos, LockFd &lockLiving, Semaforo *sEsperandoMozo, Semaforo &escrituraLiving,
           std::vector<std::pair<std::string, int> > menu) :
        living(living), pedidos(pedidos), lockLiving(lockLiving), estado(ESPERANDO_CLIENTE),
        sEsperandoMozo(sEsperandoMozo), escrituraLiving(escrituraLiving), idCliente(-1),
        cantClientesLiving("/bin/bash", 'z'), cuentaSesion(0), menu(menu) {}

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
        if (apagon_handler_procesos.getApagon()) {
            std::cout << "APAGONNNNNNNNNNNNNN" << std::endl << std::endl << std::endl;
            estado = APAGON_MESA;
        }
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
            case CLIENTE_COMIENDO:
                comer();
            case CLIENTE_ESPERA_CUENTA:
                clienteEsperaCuenta();
                break;
            case APAGON_MESA:
                apagonMesa();
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
            estado = CLIENTE_COMIENDO;
            break;
        case CLIENTE_COMIENDO:
            // Con una probabilidad de 0.3 vuelve a pedir comida. Podria ser interminable...
            if (calcularRandom(10) < 3)
                estado = CLIENTE_SENTADO;
            else
                estado = CLIENTE_ESPERA_CUENTA;
            break;
        case CLIENTE_ESPERA_CUENTA:
            estado = CLIENTE_SE_VA;
            break;
        case CLIENTE_SE_VA:
            estado = ESPERANDO_CLIENTE;
            break;
        case APAGON_MESA:
            estado = ESPERANDO_CLIENTE;
            break;
        default:
            estado = ESPERANDO_CLIENTE;
            break;
    }
}

void Mesa::esperandoCliente() {
    cuentaSesion = 0; // reseteo la cuenta antes de que venga otro grupo de comensales

    // lector
    char buffer[BUFFSIZE];

    std::stringstream ss;
    ss << "Mesa(" << getpid() << "): Esperando que clientes vayan al living..." << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << "): Esperando que clientes vayan al living..." << std::endl;


    //Leemos del pipe living con un lock de lectura
    this->lockLiving.tomarLock();
    ssize_t bytesLeidos = this->living.leer(static_cast<void *>(buffer), BUFFSIZE);
    this->lockLiving.liberarLock();

    if (bytesLeidos <= 0) return;

    escrituraLiving.p();
    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos);
    std::string::size_type sz;

    std::stringstream ss1;
    unsigned int i;
    for (i = 0; i < PID_LENGHT && i < mensaje.size(); ++i) {
        ss1 << mensaje.at(i);
    }
    ss1 >> idCliente;

    ss.str("");
    ss << "Mesa(" << getpid() << "): Me llevo al cliente [" << idCliente << "] (" << bytesLeidos
       << " bytes) del pipe living"
       << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << "): Me llevo al cliente [" << idCliente << "] (" << bytesLeidos
              << " bytes) del pipe living"
              << std::endl;


    //TODO: sacar del pipe y actualizar cantidad en el mismo bloque critico
    //Bloqueo para que no puedan cambiar la cantidad de gente en el living


    int cantClientes = this->cantClientesLiving.leer();
    cantClientes--;

    ss.str("");
    ss << "Mesa(" << getpid() << "): Cantidad clientes en living [" << cantClientes << "] " << std::endl;
    Log::getInstance()->log(ss.str());

    std::cout << "Mesa(" << getpid() << "): Cantidad clientes en living [" << cantClientes << "] " << std::endl;
    this->cantClientesLiving.escribir(cantClientes);

    escrituraLiving.v();
    avanzarEstado();
}

void Mesa::clienteSentado() {
    //Hacer pedido, los primeros 10 bytes son el pid, el resto es la comida
    std::ostringstream ss;
    pedirComida(ss);
    std::string dato(ss.str());
    pedidos.escribir(static_cast<const void *>(dato.c_str()), dato.size());

    ss.str("");
    ss << "Mesa(" << getpid() << "): Hago pedido de [" << dato << "] en el pipe del cliente [" << idCliente << "]"
       << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << "): Hago pedido de [" << dato << "] en el pipe del cliente [" << idCliente
              << "]" << std::endl;

    // EL PRECIO
    //this->cuenta += 50;
    avanzarEstado();
}

void Mesa::clienteEsperaPedido() {
    std::stringstream ss;
    ss << "Mesa(" << getpid() << "): Esperando Comida " << "del cliente [" << idCliente << "]" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << "): Esperando Comida " << "del cliente [" << idCliente << "]" << std::endl;


    this->sEsperandoMozo->p(); //me bloqueo mientras espero al mozo. Me desbloqueo cuando el mozo me hace v()
    avanzarEstado();
}

void Mesa::comer() {
    std::stringstream ss;
    ss << "Mesa(" << getpid() << ") del cliente [" << idCliente << "]: COMIENDO" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << ") del cliente [" << idCliente << "]: COMIENDO" << std::endl;

    sleep(1);

    ss.str("");
    ss << "Mesa(" << getpid() << ") del cliente [" << idCliente << "]: Ya comi, que rico!!" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << ") del cliente [" << idCliente << "]: Ya comi, que rico!!" << std::endl;

    avanzarEstado();
}

void Mesa::clienteEsperaCuenta() {
//Hacer pedido de cuenta
    //static const int BUFFSIZE = 10;
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(PID_LENGHT) << getpid();
    //Pedir algo de 0 pesos es pedir la cuenta
    ss << std::setfill('0') << std::setw(BUFFSIZE-PID_LENGHT) << 0;
    std::string dato(ss.str());
    pedidos.escribir(static_cast<const void *>(dato.c_str()), dato.size());

    ss.str("");
    ss << "Mesa(" << getpid() << "): Hago pedido de cuenta en el pipe del mozo [" << idCliente << "]"
       << std::endl;
    Log::getInstance()->log(ss.str());

    std::cout << "Mesa(" << getpid() << "): Hago pedido de cuenta en el pipe del mozo [" << idCliente << "]"
              << std::endl;

    this->sEsperandoMozo->p(); //me bloqueo mientras espero al mozo. Me desbloqueo cuando el mozo me hace v()

    avanzarEstado();
    avanzarEstado(); //Vuelvo al estado ESPERANDO_CLIENTE por si viene otro cliente.
}

void Mesa::apagonMesa() {
    std::stringstream ss;
    ss << "APAGON: Mesa(" << getpid() << ") del cliente [" << idCliente << "]" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "APAGON: Mesa(" << getpid() << ") del cliente [" << idCliente << "]" << std::endl;

    avanzarEstado();
}



void Mesa::pedirComida(std::ostringstream &stream) {
    stream << std::setfill('0') << std::setw(PID_LENGHT) << getpid();
    std::pair<std::string, int> plato = menu[calcularRandom(menu.size())];
    std::string comida = plato.first;
    int precio = plato.second;
    cuentaSesion += precio;

    stream << std::setfill('0') << std::setw(BUFFSIZE-PID_LENGHT) << comida;
}


int Mesa::calcularRandom (int max) {
    srand ( time(NULL) );
    int resultado = rand() % max;
    return resultado;
}