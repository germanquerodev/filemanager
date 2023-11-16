#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "operaciones.h"

class FileManager
{
private:
	std::string ip = "127.0.0.1";
	int port = 60000;
	connection_t serverConnection;

public:
	FileManager(const std::string &filePath)
	{
		// conectar con servidor
		serverConnection = initClient(ip, port);

		// inicializa la operacion para llamar al constructor en el servidor
		filemanagerOp op = constructorOp;

		// inicializa rpcs
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		// empaqeta la operacion
		pack(rpcOut, op);

		// envia el parquete al servidor
		sendMSG(serverConnection.serverId, rpcOut);
		// recibe el paquete del servidor
		recvMSG(serverConnection.serverId, rpcIn);

		// comprueba el ok
		if (rpcIn[0] != MSG_OK) // si no es ok salta error
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}
	}

	~FileManager()
	{
		// inicializa la operacion para llamar al destructor en el servidor
		filemanagerOp op = destructorOp;

		// inicializa rpcs
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		// empaqeta la operacion
		pack(rpcOut, op);
		// envia el parquete al servidor
		sendMSG(serverConnection.serverId, rpcOut);
		// recibe el paquete del servidor
		recvMSG(serverConnection.serverId, rpcIn);
		// comprueba el ok
		if (rpcIn[0] != MSG_OK) // si no es ok salta error
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}

		// cierra la conexion
		closeConnection(serverConnection.serverId);
	}

	void readFile(char *fileName, char *&data, unsigned long int &dataLength)
	{
		// inicializa la operacion para llamar al readFile en el servidor
		filemanagerOp op = readFileOp;

		// inicializa rpcs
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		pack(rpcOut, op);
		unsigned int s = strlen(fileName);
		pack(rpcOut, s);
		packv(rpcOut, fileName, s);

		// envia el parquete al servidor
		sendMSG(serverConnection.serverId, rpcOut);
		// recibe el paquete del servidor
		recvMSG(serverConnection.serverId, rpcIn);

		// comprueba el ok
		unsigned char ok = unpack<unsigned char>(rpcIn);
		if (ok != MSG_OK) // si no es ok salta error
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}
		else // si es ok:
		{
			dataLength = unpack<unsigned long int>(rpcIn);
			data = new char[dataLength];
			unpackv(rpcIn, data, dataLength);
		}
	}

	void writeFile(char *fileName, char *&data, unsigned long int dataLength)
	{
		// inicializa la operacion para llamar al writeFile en el servidor
		filemanagerOp op = writeFileOp;

		// inicializa rpcs
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		pack(rpcOut, op);
		int s = strlen(fileName);
		pack(rpcOut, s);
		packv(rpcOut, fileName, s);
		pack(rpcOut, dataLength);
		packv(rpcOut, data, dataLength);

		// envia el parquete al servidor
		sendMSG(serverConnection.serverId, rpcOut);
		// recibe el paquete del servidor
		recvMSG(serverConnection.serverId, rpcIn);

		// comprueba el ok
		unsigned char ok = unpack<unsigned char>(rpcIn);
		if (ok != MSG_OK) // si no es ok salta error
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}
	}

	std::vector<std::string *> *listFiles()
	{

		// inicializa la operacion para llamar al listFiles en el servidor
		filemanagerOp op = listFilesOp;

		// inicializa rpcs
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		pack(rpcOut, op);

		// envia el parquete al servidor
		sendMSG(serverConnection.serverId, rpcOut);

		// recibe el paquete del servidor
		recvMSG(serverConnection.serverId, rpcIn);
		// comprueba el ok
		unsigned char ok = unpack<unsigned char>(rpcIn);
		if (ok != MSG_OK) // si no es ok salta error
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}
		else // si es ok:
		{
			unsigned long int ls = unpack<unsigned long int>(rpcIn);
			std::vector<std::string *> *flist = new std::vector<std::string *>();
			for (int i = 0; i < ls; ++i)
			{
				int s = unpack<int>(rpcIn);
				char *name = new char[s];
				unpackv(rpcIn, name, s);
				flist->push_back(new std::string(name));
				delete[] name;
			}
			return flist;
		}
		return nullptr;
	}
};
