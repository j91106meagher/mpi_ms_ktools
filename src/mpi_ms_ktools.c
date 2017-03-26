#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char** argv)

{

  int rank, num_proc, namelen;

  char processor_name[MPI_MAX_PROCESSOR_NAME];

  int p, proc;

  unsigned long num_cat, cat, cats_done, *cat_assign;

  double results[RESULTS_SIZE];

  MPI_Status status;


  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

  MPI_Get_processor_name(processor_name, &namelen);

  printf(“Program starting on rank %d of %d on %s\n”, rank, num_proc,

    processor_name);

  num_cat = NUM_CATEGORIES;

  /* Broadcast of parameters and global data to slave processes */

    .

    .

    .

  /* Begin processing */

  if (rank) {

    /* SLAVE PROCESS CODE */

    for (cat = 0; cat != -1;) {

      printf(“%d: Awaiting assignment from master\n”, rank);

      MPI_Recv(&cat, 1, MPI_UNSIGNED_LONG, 0, ASSIGNMENT_TAG,

        MPI_COMM_WORLD, &status);

      if (cat != -1) {

        printf(“%d: Received assignment of category %ld\n”, rank, cat);

        analyze_category(cat, results);

        printf(“%d: Sending results from category %ld to master\n”, rank, cat);
        MPI_Send(&results, RESULTS_SIZE, MPI_DOUBLE, 0,

                  RESULTS_TAG, MPI_COMM_WORLD);

              }

              else

                printf(“%d: Received die signal from master\n”,

                  rank);

            }

          }

          else {

            /* MASTER PROCESS CODE */

            cat_assign = (unsigned long *)malloc(num_proc *

              sizeof(unsigned long));

            /* Initial assignments */

            for (p = 1, cat = 0; p < num_proc; p++) {

              MPI_Send(&cat, 1, MPI_UNSIGNED_LONG, p, ASSIGNMENT_TAG,

                MPI_COMM_WORLD);

              printf(“%d: Assigning category %ld to process %d\n”,

                rank, cat, p);
              cat_assign[p] = cat;

                    if (cat != -1) {

                      cat++;

                      if (cat == num_cat) cat = -1;

                    }

                  }

                  /* Obtain results & continue doling out work until exhausted */

                  for (cats_done = 0; cats_done < num_cat;) {

                    MPI_Recv(&results, RESULTS_SIZE, MPI_DOUBLE,

                      MPI_ANY_SOURCE, RESULTS_TAG, MPI_COMM_WORLD,

                      &status);

                    proc = status.MPI_SOURCE;

                    save_results(cat_assign[proc], results);

                    cats_done++;

                    MPI_Send(&cat, 1, MPI_UNSIGNED_LONG, proc,

                      ASSIGNMENT_TAG, MPI_COMM_WORLD);

                    printf(“%d: Assigning category %ld to process %d\n”,

                      rank, cat, proc);
                    cat_assign[proc] = cat;

                          if (cat != -1) {

                            cat++;

                            if (cat == num_cat) cat = -1;

                          }

                        }

                      }

                      printf(“%d: Calling MPI_Finalize()\n”, rank);

                      MPI_Finalize();

                      return 0;

                    }
