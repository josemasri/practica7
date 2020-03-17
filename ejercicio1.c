#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>
#define N 30

int main(int argc, char **argv)
{
    // Iniciando programa de MPI
    MPI_Init(&argc, &argv);
    // Manejador del grupo global
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); /*Para obtener el ID*/
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); /*Para obtener el número de procesos*/
    double p, q;

    // Inicializando semilla de números aleatorios
    srand(time(NULL) + world_rank);
    // Inicializando reloj
    int clock = 0;
    int clock_message;

    MPI_Request request[N];
    int flag;
    for (int i = 0; i < N; i++)
    {
        // Generando numero aleatorio
        p = (double)rand() / (double)RAND_MAX;
        // Incrtementando reloj
        if (p < 0.05){
            printf("[%d] evento con contador %d\n",world_rank,clock);
            clock++;
        }
        // Recibiendo mensaje de forma asincrona
        MPI_Irecv(&clock_message, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request[i]);
        // Evaluando si se envía mensaje
        q = (double)rand() / (double)RAND_MAX;
        if (q < 0.1)
        {
            // Enviaré mensaje con mi reloj
            int destination;
            do
            {
                destination = rand() % world_size;
            } while (destination == world_rank);

            MPI_Send(&clock, 1, MPI_DOUBLE, destination, 1, MPI_COMM_WORLD);
        }


        
        MPI_Test(&request[i], &flag, MPI_STATUS_IGNORE);
        MPI_Barrier(MPI_COMM_WORLD);
        if (flag)
        {
            if (clock < clock_message)
            {
                // El mensaje remoto ocurrio despues
                clock = clock_message + 1;
            }
            else
            {
                // Mi mensaje ocurrio despues
                clock++;
            }
            printf("[%d] recepción de mensaje %d\n", world_rank, clock);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("[%d] reloj: %d\n", world_rank, clock);
    MPI_Finalize();
    return 0;
}
