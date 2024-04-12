#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define STORAGE_ID "/OBJ_MEM_COM"
#define STORAGE_SIZE sizeof(miEstructura)

typedef struct miEstructura
{
	int num_men;
	char men[50];
} miEstructura;

int main(int argc, char *argv[])
{
	int fd;
	pid_t pid;
	miEstructura miest;
	pid = getpid(); // Obtener el ID de proceso actual

	// Abrir un objeto de memoria compartida en modo de solo lectura
	fd = shm_open(STORAGE_ID, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		return 10;
	}

	// Mapear la memoria compartida en el espacio de direcciones del proceso
	miEstructura *addr = mmap(NULL, STORAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
	{
		perror("mmap");
		return 30;
	}
	// Leer el mensaje
	int numMnsj = *(int *)addr;
	if (numMnsj > 0)
	{
		numMnsj -= 1; // Decrementar numMnsj antes de leer el mensaje
		memcpy(&miest, addr + sizeof(int) + numMnsj * sizeof(miEstructura), sizeof(miEstructura));
		printf("Leí de memoria compartida: \"%s\"\n", miest.men);

		// Eliminar el mensaje
		miest.num_men = 0;
		memset(miest.men, 0, sizeof(miest.men));
		memcpy(addr + sizeof(int) + numMnsj * sizeof(miEstructura), &miest, sizeof(miEstructura));
		*(int *)addr = numMnsj; // Actualizar el valor de numMnsj en la memoria compartida
		printf("\nPID %d: \n", pid);
	}
	else
	{
		printf("No hay mensajes en la memoria compartida.\n");
	}

	return 0;
}
