/*Programa que permite crear un espacio para una conversacion */
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// #define STORAGE_ID "/OBJ_MEM_COM"
// #define STORAGE_SIZE sizeof(struct {int num_men; char men[50]; })
#define STORAGE_SIZE 200
// Clase que me permite crear una conversacion
int main(int argc, char *argv[])
{
    printf("\nEscribe el nombre de la conversacion \n");
    char nomConv[50];
    scanf("%s", &nomConv);
    // Abre un objeto de memoria compartida (NO un archivo)
    int fd = shm_open(nomConv, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return 10;
    }
    printf("\nSe creo la convesacion llamada: %s con espacio de memoria de: %d\n", nomConv, STORAGE_SIZE);
    return 0;

    // NOTA: Esto crea la conversacion pero NO la elimina tras ejecución.
    //  Agregar una clase para eliminar la conversacion completa.
}