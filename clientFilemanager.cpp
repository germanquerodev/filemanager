#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "filemanager_stub.h"

void freeListedFiles(std::vector<std::string *> *fileList)
{
    for (std::vector<std::string *>::iterator i = fileList->begin(); i != fileList->end(); ++i)
    {
        delete *i;
    }
    delete fileList;
}

int main(int argc, char **argv)
{
    FileManager *fm = new FileManager("./dirprueba/");
    std::vector<std::string *> *vfiles = fm->listFiles();
    std::cout << "Lista de ficheros en el directorio de prueba:\n";
    for (unsigned int i = 0; i < vfiles->size(); ++i)
    {
        std::cout << "Fichero: " << vfiles->at(i)->c_str() << std::endl;
    }
    std::cout << "Leyendo el primer fichero del directorio de prueba:\n";

    char *data = nullptr;
    unsigned long int fileLen = 0;
    fm->readFile(&(*(vfiles->at(0)))[0], data, fileLen);
    std::cout << "Escribiendo el primer fichero del directorio de prueba:\n";
    fm->writeFile(&(*(vfiles->at(0)))[0], data, fileLen);
    std::cout << "Liberando lista de ficheros:\n";
    freeListedFiles(vfiles);
    std::cout << "Liberando datos de fichero leído:\n";
    
    delete[] data;
    
    delete fm;
    return 0;
}
