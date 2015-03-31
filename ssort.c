/* Parallel sample sort
 */
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>

#define COMM MPI_COMM_WORLD


static int compare(const void *a, const void *b)
{
    int *da = (int *)a;
    int *db = (int *)b;

    if (*da > *db)      { return 1;  }
    else if (*da < *db) { return -1; }
    else                { return 0;  }
}



int main( int argc, char *argv[])
{
    int rank, P, N, bin, i,j=0;
    int *vec, *sample, *bucket;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(COMM, &P);
    MPI_Comm_rank(COMM, &rank);

    N  =  (argc < 1)? 400 : atoi(argv[1]);                  // INPUT DEFAULTS
    s = (argc < 2)? max(N/P,P) : atoi(argv[2]);             // N=400, s=max(N/P,P)
    if (N < 1)
    {
        printf("Invalid argument\n");
        MPI_Abort(COMM,1);
    }

    vec     = calloc(N, sizeof(int));                       // VECTOR TO BE SORTED
    sample  = calloc(s, sizeof(int));                       // RANDOM SAMPLE FROM vec
    int split[P], scounts[P], sdispls[P];
    int rcounts[P], rdispls[P];

    srand((unsigned int) (rank + 393919));                  // SEED
    for (i = 0; i < N; ++i) { vec[i] = rand(); }            // INITIALIZE
    qsort(vec, N, sizeof(int), compare);                    // SORT
    for (i = 0; i < s; i++) { sample[i] = vec(i*N/s); }     // SAMPLE

    if (rank == 0)
    {
        int *allsamps = calloc(s*P, sizeof(int));
        MPI_Gather(sample, s, MPI_INT, allsamps, s, MPI_INT, 0, COMM);  // GATHER
        qsort(allsamps, s*P, sizeof(int), compare);                     // SORT SAMPLES
        for (i = 1; i < P; i++) { split[i] = allsamps[i*s - s/2] };     // GET SPLITS
        split[P-1] = MAX_INT;
    }
    else { MPI_Gather(sample, s, MPI_INT, NULL, s, MPI_INT, 0, COMM); }

    MPI_Bcast(split, P, MPI_INT, 0, COMM);                      // BCAST SPLITS
    for (i = 0; i<N; i++)                                       // GET INDICES OF SPLITS
    {
        if (vec[i] > split[j])
        {
            scounts[j] = (j==0)? i : i-scounts[j-1];
            sdispls[j++] = i;
        }
    }

    MPI_Alltoall(scounts, 1, MPI_INT, rcounts, 1, MPI_INT, COMM);
    MPI_Alltoall(sdispls, 1, MPI_INT, rdispls, 1, MPI_INT, COMM);
    bin = rcounts[length(rcounts)-1] + rdispls[length(rdispls)-1];
    bucket = calloc(bin, sizeof(int))
    MPI_Alltoallv(vec, scounts, sdispls, MPI_INT, bucket, rcounts, rdispls, MPI_INT, COMM);
    qsort(bucket, bin, sizeof(int), compare);



    /* randomly sample s entries from vector or select local splitters,
     * i.e., every N/P-th entry of the sorted vector */

    /* every processor communicates the selected entries
     * to the root processor; use for instance an MPI_Gather */

    /* root processor does a sort, determinates splitters that
     * split the data into P buckets of approximately the same size */

    /* root process broadcasts splitters */

    /* every processor uses the obtained splitters to decide
     * which integers need to be sent to which other processor (local bins) */

    /* send and receive: either you use MPI_AlltoallV, or
     * (and that might be easier), use an MPI_Alltoall to share
     * with every processor how many integers it should expect,
     * and then use MPI_Send and MPI_Recv to exchange the data */

    /* do a local sort */

    /* every processor writes its result to a file */

    free(vec);
    free(sample);
    free(bucket);
    MPI_Finalize();
    return 0;
}
