/*
 * Skeleton function for Introduction of Parallel Programming,
 * Assignment 5: K-Means Algorithm (MPI)
 *
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <mpi.h>

#include "k_means.h"


/*
 * k_means: k_means clustering algorithm implementation.
 *
 * Input parameters:
 *     struct point p[]: array of data points
 *     int m           : number of data points in p[]
 *     int k           : number of clusters to find
 *     int iters       : number of clustering iterations to run
 *
 * Output parameters:
 *     struct point u[]: array of cluster centers
 *     int c[]         : cluster id for each data points
 */
void k_means(struct point p[MAX_POINTS],
        int m,
        int k,
        int iters,
        struct point u[MAX_CENTERS],
        int c[MAX_POINTS])
{
    /* TO STUDENTS: Please implement the MPI k_means here.
     *
     * Note that, this time you will do the random initialization
     * yourself. The random function's interface is the same as
     * previous assignments. You can direct copy the old code here.
     * However, you need to make sure that all your processes have
     * the same initial centers (i.e., only initialized in one
     * process and send the initial centers to other processes).
     */

    int j,l,i,cluster_size;
    int pid,pNum,start,end;
    long double min_dist,dist,sumx,sumy;

    MPI_Comm_rank(MPI_COMM_WORLD,&pid);

    /* randomly initialized the centers */
    if(pid == 0){
        for(j = 0; j < k; j++){
            u[j] = random_center(p);
        }
    }

    for(j = 0; j < k; j++){
        MPI_Bcast(&u[j].x, 1, MPI_DOUBLE,0, MPI_COMM_WORLD);
        MPI_Bcast(&u[j].y, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Comm_size(MPI_COMM_WORLD,&pNum);

    start = pid * (iters / pNum);
    end = pid + (iters / pNum);


    for(l = start; l < end; l++){
        for(i = 0; i < m; i++){
            min_dist = DBL_MAX;
            for(j = 0; j < k; j++){
                dist = sqrt( pow((p[i].x - u[j].x), 2) + pow((p[i].y - u[j].y),2));
                if(dist < min_dist){
                    min_dist = dist;
                    c[i] = j;
                }
            }
        }
        for(j = 0; j < k; j++){
            sumx = 0;
            sumy = 0;
            cluster_size = 0;
            for(i = 0; i < m; i++){
                if(c[i] == j){
                    sumx += p[i].x;
                    sumy += p[i].y;
                    cluster_size++;
                }
            }
            if(cluster_size > 0){
                u[j].x = sumx / cluster_size;
                u[j].y = sumy / cluster_size;
            }else{
                u[j] = random_center(p);
            }
        }
    }
    return;
}
