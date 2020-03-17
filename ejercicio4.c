#include "functions.h"
#ifndef STDIO_h
#include <stdio.h>
#endif
#include <time.h>
#define NUM_NEIGHBORS 3
#define INICIADOR 0
#define CAPACIDAD_MAX 10

int main(int argc, char **argv)
{
    // Iniciando programa de MPI
    MPI_Init(&argc, &argv);
    // Manejador del grupo global
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); /*Para obtener el ID*/
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); /*Para obtener el número de procesos*/
    // Inicializando semilla de números aleatorios
    srand(time(NULL) + world_rank);
    int capacidad = rand() % CAPACIDAD_MAX;
    // Obtengo mis vecinos
    int *neighbors = getNeighbors2(world_rank, NUM_NEIGHBORS);
    printf("Soy el nodo %d, mi capacidad es %d y mis vecinos son ", world_rank, capacidad);
    for (int i = 0; i < NUM_NEIGHBORS; i++)
        printf("%d ", neighbors[i]);
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);

    // Código para proceso INICIADOR
    // Inicia inundación
    if (world_rank == INICIADOR)
    {
        int despierta = 1;
        // Envió mensaje (capacideddes) a mis vecinos (Despierta)
        for (int i = 0; i < NUM_NEIGHBORS; i++)
            MPI_Send(&despierta, 1, MPI_INT, neighbors[i], 1, MPI_COMM_WORLD);

        // Envió mensaje de retroalimentacion a mis vecinos
        int miAcuse[2];
        miAcuse[0] = world_rank;
        miAcuse[1] = capacidad;
        for (int i = 0; i < NUM_NEIGHBORS; i++)
            MPI_Send(&miAcuse, 2, MPI_INT, neighbors[i], 2, MPI_COMM_WORLD);
        // Espero Recibir mensajes de retroalimentacion
        // De todos mis vecinos
        int acuses[NUM_NEIGHBORS][2];
        for (int i = 0; i < NUM_NEIGHBORS; i++)
            MPI_Recv(&acuses[i], 2, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Elegir nodo con mayor capacidad
        int lider[2];
        // [id, capacidad]
        lider[0] = miAcuse[0];
        lider[1] = miAcuse[1];
        for (int i = 0; i < NUM_NEIGHBORS; i++)
            if (lider[1] < acuses[i][1])
            {
                lider[0] = acuses[i][0];
                lider[1] = acuses[i][1];
            }

        // Enviar menasaje con lider
        for (int i = 0; i < world_size; i++)
            if (i != world_rank)
                MPI_Send(&lider[0], 1, MPI_INT, i, 3, MPI_COMM_WORLD);
        
        if (lider[0] == world_rank)
            printf("Soy el nodo %d. Soy el coordinador\n", world_rank);
        else
            printf("Soy el nodo %d y el coordinador es %d\n", world_rank, lider[0]);
    }

    // Código para procesos normales
    else
    {
        int despertar;
        // Recibo mensaje de mi padre (despertar)
        MPI_Status status;
        MPI_Recv(&despertar, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        int padre = status.MPI_SOURCE;
        // Envió mensajes a mis vecinos (Despierto)
        for (int i = 0; i < NUM_NEIGHBORS; i++)
            MPI_Send(&despertar, 1, MPI_INT, neighbors[i], 1, MPI_COMM_WORLD);

        int miAcuse[2];
        miAcuse[0] = world_rank;
        miAcuse[1] = capacidad;
        // Envio mensajes de retroalimentacion a mis vecinos excepto a mi padre
        for (int i = 0; i < NUM_NEIGHBORS; i++)
            if (neighbors[i] != padre)
                MPI_Send(&miAcuse, 2, MPI_INT, neighbors[i], 2, MPI_COMM_WORLD);

        // Recibo mensajes de retroalimentación
        int acuses[NUM_NEIGHBORS][2];
        for (int i = 0; i < NUM_NEIGHBORS; i++)
            MPI_Recv(&acuses[i][0], 2, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Elegir nodo con mayor capacidad
        int lider[2];
        lider[0] = miAcuse[0];
        lider[1] = miAcuse[1];
        for (int i = 0; i < NUM_NEIGHBORS; i++)
            if (lider[1] < acuses[i][1])
            {
                lider[0] = acuses[i][0];
                lider[1] = acuses[i][1];
            }

        // Envio retoalimentacion a mi padre
        MPI_Send(&lider, 2, MPI_INT, padre, 2, MPI_COMM_WORLD);
        // Recibir mensaje de coordinador si se envia
        int liderFinal;
        MPI_Status status2;
        MPI_Recv(&liderFinal, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status2);
        if (liderFinal == world_rank)
            printf("Soy el nodo %d y soy el lider\n", world_rank);
        else
            printf("Soy el nodo %d y el lider es %d\n", world_rank, liderFinal);
    }

    MPI_Finalize();
    return 0;
}
