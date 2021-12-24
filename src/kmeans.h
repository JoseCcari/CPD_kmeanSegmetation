#ifndef KMEANS_H
#define KMEANS_H

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <omp.h>
#include "image_io.h"

class Kmeans{
    byte_t *data= nullptr;; 
    int width; 
    int height; 
    int n_ch; 
    int n_clus; 
    int *n_iters= nullptr; 
    double *sse= nullptr;
    int n_threads;

    public: 

        Kmeans(byte_t *data, int width, int height, int n_ch, int n_clus, int *n_iters, double *sse, int n_threads){
           this->data=data; 
            this->width=width; 
            this->height=height; 
            this->n_ch=n_ch; 
            this->n_clus=n_clus; 
            this->n_iters=n_iters; 
            this->sse=sse; 
            this->n_threads=n_threads; 
        };
        void init();
        //Función para inicializar los k centros de forma aletoria.
        void init_centers(byte_t *data, double *centers, int n_px, int n_ch, int n_clus);
        //Función que asigna los pixeles de la imagen a los centros dependiendo de cuan cerca esté.
        void assign_pixels(byte_t *data, double *centers, int *labels, double *dists, int *changes, int n_px, int n_ch, int n_clus);
        //Función que Actualiza los nuevos centros de cada cluster
        void update_centers(byte_t *data, double *centers, int *labels, double *dists, int n_px, int n_ch, int n_clus);
        //actualiza el color de los pixeles luego de terminar.
        void update_data(byte_t *data, double *centers, int *labels, int n_px, int n_ch);
        //Función que computa la Sum Squared Error.
        void compute_sse(double *sse, double *dists, int n_px);

            
};

#endif