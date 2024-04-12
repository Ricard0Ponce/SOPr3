#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int opcion=0; //pag 184
    while (opcion != 5)
    {
        
        //int n = atoi(argv[1]), i;
        printf("\n1.- Digite la opcion que desea ejecutar: \n");
        printf("\n2.- Escribir un mensaje\n");   //
        printf("\n3.- Leer un mensaje\n");       //
        printf("\n4.- Borrar conversaciones\n"); //
        printf("\n5.- Salir.\n");                //
        scanf("%d",&opcion);
        switch (opcion)
        {
        case 1:
            // pid_t childpid;
            //exec
            break;
        case 2:
            /* Aqui creo el exec de set */
            break;
        case 3:
            /* Aqui creo el exec de set */
            break;
        case 4:
            /* Aqui creo el exec de set */
            break;
        case 5:
            /* Aqui creo el exec de set */
            break;
        case 6:
            printf("\nSaliendo del programa...\n");
            return 0;
            break;
        default:
            printf("\nA elegido una opcion que no esta en el menú.\n");
            break;
        }
    }

    return 0;
}

/// NOTA: Cada que se lee un mensaje libera un espacio
// NOTA: SE LEEN LOS MENSAJES EN EL ORDEN EN QUE SE DIERAN
// LA ESTRUCTURA: Puede que los espacios de memoria cambien en cada ejecución.
// NO usar listas ligadas
// Lo más facil es usar un arreglo.
// APUNTADOR A DIRECCION DE MEMORIA NO VA A SERVIR AQUÍ
// BORRA DEBERÍA BORRAR TODA LA CONVERSACION