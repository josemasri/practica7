#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#define N 100000

void printArr(int* Arr,int n,int a){
  printf("[%d]: ",a);
  for(int i=0;i<n;i++) printf("%d ",Arr[i]);
  printf("\n");
}
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
  MPI_Request request;
  MPI_Status status;
  int arrayMaestros[world_size];
  int isDone=-1;
  int lider=-1;
  int message;
  for(int i=0;i<world_size;i++) arrayMaestros[i]=-1;
  
    while(isDone==-1){

    int destino=(world_rank+1)%world_size;
    int recibido = 0;
    
    if (world_rank!=nodoInicial){
      MPI_Irecv(arrayMaestros, world_size, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);
      message=-1;
      
      //ack
      MPI_Send(&message, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
    }
   
    //mensaje de ok
    MPI_Irecv(&message, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &request);
    while (!recibido){
      // Envio un mensaje con mi ID
      // En el arreglo
      // Para ser el master
      arrayMaestros[world_rank]=world_rank;
      printf("[%d] Destino: %d\n",world_rank, destino);
      MPI_Send(arrayMaestros, world_size, MPI_INT, destino, 1, MPI_COMM_WORLD);
      // Espero retroalimentación para ver si le llego
      sleep(2);
        // Esperando para ver si responde
        int flag = 0;
        // Probando si llego mensaje
        MPI_Test(&request, &flag, &status);
        if (flag){
          // El proceso recibió el menasaje
          // Ya puedo seguir
          //printf("nodo %d respondio a %d\n ",status.MPI_SOURCE, world_rank);

          isDone=message;
          recibido = 1;
        }else{
          printf("nodo %d no respondio a %d\n ",destino, world_rank);
        }
        // Envio al siguiente nodo
        destino++;
        if (destino == world_size)
        {
          destino =0;
          // printf("Ningún proceso recibió mi mensaje, vuelvo a empezar\n");
        }
      }
      if (world_rank==nodoInicial){
        MPI_Irecv(arrayMaestros, world_size, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);

        for (int j = 0; j < world_size; j++){
          if(arrayMaestros[j]==world_rank){
            isDone=world_rank;
          }
        }
        MPI_Send(&message, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
      }
      else {
        isDone=-2;
        int message=isDone;
        
        MPI_Send(&message, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
    }
    }

    if(world_rank==nodoInicial){
      lider=world_rank;
      for(int j=world_rank+1;j<world_size;j++){
          if(arrayMaestros[j]!=-1){
            int destino=j;
            MPI_Send(&lider, 1, MPI_INT, destino, 3, MPI_COMM_WORLD);
            MPI_Send(arrayMaestros, world_size, MPI_INT, destino, 4, MPI_COMM_WORLD);
            break;
          }
        }
    }
    else{
      MPI_Recv(arrayMaestros, world_size, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
      MPI_Recv(&lider, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
      for(int j=world_rank+1;j<world_size;j++){
          if(arrayMaestros[j]!=-1){
            int destino=j;
            MPI_Send(&lider, 1, MPI_INT, destino, 3, MPI_COMM_WORLD);
            MPI_Send(arrayMaestros, world_size, MPI_INT, destino, 4, MPI_COMM_WORLD);
            
            break;
          }
      }
    }
    //MPI_Send(&message, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
        

MPI_Barrier(MPI_COMM_WORLD);
  printArr(arrayMaestros,world_size,world_rank);
  MPI_Barrier(MPI_COMM_WORLD);
    printf("[%d]leader: %d\n",world_rank,lider);

  MPI_Barrier(MPI_COMM_WORLD);
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