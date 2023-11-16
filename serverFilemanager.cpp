#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"

#include <thread>
#include "filemanager_imp.h"


void atiendeCliente(int clientId)
{
	//crear interfaz cliente
	fileManager_imp imp=fileManager_imp(clientId);
	//repetir
	do{
		//atiende operación
		imp.recibeOp();
	}while(!imp.connectionClosed());
	//mientras no cierre conexión
	
	
}


int main(int argc, char** argv)
{
	//iniciar un servidor
    auto serverSocket=initServer(60000);
    while(1){
		//esperar conexiones en puerto
         while(!checkClient()){
             usleep(1000);
        }
		int clientId=getLastClientID();
		std::thread *th=new std::thread(atiendeCliente,clientId);
	}
	close(serverSocket);
	return 0;
}




