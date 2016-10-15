//
// Created by juan on 15/10/16.
//

#ifndef RESTAURANTCONCURRENTE_PARSER_H
#define RESTAURANTCONCURRENTE_PARSER_H

#include <fstream>
#include <string>

class Parser {
    private:
        //const std::string& archivo;
        std::ifstream infile;
        Parser(const Parser& object);
        Parser& operator=(const Parser& object);

    public:
        Parser (const std::string& archivo): infile(archivo.c_str()) {}
        bool obtenerTupla(std::string& header, int* cantidad);
        //~Parser ();

};


#endif //RESTAURANTCONCURRENTE_PARSER_H
