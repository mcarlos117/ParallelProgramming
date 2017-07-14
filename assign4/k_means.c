/*
 * Skeleton function for Introduction of Parallel Programming,
 * Assignment 4: K-Means Algorithm (Pthread)
 *
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <pthread.h>

#include "k_means.h"

/* Global variables that hold the centers, data points and assignments */
extern struct point p[MAX_POINTS]; /* array that holds data points */
extern struct point u[MAX_CENTERS]; /* array that holds the centers */
extern int c[MAX_POINTS]; /* cluster id for each point */


/*
 * k_means: thread function that implements k_means clustering algorithm
 *
 * Input parameters:
 *     TO STUDENTS: Although the arrays of centers, data points and "points'
 *                  cluster ids" are made global variables, you still
 *                  need to pass in additional variables, such as number of
 *                  clusters, number of data points, and other variables to
 *                  help your decomposition.
 * Output parameters:
 *     TO STUDENTS: You should work on the global variables of centers and
 *                  "points; cluster ids." They are visible in main thread as
 *                  well.
 * Return values:
 *     NONE; main thread will not check return values.
 */

void *k_means(void *parameter)
{
        /* TO STUDENTS: centers random initialization is moved to main func
         * Please implement you k_mean here. Make sure you set the final
         * results (centers and cluster ids) in the above global arrays of
         * "u" and "c." Data points are passed through the global array "p."
         */

        int j,l,i,cluster_size;
        long double min_dist,dist,sumx,sumy;
        struct kMeansParams *kMeanParam = (struct kMeansParams*)parameter;
        //pthread_barrier_wait(&barrier);

        for(l = kMeanParam->start;l < kMeanParam->end; l++){
            //pthread_barrier_wait(&barrier);
            for(i = 0; i < kMeanParam->m; i++){
                //min_dist = MAX_POINTS;
                min_dist = DBL_MAX;
                for(j = 0; j < kMeanParam->k; j++){
                    dist = sqrt ( pow((p[i].x - u[j].x),2) + pow((p[i].y - u[j].y ), 2));
                    if(dist < min_dist){
                        min_dist = dist;
                        c[i] = j;
                    }
                }
            }
            //pthread_barrier_wait(&barrier);
            for(j = 0; j < kMeanParam->k; j++){
                sumx = 0;
                sumy = 0;
                cluster_size = 0;
                for(i = 0; i < kMeanParam->m; i++){
                    if(c[i] == j){
                        sumx += p[i].x;
                        sumy += p[i].y;
                        cluster_size++;
                    }
                    if(cluster_size > 0){
                        u[j].x = sumx / cluster_size;
                        u[j].y = sumy / cluster_size;
                    }else{
                        u[j] = random_center(p);
                    }
                }
            }
            //pthread_barrier_wait(&barrier);
        }

        //pthread_barrier_wait(&barrier);
        return NULL;
}
