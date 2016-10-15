//
// Created by juan on 15/10/16.
//

#include "Parser.h"

bool Parser::obtenerTupla(std::string &header, int *cantidad) {

    return (infile >> header >> (*cantidad));
}
