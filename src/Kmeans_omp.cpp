#include "kmeans.h"
void Kmeans::init(){
        int n_px;  // # de pixeles de la img
        int iter, max_iters; // iteracion , # max de iteration
        int changes; // n cambios 
        int *labels;  // etiquetas
        double *centers; // centros de cluster
        double *dists; // distancias

        max_iters = *n_iters;

        n_px = width * height;

        labels = (int *) malloc(n_px * sizeof(int));
        centers = (double *) malloc(n_clus * n_ch * sizeof(double));
        dists = (double *) malloc(n_px * sizeof(double));

        omp_set_num_threads(n_threads);

        init_centers(data, centers, n_px, n_ch, n_clus);
        
        for (iter = 0; iter < max_iters; iter++) {
            assign_pixels(data, centers, labels, dists, &changes, n_px, n_ch, n_clus);

            if (!changes) {
                break;
            }

            update_centers(data, centers, labels, dists, n_px, n_ch, n_clus);
        }

        update_data(data, centers, labels, n_px, n_ch);

        compute_sse(sse, dists, n_px);

        *n_iters = iter;

        free(centers);
        free(labels);
        free(dists);

    
}
void Kmeans::init_centers(byte_t *data, double *centers, int n_px, int n_ch, int n_clus){
    int k, ch, rnd;

    for (k = 0; k < n_clus; k++) {
        rnd = rand() % n_px;

        for (ch = 0; ch < n_ch; ch++) {
            centers[k * n_ch + ch] = data[rnd * n_ch + ch];
        }
    }
}
void Kmeans::assign_pixels(byte_t *data, double *centers, int *labels, double *dists, int *changes, int n_px, int n_ch, int n_clus){
    int px, ch, k;
    int min_k, tmp_changes = 0;
    double dist, min_dist, tmp;

    #pragma omp parallel for schedule(static) private(px, ch, k, min_k, dist, min_dist, tmp)
    for (px = 0; px < n_px; px++) {
        min_dist = DBL_MAX;

        for (k = 0; k < n_clus; k++) {
            dist = 0;

            for (ch = 0; ch < n_ch; ch++) {
                tmp = (double)(data[px * n_ch + ch] - centers[k * n_ch + ch]);
                dist += tmp * tmp;
            }

            if (dist < min_dist) {
                min_dist = dist;
                min_k = k;
            }
        }

        dists[px] = min_dist;

        if (labels[px] != min_k) {
            labels[px] = min_k;
            tmp_changes = 1;
        }
    }

    *changes = tmp_changes;
}
void Kmeans::update_centers(byte_t *data, double *centers, int *labels, double *dists, int n_px, int n_ch, int n_clus)
{
    int px, ch, k;
    int *counts;
    int min_k, far_px;
    double max_dist;

    counts = (int *) malloc(n_clus * sizeof(int));

    // Resetting centers and initializing clusters counters

    for (k = 0; k < n_clus; k++) {
        for (ch = 0; ch < n_ch; ch++) {
            centers[k * n_ch + ch] = 0;
        }

        counts[k] = 0;
    }

    // Computing partial sums of the centers and updating clusters counters

    #pragma omp parallel for private(px, ch, min_k) reduction(+:centers[:n_clus * n_ch],counts[:n_clus])
    for (px = 0; px < n_px; px++) {
        min_k = labels[px];

        for (ch = 0; ch < n_ch; ch++) {
            centers[min_k * n_ch + ch] += data[px * n_ch + ch];
        }

        counts[min_k]++;
    }

    // Dividing to obtain the centers mean

    for (k = 0; k < n_clus; k++) {
        if (counts[k]) {
            for (ch = 0; ch < n_ch; ch++) {
                centers[k * n_ch + ch] /= counts[k];
            }
        } else {
            // If the cluster is empty we find the farthest pixel from its cluster center

            max_dist = 0;

            for (px = 0; px < n_px; px++) {
                if (dists[px] > max_dist) {
                    max_dist = dists[px];
                    far_px = px;
                }
            }

            for (ch = 0; ch < n_ch; ch++) {
                centers[k * n_ch + ch] = data[far_px * n_ch + ch];
            }

            dists[far_px] = 0;
        }
    }

    free(counts);
}
void Kmeans::update_data(byte_t *data, double *centers, int *labels, int n_px, int n_ch)
{
    int px, ch, min_k;

    #pragma omp parallel for schedule(static) private(px, ch, min_k)
    for (px = 0; px < n_px; px++) {
        min_k = labels[px];

        for (ch = 0; ch < n_ch; ch++) {
            data[px * n_ch + ch] = (byte_t)round(centers[min_k * n_ch + ch]);
        }
    }
}
void Kmeans::compute_sse(double *sse, double *dists, int n_px)
{
    int px;
    double res = 0;

    #pragma omp parallel for private(px) reduction(+:res)
    for (px = 0; px < n_px; px++) {
        res += dists[px];
    }

    *sse = res;
}