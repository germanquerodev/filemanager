# FileManager
Es un ejercicio para clase de Programacion de sistemas distribuidos.
Se trata de una clase distribuida en C++
## Utilizacion:
La ip de conexion se encuentra en "filemanager_stub.h" en la clase multMatrix como la variable privada std::string ip
Poner la ip privada del servidor.<br>
En el servidor, clonar el repo y dentro de su directorio hay que ejecutar:
```sh
cmake .
make
./serverFilemanager
```
En el cliente enviar el ejecutable clientFilemanager y ejecutarlo:
```sh
./clientFilemanager
```
Si se quiere cambiar la secuencia de acciones que debe de realizar el sistema cambiarlo en el main del cliente en "clientFilemanager.cpp"