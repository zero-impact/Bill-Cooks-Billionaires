/****************************************************************************/
/*                                                                          */
/*                      Utility Functions for CO759                         */
/*                                                                          */
/****************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include "util.h"

using namespace std;

/* zeit function for recording times */

double CO759_zeit (void)
{
    struct rusage ru;

    getrusage (RUSAGE_SELF, &ru);

    return ((double) ru.ru_utime.tv_sec) +
           ((double) ru.ru_utime.tv_usec)/1000000.0;
}

double CO759_real_zeit (void)
{
    return (double) time (0);
}

/* function for creating a random set of points in unit square */
int CO759_build_xy (int ncount, double *xlist, double *ylist, int gridsize)
{
    int rval = 0, i, j, winner, x, y;
    int **hit = (int **) NULL, *hitcount = (int *) NULL;

    printf ("Random %d point set, gridsize = %d\n", ncount, gridsize);
    fflush (stdout);

    hit =  (int **) malloc (gridsize * sizeof (int *));
    if (!hit) {
        fprintf (stderr, "out of memory for hit\n");
        rval = 1; goto CLEANUP;
    }
    for (i = 0; i < gridsize; i++) hit[i] = (int *) NULL;

    hitcount = (int *) malloc (gridsize * sizeof (int));
    if (!hitcount) {
        fprintf (stderr, "out of memory for hitcount\n");
        rval = 1; goto CLEANUP;
    }
    for (i = 0; i < gridsize; i++) hitcount[i] = 0;

    for (i = 0; i < ncount; i++) {
        winner = 0;
        do {
            x = (int) (random () % gridsize);
            y = (int) (random () % gridsize);

            /* check to see if (x,y) is a duplicate point */

            for (j = 0; j < hitcount[x]; j++) {
                if (hit[x][j] == y) break;
            }
            if (j == hitcount[x]) {
                void *tmp_ptr = (void *) hit[x];
                tmp_ptr = realloc (tmp_ptr, (hitcount[x]+1)*sizeof (int));
                if (!tmp_ptr) {
                    fprintf (stderr, "out of member in realloc of hit\n");
                    rval = 1; goto CLEANUP;
                }
                hit[x] = (int *) tmp_ptr;
                hit[x][hitcount[x]] = y;
                hitcount[x]++;
                winner = 1;
            }
            if (!winner) {
                printf ("X"); fflush (stdout);
            }
        } while (!winner);
        xlist[i] = (double) x;
        ylist[i] = (double) y;
    }

CLEANUP:

    printf ("\n");

    if (hit) {
        for (i = 0; i < gridsize; i++) {
            if (hit[i]) free (hit[i]);
        }
        free (hit);
    }
    if (hitcount) free (hitcount);
    return rval;
}

int CO759_build_xy (int ncount, vector<double>& xlist, vector<double>& ylist, int gridsize)
{
    return CO759_build_xy(ncount, &xlist[0], &ylist[0], gridsize);
}

bool is_almost_integral(double x) {
    return x < LP_EPSILON || (1.0 - x) < LP_EPSILON;
}

