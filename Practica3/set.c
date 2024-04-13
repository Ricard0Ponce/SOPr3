#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define MAX_MENSAJES 3
#define STORAGE_SIZE 250

typedef struct miEstructura
{
	int num_men;
	char men[50];
} miEstructura;

int main(int argc, char *argv[])
{
	char STORAGE_ID[50];
	int fd;
	int numMnsj = 0;
	struct dirent *acceso;
	miEstructura miest;

	printf("Accediendo a escritura de mensaje\n");

	DIR *ruta = opendir("/dev/shm/"); // Abre el directorio /dev/shm
	if (ruta == NULL)
	{ // opendir devuelve NULL si no puede abrir el directorio
		printf("No se puede abrir el directorio actual\n");
		return 1;
	}

	printf("\nPara comenzar seleccione una conversación de la siguiente lista anterior: \n");
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
		if (strcmp(acceso->d_name, STORAGE_ID) == 0)
		{
			encontrado = 1; // Indicamos que se encontro, esto rompe el while
			printf("\nIngreso a la conversacion, escriba el mensaje que quiere enviar: \n");
			fgets(miest.men, sizeof(miest.men), stdin); // Hago que el usuario escriba el mensaje

			// Abre un objeto de memoria compartida (NO un archivo)
			fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

			if (fd == -1)
			{
				perror("open");
				return 10;
			}

			// Aumenta el tamaño del objeto de memoria compartida (por defecto es 0)
			int res = ftruncate(fd, STORAGE_SIZE);

			if (res == -1)
			{
				perror("ftruncate");
				return 20;
			}

			// Mapea la memoria del objeto en el espacio de direcciones del proceso
			miEstructura *comp = mmap(NULL, STORAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
			if (comp == MAP_FAILED)
			{
				perror("mmap");
				return 30;
			}

			numMnsj = *(int *)comp;
			memcpy(comp + sizeof(int) + numMnsj * sizeof(miEstructura), &miest, sizeof(miEstructura));
			*(int *)comp += 1; // Aqui se aumentan la cantidad de mensajes que hay
		}
	}
	// Indicamos que no fue encontrado
	if (!encontrado)
	{
		printf("No se encontró la conversación %s\n", STORAGE_ID);
	}

	closedir(ruta); // Cierra el directorio
	return 0;
}
