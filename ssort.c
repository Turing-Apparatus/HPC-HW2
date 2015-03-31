/* PARALLEL SAMPLE SORT
*   Andrew Szymczak
*   ajs855
*/

#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
// #include <limits.h>

#define COMM MPI_COMM_WORLD
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

void array_to_file(int, int*, char*);
static int compare(const void*, const void*);



int main( int argc, char *argv[])
{
    int rank, P, N, s, bin, i=0, j=0;
    int *vec, *sample, *bucket;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(COMM, &P);
    MPI_Comm_rank(COMM, &rank);

    N = (argc < 2)? 400 : atoi(argv[1]);                                // INPUT DEFAULTS
    s = (argc < 3)? MAX(N/P,P) : atoi(argv[2]);                         // N=400, s=max(N/P,P)
    if (N < 1 || P < 1 || s < 2 ) { MPI_Abort(COMM,1); }

    vec     = calloc(N, sizeof(int));                                   // VECTOR TO BE SORTED
    sample  = calloc(s, sizeof(int));                                   // RANDOM SAMPLE FROM vec
    int split[P], scounts[P], sdispls[P];                               // splits AND Alltoallv ARGS
    int rcounts[P], rdispls[P];

    srand ((rank+1)*time(NULL));                                        // SEED
    for (i = 0; i < N; i++) { vec[i] = rand(); }                        // INITIALIZE
    qsort(vec, N, sizeof(int), compare);                                // SORT
    for (i = 0; i < s; i++) { sample[i] = vec[i*N/s]; }                 // SAMPLE

    if (rank == 0)
    {
        int *allsamps = calloc(s*P, sizeof(int));
        MPI_Gather(sample, s, MPI_INT, allsamps, s, MPI_INT, 0, COMM);  // GATHER SAMPLES
        qsort(allsamps, s*P, sizeof(int), compare);                     // SORT SAMPLES
        for (i = 1; i < P; i++) { split[i-1] = allsamps[i*s]; }         // GET SPLITS
        split[P-1] = RAND_MAX;
        free(allsamps);
    }
    else { MPI_Gather(sample, s, MPI_INT, NULL, s, MPI_INT, 0, COMM); }

    MPI_Bcast(split, P, MPI_INT, 0, COMM);                              // BCAST SPLITS
    sdispls[0] = 0;
    for (i = 0; i<N; i++)                                               // GET INDICES OF SPLITS
    {
        if (vec[i] > split[j])
        {
            scounts[j] = (j==0)? i : i-sdispls[j];
            sdispls[++j] = i;
        }
    }
    scounts[P-1] = N-sdispls[P-1];

    MPI_Alltoall(scounts, 1, MPI_INT, rcounts, 1, MPI_INT, COMM);       // a2a ORDERED TRANSFER
    rdispls[0] = 0;
    for (i=1; i<P; i++) { rdispls[i] = rcounts[i-1] + rdispls[i-1]; }
    bin = rcounts[P-1] + rdispls[P-1];
    bucket = calloc(bin, sizeof(int));
    MPI_Alltoallv(vec, scounts, sdispls, MPI_INT, bucket, rcounts, rdispls, MPI_INT, COMM);
    qsort(bucket, bin, sizeof(int), compare);

    char file[256];                                                     // WRITE TO FILE
    snprintf(file, 256, "Output/output%d.txt", rank);
    array_to_file(bin, bucket, file);

    free(vec);
    free(sample);
    free(bucket);
    MPI_Finalize();
    return 0;
}



void array_to_file(int n, int *arr, char *file)
{
    FILE* fd = NULL;
    fd = fopen(file,"w+");
    int i;

    if(fd == NULL) { printf("Error opening file \n"); }
    else
    {
        for(i = 0; i < n; i++) { fprintf(fd, "%d\n", arr[i]); }
        fclose(fd);
    }
}



static int compare(const void *a, const void *b)
{
    int *da = (int *)a;
    int *db = (int *)b;

    if (*da > *db)      { return 1;  }
    else if (*da < *db) { return -1; }
    else                { return 0;  }
}



