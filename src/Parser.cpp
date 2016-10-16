//
// Created by juan on 15/10/16.
//

#include "Parser.h"

bool Parser::obtenerTupla(std::string &header, int *cantidad) {

    infile >> header >> (*cantidad);
    return infile.good();
}

Parser::~Parser() {
    infile.close();
}
