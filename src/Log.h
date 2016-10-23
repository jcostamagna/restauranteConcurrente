//
// Created by ale on 15/10/16.
//

#ifndef RESTAURANTCONCURRENTE_LOG_H
#define RESTAURANTCONCURRENTE_LOG_H


#include <ostream>
#include "../Lock/LockFile.h"

class Log {
private:
    static Log* instance;
    static LockFile* lockFile;

    Log();
    void setActualTime(char buf[64]);

public:
    static Log* getInstance ();
    static void destruir ();
    bool setearArchivo(const char* archivo);

    void log(const char* msg);
    void log(std::string msg);
};


#endif //RESTAURANTCONCURRENTE_LOG_H