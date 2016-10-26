#include <iostream>
#include <fstream>
#include <list>
#include <Restaurante.h>
#include <Log.h>
#include <SignalHandler.h>
#include "src/Parser.h"
#include "Signal/Apagon_Handler.h"
#define SALIR "q"

int main() {
    std::string fileName = "pid.txt";
    std::ofstream myfile (fileName);
    if (myfile.is_open())
    {
        myfile << getpid();
        myfile.close();
    }
    else
        std::cout << "No se pudo crear archivo para el apagon";

    Log::getInstance()->setearArchivo("log.txt");

    std::string string("restaurante.config");

    Parser parser(string);
    std::string header;
    int recepCant, mozosCant, mesasCant, clientesCant, precio;

    //Leo la cantidad de recepcionistas, mozos y mesas
    parser.obtenerTupla(header, &recepCant);
    std::cout << "\nitem: " << header << "\tvalor: " << recepCant;
    parser.obtenerTupla(header, &mozosCant);
    std::cout << "\nitem: " << header << "\tvalor: " << mozosCant;
    parser.obtenerTupla(header, &mesasCant);
    std::cout << "\nitem: " << header << "\tvalor: " << mesasCant;
    parser.obtenerTupla(header, &clientesCant);
    std::cout << "\nitem: " << header << "\tvalor: " << clientesCant;

    std::vector<std::pair<std::string, int> > menu;

    while (parser.obtenerTupla(header, &precio)) {
        menu.push_back(std::make_pair(header, precio));
    }


    std::cout << "\nmylist contains:\n";
    for (std::vector<std::pair<std::string, int> >::iterator it = menu.begin(); it != menu.end(); ++it) {
        std::cout << "\t\t" << (*it).first << "\t" << (*it).second << "\n";
    }
    std::cout << std::endl;

    Restaurante resto(recepCant, mozosCant, mesasCant, clientesCant, menu);
    resto.iniciarPersonal();

    Apagon_Handler apagon_handler(resto);
    SignalHandler::getInstance()->registrarHandler(SIGCONT, &apagon_handler);


    std::cout << "Salir ingresando tecla 'q'" << std::endl;
    std::string mensaje;

    if (std::cin.fail() || std::cin.eof())
        std::cin.clear();
    std::getline(std::cin,mensaje);

    while (mensaje != SALIR){
        if (std::cin.fail() || std::cin.eof())
            std::cin.clear();
        std::getline(std::cin,mensaje);
        //std::cout << mensaje << std::endl;
    }

    SignalHandler::destruir();
    remove(const_cast<char*>(fileName.c_str()));
}
