//
// Created by nicolas on 20/10/16.
//

#include <iomanip>
#include <sstream>
#include "Mesa.h"
#include "Log.h"
#include "lockFiles.h"

Mesa::Mesa(Pipe &living, Pipe &pedidos, LockFd &lockLiving, Semaforo *sEsperandoMozo, Semaforo &escrituraLiving,
           Semaforo &semCajaRestaurante, Semaforo &semDineroPerdido, std::vector<std::pair<std::string, int> > menu) :
        living(living), pedidos(pedidos), lockLiving(lockLiving), estado(ESPERANDO_CLIENTE),
        sEsperandoMozo(sEsperandoMozo), idCliente(-1),
        cantClientesLiving(SM_CLIENTES_LIVING_FILE, SM_CLIENTES_LIVING_LETRA),
        escrituraLiving(escrituraLiving),
        cuentaSesion(0), dineroPerdido(SM_DINERO_PERDIDO_FILE, SM_DINERO_PERDIDO_LETRA),
        semDineroPerdido(semDineroPerdido), cajaResto(SM_CAJA_FILE, SM_CAJA_LETRA),
        semCajaRestaurante(semCajaRestaurante), menu(menu) {}

void Mesa::run() {
    this->rutinaMesa();

    this->living.cerrar();
    this->pedidos.cerrar();

    std::cout << "Termino el proceso mesa " << getpid() << std::endl;

}

void Mesa::rutinaMesa() {
    while (sigint_handler.getGracefulQuit() == 0) {
        if (apagon_handler_procesos.getApagon() == 1) {
            estado = APAGON_MESA;
            apagon_handler_procesos.stopApagon();
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
                break;
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
            if (calcularRandom(10) < 3) {
                estado = CLIENTE_SENTADO;
                std::stringstream ss;
                ss << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Vuelvo a pedir comida!! :)" << std::endl;
                Log::getInstance()->log(ss.str());
            } else {
                estado = CLIENTE_ESPERA_CUENTA;
            }
            break;
        case CLIENTE_ESPERA_CUENTA:
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
    idCliente = -1;

    // lector
    char buffer[BUFFSIZE];

    std::stringstream ss;
    ss << "Mesa(" << getpid() << "): Esperando que clientes vayan al living..." << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << "): Esperando que clientes vayan al living..." << std::endl;


    //Leemos del pipe living con un lock de lectura
    this->lockLiving.tomarLock();
    ssize_t bytesLeidos = this->living.leer(static_cast<void *>(buffer), BUFFSIZE);
    escrituraLiving.p();

    if (bytesLeidos <= 0) {
        std::cerr << "PASE SIN HABER LEIDO NADA!!!!!!!!!" << std::endl;
        Log::getInstance()->log("PASE SIN HABER LEIDO NADA!!!!!!!!!\n");
        this->lockLiving.liberarLock();
        escrituraLiving.v();
        return;
    }


    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos);

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

    int cantClientes = this->cantClientesLiving.leer();
    cantClientes--;

    ss.str("");
    ss << "Mesa(" << getpid() << "): Cantidad clientes en living [" << cantClientes << "] " << std::endl;
    Log::getInstance()->log(ss.str());

    std::cout << "Mesa(" << getpid() << "): Cantidad clientes en living [" << cantClientes << "] " << std::endl;
    this->cantClientesLiving.escribir(cantClientes);

    this->lockLiving.liberarLock();
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
    ss << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Hago pedido de [" << dato << "] en el pipe" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Hago pedido de [" << dato << "] en el pipe" << std::endl;

    avanzarEstado();
}

void Mesa::clienteEsperaPedido() {
    std::stringstream ss;
    ss << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Esperando Comida "<< std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Esperando Comida "<< std::endl;

    try {
        this->sEsperandoMozo->p(); //me bloqueo mientras espero al mozo. Me desbloqueo cuando el mozo me hace v()
    } catch (int e) {
        if (e == APAGON_MATA_SEMAFORO)
            return;
    }
    avanzarEstado();
}

void Mesa::comer() {
    std::stringstream ss;
    ss << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: COMIENDO" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: COMIENDO" << std::endl;

    sleep(1);
    if (apagon_handler_procesos.getApagon() == 1) {
        estado = APAGON_MESA;
        apagon_handler_procesos.stopApagon();

        ss.str("");
        ss << "APAGON: Mesa(" << getpid() << "), cliente [" << idCliente << "]: No pude terminar de comer por el apagon!" << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "APAGON: Mesa(" << getpid() << "), cliente [" << idCliente << "]: No pude terminar de comer por el apagon!" << std::endl;
        return;
    } else {
        ss.str("");

        ss << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Ya comi, que rico!!" << std::endl;


        Log::getInstance()->log(ss.str());

        std::cout << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Ya comi, que rico!!" << std::endl;

        avanzarEstado();
    }
}

void Mesa::clienteEsperaCuenta() {
//Hacer pedido de cuenta
    //static const int BUFFSIZE = 10;
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(PID_LENGHT) << getpid();
    //Pedir algo de 0 pesos es pedir la cuenta
    ss << std::setfill('0') << std::setw(BUFFSIZE - PID_LENGHT) << 0;
    std::string dato(ss.str());
    pedidos.escribir(static_cast<const void *>(dato.c_str()), dato.size());

    ss.str("");
    ss << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Hago pedido de cuenta en el pipe" << std::endl;
    Log::getInstance()->log(ss.str());

    std::cout << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Hago pedido de cuenta en el pipe"  << std::endl;

    try {
        this->sEsperandoMozo->p(); //me bloqueo mientras espero al mozo. Me desbloqueo cuando el mozo me hace v()
    } catch (int e) {
        if (e == APAGON_MATA_SEMAFORO)
            return;
    }

    ss.str("");
    ss << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Pagando [" << cuentaSesion << "] pesos" << std::endl;
    Log::getInstance()->log(ss.str());

    std::cout << "Mesa(" << getpid() << "), cliente [" << idCliente << "]: Pagando [" << cuentaSesion << "] pesos" << std::endl;

    //pago
    semCajaRestaurante.p();
    int caja = cajaResto.leer();
    cajaResto.escribir(caja + cuentaSesion);
    semCajaRestaurante.v();
    cuentaSesion = 0;

    avanzarEstado(); //Vuelvo al estado ESPERANDO_CLIENTE por si viene otro cliente.
}

void Mesa::apagonMesa() {
    std::stringstream ss;
    ss << "APAGON: Mesa(" << getpid() << "), cliente [" << idCliente << "]" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "APAGON: Mesa(" << getpid() << "), cliente [" << idCliente << "]" << std::endl;

    //actualizo la plata que se perdio
    semDineroPerdido.p();
    int perdido = dineroPerdido.leer();
    dineroPerdido.escribir(perdido + cuentaSesion);
    semDineroPerdido.v();
    cuentaSesion = 0;

    sleep(TIEMPO_APAGON);
    avanzarEstado();
}


void Mesa::pedirComida(std::ostringstream &stream) {
    stream << std::setfill('0') << std::setw(PID_LENGHT) << getpid();
    std::pair<std::string, int> plato = menu[calcularRandom(menu.size())];
    std::string comida = plato.first;
    int precio = plato.second;
    cuentaSesion += precio;

    stream << std::setfill('0') << std::setw(BUFFSIZE - PID_LENGHT) << comida;
}


int Mesa::calcularRandom(int max) {
    srand(time(NULL));
    int resultado = rand() % max;
    return resultado;
}