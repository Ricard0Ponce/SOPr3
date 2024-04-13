/*Programa que permite eliminar una conversacion por completo*/
#include <stdio.h>
#include <dirent.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define STORAGE_SIZE 128

int main(int argc, char *argv[])
{
    char nomConv[50];
    printf("\nDigite el nombre de la conversación a eliminar: \n");
    scanf("%s", nomConv);

    DIR *ruta = opendir("/dev/shm");
    if (ruta == NULL)
    {
        perror("opendir");
        return 1;
    }

    struct dirent *acceso;
    int found = 0;
    while ((acceso = readdir(ruta)) != NULL)
    {
        if (strcmp(acceso->d_name, nomConv) == 0)
        {
            found = 1;
            // Formar el path completo del archivo en memoria compartida
            char rutaF[256] = "/dev/shm/";
            strcat(rutaF, nomConv);

            // Obtener el descriptor del archivo de memoria compartida
            int fd = open(rutaF, O_RDWR);
            if (fd == -1)
            {
                perror("open");
                return 1;
            }
            // Cerrar el descriptor del archivo de memoria compartida
            close(fd);

            // Cerrar el objeto de memoria compartida
            int res = shm_unlink(nomConv);
            if (res == -1)
            {
                printf("unlink: %s\n", strerror(errno));
                return 100;
            }
            printf("Se encontró y eliminó la conversación %s\n", nomConv);
            break; // Salir del bucle una vez que se haya encontrado el elemento
        }
    }

    if (!found)
    {
        printf("No se encontró la conversación %s\n", nomConv);
    }

    closedir(ruta);

    return 0;
}
