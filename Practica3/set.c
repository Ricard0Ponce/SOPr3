/*Programa que permite escribir un mensaje en una conversacion*/
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define STORAGE_ID "/OBJ_MEM_COM" // Nombre de la conversacion va acá
// #define STORAGE_SIZE 200		  // sizeof(struct {int num_men; char men[50]; })
#define MAX_MENSAJES 3
#define STORAGE_SIZE 250

typedef struct miEstructura
{
	int num_men;
	char men[50];
	// int numeroDeMensaje
} miEstructura;
char nombreConversacion[50];
int main(int argc, char *argv[])
{
	miEstructura miest; // Creo la estructura desde el main para poder usarla
	printf("Accediendo a escritura de mensaje\n");
	printf("\nPara comenzar seleccione una conversación de la siguiente lista: \n");
	printf("\n");					 // Agregar la lista de las conversaciones
	scanf("%s", nombreConversacion); // Luego seleccionar una conversacion
	getchar();						 // Consumir la nueva línea que queda en el búfer
	printf("\nIngreso a la conversacion, escriba el mensaje que quiere enviar: \n");
	fgets(miest.men, sizeof(miest.men), stdin); // Hago que el usuario escriba el mensa
	int res;									// Variable para almacenar resultados de funciones
	int fd;										// Descriptor de archivo para el objeto de memoria compartida
	pid_t pid;									// ID de proceso actual
	// void *addr;									// Dirección de memoria mapeada
	int numMnsj = 0;

	// Obtiene el ID de proceso actual
	pid = getpid();

	// Abre un objeto de memoria compartida (NO un archivo)
	// STORAGE_ID: Identificador de la conversación.
	fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		return 10;
	}

	// Aumenta el tamaño del objeto de memoria compartida (por defecto es 0)
	res = ftruncate(fd, STORAGE_SIZE);
	if (res == -1)
	{
		perror("ftruncate");
		return 20;
	}

	// Mapea la memoria del objeto en el espacio de direcciones del proceso
	// STORAGE_SIZE tiene dentro el mensaje
	// Antes el segundo parametro era STORAGE_SIZE
	/*
	addr = mmap(NULL, STORAGE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
	{
		perror("mmap");
		return 30;
	}
	*/
	// struct mensaje *miest = mmap(NULL, STORAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	// Mapea la memoria del objeto en el espacio de direcciones del proceso
	miEstructura *comp = mmap(NULL, STORAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (comp == MAP_FAILED)
	{
		perror("mmap");
		return 30;
	}

	// Get the number of messages
	numMnsj = *(int *)comp;
	printf("\nHay %d mensajes por el momento\n", numMnsj);
	// Incrementa el contador de mensajes
	// Copia la estructura en la memoria compartida
	memcpy(comp + sizeof(int) + numMnsj * sizeof(miEstructura), &miest, sizeof(miEstructura));
	*(int *)comp += 1; // Aqui se aumentan la cantidad de mensajes que hay
	// Copia la estructura en la memoria compartida
	// memcpy(addr, &miest, sizeof(miest));

	// Espera a que otro proceso lea la memoria compartida
	// sleep(10);
	/*
	ESTO ESTABA AQUÍ ANTES
	res = munmap(addr, STORAGE_SIZE);
	if (res == -1)
	{
		perror("munmap");
		return 40;
	}

	*/
	// Libera el mapeo de memoria

	return 0;
}
