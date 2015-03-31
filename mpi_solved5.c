/******************************************************************************
* FILE: mpi_bug5.c
* DESCRIPTION:
*   This is an "unsafe" program. It's behavior varies depending upon the
*   platform and MPI library
* AUTHOR: Blaise Barney
* LAST REVISED: 01/24/09
* Hint: If possible, try to run the program on two different machines,
* which are connected through a network. You should see uneven timings;
* try to understand/explain them.
*
* FIX:  The uneven timings are due to the asynchronous nature of the message
*       passing. When process 0 performs the send operation, it will either be
*       blocked until the message is copied into the reciever process or into
*       the system buffer. In the latter case, control will resume before the
*       message is received and Wtime will be called too early. The easiest
*       fix is probably to just use the synchronous MPI_Ssend. Also, MPI_BYTE
*       should probably be replaced by MPI_CHAR.
*
* Andrew Szymczak
* ajs855
******************************************************************************/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MSGSIZE 2000

int main (int argc, char *argv[])
{
    int        numtasks, rank, i, tag=111, dest=1, source=0, count=0;
    char       data[MSGSIZE];
    double     start, end, result;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf ("mpi_bug5 has started...\n");
        if (numtasks > 2)
            printf("INFO: Number of tasks= %d. Only using 2 tasks.\n", numtasks);
    }

    /******************************* SEND TASK **********************************/
    if (rank == 0)
    {
        for(i=0; i<MSGSIZE; i++)
            data[i] =  'x';

        start = MPI_Wtime();
        while (1)
        {
            MPI_Ssend(data, MSGSIZE, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
            count++;
            if (count % 10 == 0)
            {
                end = MPI_Wtime();
                printf("Count= %d  Time= %f sec.\n", count, end-start);
                start = MPI_Wtime();
            }
        }
    }

    /****************************** RECEIVE TASK ********************************/

    if (rank == 1)
    {
        while (1)
        {
            MPI_Recv(data, MSGSIZE, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
            result = 0.0;
            for (i=0; i < 1000000; i++)
                result = result + (double)random();
        }
    }

    MPI_Finalize();
}







