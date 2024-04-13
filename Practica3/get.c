#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

// #define STORAGE_ID "/HolaM"
#define STORAGE_SIZE sizeof(miEstructura)

typedef struct miEstructura
{
	int num_men;
	char men[50];
} miEstructura;

int main(int argc, char *argv[])
{
	printf("Accediendo a lectura de mensajes...\n");
	char STORAGE_ID[50];
	int fd;
	pid_t pid;
	miEstructura miest;
	pid = getpid(); // Obtener el ID de proceso actual
	struct dirent *acceso;
	DIR *ruta = opendir("/dev/shm/"); // Abre el directorio /dev/shm
	if (ruta == NULL)
	{ // opendir devuelve NULL si no puede abrir el directorio
		printf("No se puede abrir el directorio actual\n");
		return 1;
	}

	printf("\nPara comenzar seleccione una conversación de la siguiente lista anterior: \n");
	//Imprime lo que se encontro en el directorio
	while ((acceso = readdir(ruta)) != NULL)
	{
		printf("%s\n", acceso->d_name);
	}

	printf("\nIngrese el nombre de la conversación: ");
	fgets(STORAGE_ID, sizeof(STORAGE_ID), stdin);

	STORAGE_ID[strcspn(STORAGE_ID, "\n")] = '\0'; // Elimina el salto de línea si fgets lo lee
	int encontrado = 0;

	rewinddir(ruta); // Vuelve al principio del directorio

	while ((acceso = readdir(ruta)) != NULL)
	{
		// Comparamos si las candenas son iguales
		if (strcmp(acceso->d_name, STORAGE_ID) == 0)
		{
			encontrado = 1;
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

			int numMnsj = *(int *)addr;
			if (numMnsj > 0)
			{
				// Calcular la posición del primer mensaje
				int mensaje = sizeof(int);
				// Leer el primer mensaje
				memcpy(&miest, addr + mensaje, sizeof(miEstructura));
				printf("Leí de memoria compartida: \"%s\"\n", miest.men);

				// Desplazar todos los mensajes restantes hacia abajo
				for (int i = 1; i < numMnsj; i++)
				{
					memcpy(addr + sizeof(int) + (i - 1) * sizeof(miEstructura), addr + sizeof(int) + i * sizeof(miEstructura), sizeof(miEstructura));
				}

				// Decrementar numMnsj después de leer y eliminar el mensaje
				*(int *)addr = numMnsj - 1; // Actualizar el valor de numMnsj en la memoria compartida
			}
			else
			{
				printf("No hay mensajes en la memoria compartida.\n");
				*(int *)addr = 0; // Como ya no hay mensajes reiniciamos para poder acceder de nuevo
			}
		}
	}
	if (!encontrado)
	{
		printf("No se encontró la conversación %s\n", STORAGE_ID);
	}

	closedir(ruta); // Cierra el directorio

	return 0;
}
