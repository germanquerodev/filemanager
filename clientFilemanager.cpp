#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
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

    do
    {
        std::cout << "Que desea hacer?\n> ls\n> upload ${fileName}\n> download ${fileName}\n>exit\n";

        // obtener comando
        std::string input;
        std::getline(std::cin, input);

        // obtener atributos del comando
        std::vector<std::string> atributes;

        size_t pos = 0;
        while (pos != std::string::npos)
        {
            // Buscar el espacio en blanco
            size_t spacePos = input.find(' ', pos);
            // Extraer el token desde la posición actual hasta el espacio en blanco
            std::string token = input.substr(pos, spacePos - pos);
            // Agregar el token al vector
            atributes.push_back(token);
            // Actualizar la posición al siguiente carácter después del espacio en blanco
            pos = (spacePos == std::string::npos) ? std::string::npos : spacePos + 1;
        }

        if (!atributes.empty())
        {
            // Comparar el comando
            if (atributes[0] == "ls")
            {
                // ls
                std::vector<std::string *> *vfiles = fm->listFiles();
                std::cout << "Lista de ficheros en el directorio de prueba:\n";
                for (unsigned int i = 0; i < vfiles->size(); ++i)
                {
                    std::cout << "Fichero: " << vfiles->at(i)->c_str() << std::endl;
                }
                freeListedFiles(vfiles);
            }
            else if (atributes[0] == "upload")
            {
                // Verificar si se proporcionó un nombre de archivo
                if (atributes.size() >= 2)
                {
                    std::string fileName = atributes[1];
                    // upload

                    // Puntero para almacenar el contenido del archivo
                    char *data = nullptr;

                    // Tamaño de los datos leídos
                    unsigned long int dataSize = 0;

                    std::ifstream inFile(fileName, std::ios::binary);
                    if (inFile.is_open())
                    {
                        // Obtener el tamaño del archivo
                        inFile.seekg(0, std::ios::end);
                        dataSize = inFile.tellg();
                        inFile.seekg(0, std::ios::beg);

                        // Almacenar el contenido del archivo en el puntero data
                        data = new char[dataSize];
                        inFile.read(data, dataSize);

                        // Cerrar el archivo después de leer
                        inFile.close();

                        // subir archivo
                        fm->writeFile((char *)fileName.data(), data, dataSize);

                        std::cout << "Subido: " << fileName << std::endl;
                    }
                    else
                    {
                        std::cerr << "Error al abrir el archivo para lectura: " << fileName << std::endl;
                    }
                }
                else
                {
                    std::cout << "Error: Falta el nombre del archivo para el comando 'upload'\n";
                }
            }
            else if (atributes[0] == "download")
            {
                // Verificar si se proporcionó un nombre de archivo
                if (atributes.size() >= 2)
                {
                    std::string fileName = atributes[1];
                    // download
                    char *data = nullptr;
                    unsigned long int fileLen = 0;
                    fm->readFile((char *)fileName.data(), data, fileLen);

                    // Crear un objeto ofstream para escribir en el archivo
                    std::ofstream outFile(fileName);

                    // Verificar si el archivo se ha abierto correctamente
                    if (outFile.is_open())
                    {
                        // Escribir datos en el archivo
                        outFile.write(data, fileLen);

                        // Cerrar el archivo después de escribir
                        outFile.close();

                        std::cout << "Descargado: " << fileName << std::endl;
                    }
                    else
                    {
                        std::cerr << "Error en la descarga" << fileName << std::endl;
                    }
                }
                else
                {
                    std::cout << "Error: Falta el nombre del archivo para el comando 'download'\n";
                }
            }
            else if (atributes[0] == "exit")
            {
                // Salir del bucle
                break;
            }
            else
            {
                std::cout << "Comando no reconocido: " << atributes[0] << "\n";
            }
        }
    } while (true);

    // std::vector<std::string *> *vfiles = fm->listFiles();
    // std::cout << "Lista de ficheros en el directorio de prueba:\n";
    // for (unsigned int i = 0; i < vfiles->size(); ++i)
    // {
    //     std::cout << "Fichero: " << vfiles->at(i)->c_str() << std::endl;
    // }
    // std::cout << "Leyendo el primer fichero del directorio de prueba:\n";

    // char *data = nullptr;
    // unsigned long int fileLen = 0;
    // fm->readFile(&(*(vfiles->at(0)))[0], data, fileLen);
    // std::cout << "Escribiendo el primer fichero del directorio de prueba:\n";
    // fm->writeFile(&(*(vfiles->at(0)))[0], data, fileLen);
    // std::cout << "Liberando lista de ficheros:\n";
    // freeListedFiles(vfiles);
    // std::cout << "Liberando datos de fichero leído:\n";

    // delete[] data;

    delete fm;
    return 0;
}
