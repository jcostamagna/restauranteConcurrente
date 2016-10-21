
#include <iostream>
#include <list>
#include <Restaurante.h>
#include <Log.h>
#include "src/Parser.h"
#define SALIR "q"

int main() {

    Log::getInstance()->setearArchivo("log.txt");

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

    Restaurante resto(recepCant, mozosCant, mesasCant, menu);
    resto.iniciarPersonal();


    std::cout << "Salir ingresando tecla 'q'" << std::endl;
    std::string mensaje;
    std::getline(std::cin,mensaje);
    while (mensaje != SALIR){
        std::getline(std::cin,mensaje);
    }

}
