
#include <iostream>
#include <sys/wait.h>
#include <list>
#include "Pipe&Fifo/Pipe.h"
#include "src/Parser.h"
#include "src/Recepcionista.h"


int main() {

//    static const int BUFFSIZE = 100;


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


    Pipe canal;
    //int pid = fork ();
    std::list<Recepcionista> recepcionistas;
    for (int i = 0; i < recepCant; i++){
        Recepcionista recepcionista(canal);
        recepcionista.start();
        recepcionistas.push_back(recepcionista);
    }
    //Recepcionista recepcionista(canal);
    //recepcionista.start();
    /*if ( pid == 0 ) {
        // lector
        char buffer[BUFFSIZE];
        std::cout << "Lector: esperando para leer..." << std::endl;
        ssize_t bytesLeidos = canal.leer ( static_cast<void*>(buffer),BUFFSIZE );
        std::string mensaje = buffer;
        mensaje.resize ( bytesLeidos );
        std::cout << "Lector: lei el dato [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe" << std::endl;
        std::cout << "Lector: fin del proceso" << std::endl;
        canal.cerrar ();
        exit ( 0 );
    } else {*/

    // escritor
    std::string dato = "Hola mundo pipes!!";
    sleep(5);

    for (int i = 0; i < recepCant; i++){
        std::string datoNew = dato + std::string("Numero") +std::to_string(i);
        canal.escribir(static_cast<const void *>(datoNew.c_str()), datoNew.size());
        std::cout << "Escritor: escribi el dato [" << datoNew << "] en el pipe" << std::endl;
    }



    std::cout << "Escritor: fin del proceso" << std::endl;

    // espero a que termine el hijo
    //wait ( NULL );

    for (std::list<Recepcionista>::iterator it = recepcionistas.begin(); it != recepcionistas.end(); ++it){
        //std::cout << "\t\t" << (*it)<< "\n";
        (*it).stop();
    }
    canal.cerrar();
    exit(0);
}
