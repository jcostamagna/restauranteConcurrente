//
// Created by nicolas on 26/10/16.
//
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream myfile ("pid.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            std::stringstream ss(line);
            int pid_main;
            ss >> pid_main;
            kill((pid_t)pid_main,18);
        }
        myfile.close();
    }

    else
        std::cout << "No se pudo leer el PID del archivo";

    return 0;
}