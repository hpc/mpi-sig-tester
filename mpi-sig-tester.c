/**
 * Copyright (c) 2013      Los Alamos National Security, LLC.
 *                         All rights reserved.
 *
 * This program was prepared by Los Alamos National Security, LLC at Los Alamos
 * National Laboratory (LANL) under contract No. DE-AC52-06NA25396 with the U.S.
 * Department of Energy (DOE). All rights in the program are reserved by the DOE
 * and Los Alamos National Security, LLC. Permission is granted to the public to
 * copy and use this software without charge, provided that this Notice and any
 * statement of authorship are reproduced on all copies. Neither the U.S.
 * Government nor LANS makes any warranty, express or implied, or assumes any
 * liability or responsibility for the use of this software.
 */

/**
 * @author Samuel K. Gutierrez
 */

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "mpi.h"

#define MPICHK(mpi_ret, gtl) \
do { \
    if ( MPI_SUCCESS != (mpi_ret) ) { \
        fprintf(stderr, "MPI_SUCCESS not returned @ %s : line %d\n", __func__, \
                __LINE__); \
        goto gtl; \
    } \
} while(0)

#define HN_BUF_LEN MPI_MAX_PROCESSOR_NAME

static int my_mpi_rank = 0;

static void
signal_catcher(int signum)
{
    printf("sig %d caught by rank %d\n", signum, my_mpi_rank);
    MPI_Finalize();
    exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
    int num_mpi_ranks = 0;

    setbuf(stdout, NULL);
    /* init */
    MPICHK(MPI_Init(&argc, &argv), err);
    MPICHK(MPI_Comm_size(MPI_COMM_WORLD, &num_mpi_ranks), err);
    MPICHK(MPI_Comm_rank(MPI_COMM_WORLD, &my_mpi_rank), err);
    
    signal(23, signal_catcher);

    if (0 == my_mpi_rank) {
        printf("=== starting test ===\n");
        printf("=== num mpi ranks: %d\n", num_mpi_ranks);
    }

    while(1) sleep(100);
err:
    return EXIT_FAILURE;
}
