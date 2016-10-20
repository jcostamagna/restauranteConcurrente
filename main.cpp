
#include <iostream>
#include <sys/wait.h>
#include <list>
#include <GeneradorClientes.h>
#include <Semaforo.h>
#include <MemoriaCompartida2.h>
#include "Pipe&Fifo/Pipe.h"
#include "src/Parser.h"
#include "src/Recepcionista.h"
#define SALIR "q"


int main() {

//    static const int BUFFSIZE = 100;
    static const std::string NOMBRE = "/bin/bash";
    //static const std::string NOMBRE2 = "README.md";


    std::string string("restaurante.config");

    Parser parser(string);
    std::string header;
    int recepCant, mozosCant, mesasCant, precio;

    //Leo la cantidad de recepcionistas, mozos y mesas
    parser.obtenerTupla(header, &recepCant);
    std::cout << "\nitem: " << header << "\tvalor: " << recepCant;
    parser.obtenerTupla(header, &mozosCant);
    std::cout << "\nitem: " << header << "\tvalor: " << mozosCant;
    parser.obtenerTupla(header, &mesasCant);
    std::cout << "\nitem: " << header << "\tvalor: " << mesasCant;

    std::list<std::pair<std::string, int> > menu;

    while (parser.obtenerTupla(header, &precio)) {
        menu.push_back(std::make_pair(header, precio));
    }


    std::cout << "\nmylist contains:\n";
    for (std::list<std::pair<std::string, int> >::iterator it = menu.begin(); it != menu.end(); ++it) {
        std::cout << "\t\t" << (*it).first << "\t" << (*it).second << "\n";
    }
    std::cout << '\n';
    std::cout << "ESTO ANDA" << std::endl;

    Pipe clientes, living, clientesAMesa;
    LockFd lockLectura(clientes.getFdLectura());


    Semaforo escrituraLiving(NOMBRE, 0);

    //Inicializo cantidad de Clientes en el Living en 0
    //escrituraLiving.p();



    std::list<Recepcionista*> recepcionistas;
    for (int i = 0; i < recepCant; i++){
        Recepcionista* recepcionista = new Recepcionista(clientes,lockLectura, escrituraLiving, living, clientesAMesa);
        recepcionista->start();
        recepcionistas.push_back(recepcionista);
    }
    MemoriaCompartida2<int> cantClientesLiving;
    cantClientesLiving.crear(NOMBRE, 'z');
    cantClientesLiving.escribir(0);

    escrituraLiving.v();

    GeneradorClientes generador(clientes);
    generador.start();



    // espero a que termine el hijo
    //wait ( NULL );
    std::cout << "Salir ingresando tecla 'q'" << std::endl;
    std::string mensaje;
    std::getline(std::cin,mensaje);
    while (mensaje != SALIR){
        std::getline(std::cin,mensaje);
    }


    for (std::list<Recepcionista*>::iterator it = recepcionistas.begin(); it != recepcionistas.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    kill(generador.get_pid(), SIGINT);
    generador.stop();
    escrituraLiving.eliminar();
    cantClientesLiving.liberar();
    exit(0);
}
