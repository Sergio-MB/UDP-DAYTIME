# UDP-DAYTIME
Servidor iterativo y cliente de protocolo DAYTIME utilizando sockets UDP escritos en C. Proyecto desarrollado con fines de aprendizaje en Arquitectura de Redes y Servicios, asignatura de Ingeniería de Software en la Universidad de Valladolid

## Requisitos
- [Docker](https://www.docker.com/) running on your machine.

## Desarrollo

Primero, configure el entorno:
```bash
git clone https://github.com/Sergio-MB/UDP-DAYTIME # Clonar repositorio
cd UDP-DAYTIME # Accedemos a dicho repositorio
docker run --name=ubuntu-daytime-udp -it -v $(PWD):/home ubuntu # Run Ubuntu container sharing repository folder

# Dentro del contenedor. Obtenga las herramientas del compilador de C
apt-get update
apt-get install build-essential
```

Para las próximas veces, teniendo el contenedor ya en mi máquina, ¿cómo implementar el entorno local?
```bash
docker start ubuntu-daytime-udp
docker exec -it ubuntu-daytime-udp bash
```

Compilar cliente y servidor:
```bash
# Dentro del contenedor
cd /home
gcc -Wall -o client.out daytime-udp-client-Munumer-Blazquez.c
gcc -Wall -o server.out daytime-udp-server-Munumer-Blazquez.c
```

Ejecutar server:
```bash
# Dentro del contenedor
./server.out [port-number] # port-number is optional
```

Para ejecutar el cliente necesitamos otra terminal:
```bash
docker exec -it ubuntu-daytime-udp bash

# Dentro del contenedor
cd /home
./client.out [server.IP.address] [port-number] # server.IP.address obligatorio, port-number opcional
```
