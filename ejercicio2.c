#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#define N 100000

int main(int argc, char **argv){
    // Iniciando programa de MPI
    MPI_Init(&argc, &argv);
    // Manejador del grupo global
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); /*Para obtener el ID*/
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); /*Para obtener el número de procesos*/

    // Soy el master al inicio
    int master = world_rank;

    // Enviar mensaje a los procesos con mayor iRank
    for (int i = world_rank + 1; i < world_size; i++)
        MPI_Send(&world_rank, 1, MPI_INT, i, 1, MPI_COMM_WORLD);

    // Recibo mensaje de los procesos con menor IRank
    MPI_Request request[world_rank];
    int message;
    for (int i = 0; i < world_rank; i++)
        MPI_Irecv(&message, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request[i]);
    // Recibo mensaje de respuesta
    // De procesos con mayor iRank
    int messages[world_size - world_rank - 1];
    MPI_Request request2[world_size - world_rank - 1];
    for (int i = 0; i < world_size - world_rank - 1; i++)
        MPI_Irecv(&messages[i], 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &request2[i]);

    MPI_Barrier(MPI_COMM_WORLD);
    // Gastando tiempo
    int a;
    for (int i = 0; i < 10000000; i += 2)
        a++;
    MPI_Barrier(MPI_COMM_WORLD);

    // Contestando con mi iRank
    // A procesos menores
    for (int i = 0; i < world_rank; i++){
        int flag = 0;
        // Probando si llego mensaje
        MPI_Test(&request[i], &flag, MPI_STATUS_IGNORE);
        if (flag)
            MPI_Send(&world_rank, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
    }
    int b;
    for (int i = 0; i < 10000000; i += 2)
        b++;
    MPI_Barrier(MPI_COMM_WORLD);

    // Revisando mensajes
    // De procesos mayores
    for (int i = 0; i < world_size - world_rank - 1; i++){
        int flag = 0;
        // Probando si llego mensaje
        MPI_Test(&request2[i], &flag, MPI_STATUS_IGNORE);
        if (flag)
            if (master < messages[i])
                master = messages[i];
    }
    printf("Proceso %d el master es %d\n", world_rank, master);

    MPI_Finalize();
    return 0;
}
