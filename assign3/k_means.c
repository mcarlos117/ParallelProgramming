/*
 * Skeleton function for Introduction of Parallel Programming,
 * Assignment 2: K-Means Algorithm (Sequential)
 *
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <omp.h> /* OpenMP header */

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
    int j,l,i,cluster_size; /* To Students: add your local variables */
    double min_dist,dist,sumx, sumy;

    /* randomly initialized the centers */
    /* Note: DO NOT CHANGE THIS RANDOM GENERATOR! */
    /* Note: DO NOT PARALLELIZE THIS LOOP */
    /* Note: THE INTERFACE TO random_center HAS CHANGED */
    for(j = 0; j < k; j++)
        u[j] = random_center(p);

    /*
     * To students: please implment K-Means algorithm with OpenMP here
     * Your K-means implementation should do "iters" rounds of clustering. After
     * all iterations finish, array u[MAX_CENTERS] should have the coordinations
     * of your centers, and array c[MAX_POINTS] should have the cluster assignment
     * for each point.
     */
        for(l = 0; l < iters; l++){
            #pragma omp parallel for private(j,min_dist,dist)
            for(i = 0; i < m; i++){
                min_dist = MAX_POINTS;
                for(j = 0; j < k; j++){
                    dist = sqrt( pow((p[i].x - u[j].x), 2) + pow((p[i].y - u[j].y ), 2));
                    if(dist < min_dist){
                        min_dist = dist;
                        c[i] = j;
                    }
                }
            }
            #pragma omp parallel for private(i,sumx,sumy,cluster_size)
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
                    if(cluster_size>0){
                        u[j].x = sumx / cluster_size;
                        u[j].y = sumy / cluster_size;
                    }else{
                        u[j] = random_center(p);
                    }
                }
            }

        }
    return;
}
