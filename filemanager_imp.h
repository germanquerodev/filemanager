#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "filemanager.h"

// clase interfaz cliente
class fileManager_imp
{
private:
	int clientId = -1;
	FileManager *p = nullptr;

public:
	fileManager_imp(int clientId) : clientId(clientId){};

	bool connectionClosed()
	{
		return p == nullptr;
	}

	void recibeOp()
	{
		// inicializa rpcs
		std::vector<unsigned char> rpcIn;
		std::vector<unsigned char> rpcOut;

		// recibe el paquete
		recvMSG(clientId, rpcIn);

		// desempaqueta la operacion
		filemanagerOp operacion = unpack<filemanagerOp>(rpcIn);

		// ejecuta dependiendo de la operacion
		switch (operacion)
		{
		case constructorOp:
		{

			// inicializa el objeto
			p = new FileManager("./dirprueba");
			// devulelve ok
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;
		case destructorOp:
		{

			// libera la memoria del objeto
			delete p;
			p = nullptr;
			// devuelve el ok
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;
		case readFileOp:
		{
			// desempaqueta la dimension del string
			int s = unpack<int>(rpcIn);
			// reserva memoria y desempaqeta el string del nombre del fichero
			std::string fileName;
			fileName.resize(s);
			unpackv(rpcIn, (char *)fileName.data(), s);

			// declara un vector de chars vacio y un integrer para la dimension
			// lee el archivo de nombre fileName e introduce el contenido en data
			char *data = nullptr;
			unsigned long int fileLen = 0;
			p->readFile((char *)fileName.data(), data, fileLen);

			// empaqueta el ok
			pack(rpcOut, (unsigned char)MSG_OK);

			// empaqueta el tamaño
			pack(rpcOut, fileLen);

			// desempaqueta la matriz
			packv(rpcOut, data, fileLen);
		}
		break;
		case writeFileOp:
		{
			// desempaqueta la dimension del string
			int s = unpack<int>(rpcIn);
			// reserva memoria y desempaqeta el string del nombre del fichero
			std::string fileName;
			fileName.resize(s);
			unpackv(rpcIn, (char *)fileName.data(), s);
			unsigned long int fileLen = unpack<unsigned long int>(rpcIn);
			char *data = new char[fileLen];
			unpackv(rpcIn, data, fileLen);
			p->writeFile((char *)fileName.data(), data, fileLen);
			// empaqueta el ok
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;

		case listFilesOp:
		{
			vector<string *> *flist = new vector<string *>();
			flist = p->listFiles();

			// empaqueta el ok
			pack(rpcOut, (unsigned char)MSG_OK);

			// empaqueta la dimension del vector
			unsigned long int flist_size = flist->size();
			pack(rpcOut, flist_size);

			// por cada elemento empaqueta su dimension y su vector
			for (int i = 0; i < flist_size; i++)
			{
				int s = flist->at(i)->size();
				pack(rpcOut, s);
				packv(rpcOut, (char *)flist->at(i)->data(), s);
			}
			p->freeListedFiles(flist);
		}
		break;
		default:
		{
			std::cout << "Error: función no definida\n";
			pack(rpcOut, (unsigned char)MSG_NOK);
		}
		break;
		};
		// devuelve resultados
		sendMSG(clientId, rpcOut);
	};
};
