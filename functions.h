#include <mpi.h>
#include <stdlib.h>

int createGroup(MPI_Group globalGroupManager, int numberOfElements, int *processArr, MPI_Group *newGroup)
{
    return MPI_Group_incl(globalGroupManager, numberOfElements, processArr, newGroup);
}

int createCommunicator(MPI_Comm globalCommunicator, MPI_Group group, MPI_Comm *newcomm)
{
    return MPI_Comm_create(globalCommunicator, group, newcomm);
}

int *getNeighbors(int iRank, int numNeighbors)
{
    int *neighbors = (int *)calloc(numNeighbors, sizeof(int));
    switch (iRank)
    {
    case 0:
        neighbors[0] = 2;
        neighbors[1] = 4;
        break;
    case 1:
        neighbors[0] = 6;
        neighbors[1] = 8;
        break;
    case 2:
        neighbors[0] = 0;
        neighbors[1] = 7;
        break;
    case 3:
        neighbors[0] = 4;
        neighbors[1] = 5;
        break;
    case 4:
        neighbors[0] = 0;
        neighbors[1] = 3;
        break;
    case 5:
        neighbors[0] = 3;
        neighbors[1] = 9;
        break;
    case 6:
        neighbors[0] = 7;
        neighbors[1] = 1;
        break;
    case 7:
        neighbors[0] = 2;
        neighbors[1] = 6;
        break;
    case 8:
        neighbors[0] = 1;
        neighbors[1] = 9;
        break;
    case 9:
        neighbors[0] = 5;
        neighbors[1] = 8;
        break;
    default:
        break;
    }
    return neighbors;
}

int **getGroups(int nGroups, int nProcess)
{
    int **groups = (int **)malloc(nGroups * sizeof(int *));
    for (int i = 0; i < nGroups; i++)
        groups[i] = (int *)malloc(nProcess * sizeof(int));

    groups[0][0] = 0;
    groups[0][1] = 2;
    groups[0][2] = 4;
    groups[1][0] = 1;
    groups[1][1] = 6;
    groups[1][2] = 8;
    groups[2][0] = 2;
    groups[2][1] = 0;
    groups[2][2] = 7;
    groups[3][0] = 3;
    groups[3][1] = 4;
    groups[3][2] = 5;
    groups[4][0] = 4;
    groups[4][1] = 0;
    groups[4][2] = 3;
    groups[5][0] = 5;
    groups[5][1] = 3;
    groups[5][2] = 9;
    groups[6][0] = 6;
    groups[6][1] = 7;
    groups[6][2] = 1;
    groups[7][0] = 7;
    groups[7][1] = 2;
    groups[7][2] = 6;
    groups[8][0] = 8;
    groups[8][1] = 1;
    groups[8][2] = 9;
    groups[9][0] = 9;
    groups[9][1] = 5;
    groups[9][2] = 8;
    return groups;
}


int *getNeighbors2(int iRank, int numNeighbors)
{
    int *neighbors = (int *)calloc(numNeighbors, sizeof(int));
    switch (iRank)
    {
    case 0:
        neighbors[0] = 2;
        neighbors[1] = 4;
        neighbors[2] = 8;
        break;
    case 1:
        neighbors[0] = 6;
        neighbors[1] = 8;
        neighbors[2] = 4;
        break;
    case 2:
        neighbors[0] = 0;
        neighbors[1] = 7;
        neighbors[2] = 9;
        break;
    case 3:
        neighbors[0] = 4;
        neighbors[1] = 5;
        neighbors[2] = 6;
        break;
    case 4:
        neighbors[0] = 0;
        neighbors[1] = 1;
        neighbors[2] = 3;
        break;
    case 5:
        neighbors[0] = 3;
        neighbors[1] = 7;
        neighbors[2] = 9;
        break;
    case 6:
        neighbors[0] = 1;
        neighbors[1] = 3;
        neighbors[2] = 7;
        break;
    case 7:
        neighbors[0] = 2;
        neighbors[1] = 5;
        neighbors[2] = 6;
        break;
    case 8:
        neighbors[0] = 0;
        neighbors[1] = 1;
        neighbors[2] = 9;
        break;
    case 9:
        neighbors[0] = 2;
        neighbors[1] = 5;
        neighbors[2] = 8;
        break;
    default:
        break;
    }
    return neighbors;
}