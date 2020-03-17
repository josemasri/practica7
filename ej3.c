#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#define N 100000


int main(int argc, char **argv)
{
    // Iniciando programa de MPI
    int nodoInicial=0;
    MPI_Init(&argc, &argv);
    // Manejador del grupo global
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); /*Para obtener el ID*/
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); /*Para obtener el número de procesos*/
    MPI_Barrier(MPI_COMM_WORLD);
    printf("%d\n",world_size);
    MPI_Barrier(MPI_COMM_WORLD);

    int arrayMaestros[world_size];

    //mensaje de elección
    if (world_rank == nodoInicial)
    {
        // Inicializo el arreglo en -1
        for (int i = 0; i < world_size; i++)
        {
            if (i == world_rank)
                arrayMaestros[i] = world_rank;
            else
                arrayMaestros[i] = -1;
        }


        int recibido = 0;
        int destino = world_rank + 1;
        MPI_Request request;
        MPI_Status status;
        int mensaje;
        //mensaje de ok
        MPI_Irecv(&mensaje, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &request);
        
        

        while (!recibido)
        {
            // Envio un mensaje con mi ID
            // En el arreglo
            // Para ser el master
            MPI_Send(&arrayMaestros[0], world_size, MPI_INT, destino, 1, MPI_COMM_WORLD);
            // Espero retroalimentación para ver si le llego
            for(int k=0;k<100000000;k++);
            // Esperando para ver si responde
            int flag = 0;
            // Probando si llego mensaje
            MPI_Test(&request, &flag, &status);
            if (flag)
            {
                // El proceso recibió el menasaje
                // Ya puedo seguir
                recibido = 1;
            }
            // Envio al siguiente nodo
            destino++;
            if (destino == world_size)
            {
                destino = world_rank + 1;
                // printf("Ningún proceso recibió mi mensaje, vuelvo a empezar\n");
            }
        }
        printf("EL proceso %d recibio mi mensaje\n", status.MPI_SOURCE);
    }
    else if (world_rank < world_size - 1)
        
    {
        // Recibo mensaje de algún nodo para comenzar
        MPI_Request request;
        MPI_Status status;
        MPI_Irecv(&arrayMaestros[0], world_size, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        printf("Soy %d y recibí el mensaje de %d\n", world_rank, status.MPI_SOURCE);
        for (int j = 0; j < world_size; j++)
            printf("%d ", arrayMaestros[j]);
        printf("\n");
        arrayMaestros[world_rank] = world_rank;
        // Le contesto de recibido
        int message = 1;
        MPI_Send(&message, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
        int recibido = 0;
        int destino = world_rank + 1;
        MPI_Request request2;
        MPI_Status status2;
        int mensaje;
        MPI_Irecv(&mensaje, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &request2);
        while (!recibido)
        {
            // Envio un mensaje con mi ID
            // En el arreglo
            // Para ser el master
            MPI_Send(&arrayMaestros[0], world_size, MPI_INT, destino, 1, MPI_COMM_WORLD);
            // Espero retroalimentación para ver si le llego
            sleep(10);
            // Esperando para ver si responde
            int flag = 0;
            // Probando si llego mensaje
            MPI_Test(&request2, &flag, &status2);
            if (flag)
            {
                // El proceso recibió el menasaje
                // Ya puedo seguir
                recibido = 1;
            }
            // Envio al siguiente nodo
            destino++;
            if (destino == world_size)
            {
                destino = world_rank + 1;
                // printf("Ningún proceso recibió mi mensaje, vuelvo a empezar\n");
            }
        }
        printf("El proceso %d recibio mi mensaje\n", status2.MPI_SOURCE);
    }
    else
    {
        // Último proceso
        // Recibo mensaje de algún nodo para comenzar
        MPI_Request request;
        MPI_Status status;
        MPI_Irecv(&arrayMaestros[0], world_size, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        printf("Soy %d y recibí el mensaje de %d\n", world_rank, status.MPI_SOURCE);
        for (int j = 0; j < world_size; j++)
            printf("%d ", arrayMaestros[j]);
        printf("\n");
        arrayMaestros[world_rank] = world_rank;
        // Le contesto de recibido
        int message = 1;
        MPI_Send(&message, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
        // Escojer padre
        int recibido = 0;
        int destino = world_rank + 1;
        MPI_Request request2;
        MPI_Status status2;
        int mensaje;
        MPI_Irecv(&mensaje, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &request2);
        while (!recibido)
        {
            // Envio un mensaje con mi ID
            // En el arreglo
            // Para ser el master
            MPI_Send(&arrayMaestros[0], world_size, MPI_INT, destino, 1, MPI_COMM_WORLD);
            // Espero retroalimentación para ver si le llego
            sleep(10);
            // Esperando para ver si responde
            int flag = 0;
            // Probando si llego mensaje
            MPI_Test(&request2, &flag, &status2);
            if (flag)
            {
                // El proceso recibió el menasaje
                // Ya puedo seguir
                recibido = 1;
            }
            // Envio al siguiente nodo
            destino++;
            if (destino == world_size)
            {
                destino = world_rank + 1;
                // printf("Ningún proceso recibió mi mensaje, vuelvo a empezar\n");
            }
        }
        printf("El proceso %d recibio mi mensaje\n", status2.MPI_SOURCE);
    }

    //printf("leader: %d\n");

    MPI_Finalize();
    return 0;
}

void send_array(int world_size, int world_rank, int tag1, int tag2)
{
    int arrayMaestros[world_size];
    // Recibo mensaje de algún nodo para comenzar
    MPI_Request request;
    MPI_Status status;
    MPI_Irecv(&arrayMaestros[0], world_size, MPI_INT, MPI_ANY_SOURCE, tag1, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    printf("Soy %d y recibí el mensaje de %d\n", world_rank, status.MPI_SOURCE);
    for (int j = 0; j < world_size; j++)
        printf("%d ", arrayMaestros[j]);
    printf("\n");
    arrayMaestros[world_rank] = world_rank;
    // Le contesto de recibido
    int message = 1;
    MPI_Send(&message, 1, MPI_INT, status.MPI_SOURCE, tag2, MPI_COMM_WORLD);
    int recibido = 0;
    int destino = world_rank + 1;
    MPI_Request request2;
    MPI_Status status2;
    int mensaje;
    MPI_Irecv(&mensaje, 1, MPI_INT, MPI_ANY_SOURCE, tag2, MPI_COMM_WORLD, &request2);
    while (!recibido)
    {
        // Envio un mensaje con mi ID
        // En el arreglo
        // Para ser el master
        MPI_Send(&arrayMaestros[0], world_size, MPI_INT, destino, 1, MPI_COMM_WORLD);
        // Espero retroalimentación para ver si le llego
        sleep(10);
        // Esperando para ver si responde
        int flag = 0;
        // Probando si llego mensaje
        MPI_Test(&request2, &flag, &status2);
        if (flag)
        {
            // El proceso recibió el menasaje
            // Ya puedo seguir
            recibido = 1;
        }
        // Envio al siguiente nodo
        destino++;
        if (destino == world_size)
        {
            destino = world_rank + 1;
            // printf("Ningún proceso recibió mi mensaje, vuelvo a empezar\n");
        }
    }
    printf("El proceso %d recibio mi mensaje\n", status2.MPI_SOURCE);
}