#include <iostream>
#include <algorithm>
#include <climits>
#include <cassert>
#include <omp.h>
#include <fstream>
#include <memory.h>
#include <vector>
#include <stdio.h>
#include <queue>
#include "utils.h"

#define d float
#define INF 99999999
using namespace std;

void bfsSeq(d *distance, Graph &g, int root=0) {
    // int *distance = new int[g.numNode];
    for (int i = 0; i < g.numNode; i++) distance[i] = INF;
    distance[root] = 0.0;
    priority_queue<NodeDist> FS;
    priority_queue<NodeDist> NS;
    FS.push(NodeDist(root, 0));
    while (!FS.empty()) {
        while (!FS.empty()) {
            NodeDist nd = FS.top();
            FS.pop();
            int u = nd.id;
            for (int j = 0; j < g.neighborList[u].size(); j++) {
                int v = g.neighborList[u][j];
                d new_dist = distance[u] + g.map[u * g.numNode + v];
                if (new_dist < distance[v]) {
                    NS.push(NodeDist(v, new_dist));
                    distance[v] = new_dist;
                }
            }
        }
        // printf("[before merge] NS size = %d, FS size = %d\n", NS.size(), FS.size());

        // merge
        int tmpNodeId = -1;
        while (!NS.empty()) {
            NodeDist nd = NS.top();
            // printf("delete from NS: (%d, %lf)\n", nd.id, nd.dist);
            NS.pop();
            if (nd.id != tmpNodeId) {
                FS.push(nd);
                // printf("insert to FS: (%d, %lf)\n", nd.id, nd.dist);
                tmpNodeId = nd.id;
            }
        }
        // printf("[after merge] NS size = %d, FS size = %d\n", NS.size(), FS.size());
    }
}

// void bfsPar(d *distance, Graph &g, int root=0) {
//     // int *distance = new int[g.numNode];
//     for (int i = 0; i < g.numNode; i++) distance[i] = -1;
//     distance[root] = 0.0;
//     priority_queue<NodeDist> FS;
//     priority_queue<NodeDist> NS;
//     FS.push(NodeDist(root, 0));
//     while (!FS.empty()) {
//         while (!FS.empty()) {
//             NodeDist nd = FS.top();
//             FS.pop();
//             int u = nd.id;
//             for (int j = 0; j < g.neighborList[u].size(); j++) {
//                 int v = g.neighborList[u][j];
//                 d new_dist = distance[u] + g.map[u * g.numNode + v];
//                 if (distance[v] == -1 || new_dist < distance[v]) {
//                     NS.push(NodeDist(v, new_dist));
//                     distance[v] = new_dist;
//                 }
//             }
//         }
//         // printf("[before merge] NS size = %d, FS size = %d\n", NS.size(), FS.size());

//         // merge
//         int tmpNodeId = -1;
//         while (!NS.empty()) {
//             NodeDist nd = NS.top();
//             // printf("delete from NS: (%d, %lf)\n", nd.id, nd.dist);
//             NS.pop();
//             if (nd.id != tmpNodeId) {
//                 FS.push(nd);
//                 // printf("insert to FS: (%d, %lf)\n", nd.id, nd.dist);
//                 tmpNodeId = nd.id;
//             }
//         }
//         // printf("[after merge] NS size = %d, FS size = %d\n", NS.size(), FS.size());
//     }
// }


int main(int argc, char** argv) {
    if(argc != 5) {
        cout << "Usage: ./a.out seed numNode numEdge nthreads" << endl;
        return 1;
    }
    int seed = atoi(argv[1]);
    int numNode = atoi(argv[2]);  // 10,000
    int numEdge = atoi(argv[3]);  // 50,000,000
    int nthreads = atoi(argv[4]);

    omp_set_num_threads(nthreads);

    Graph graph = createRandomDirectedGraph(numNode, numEdge, seed);
    printf("---------------------------------------------------\n");
    printf("numNode = %d, numEdge = %d\n", graph.numNode, graph.numEdge);
    graph.printEdges();
    graph.printNeighbors();
    graph.printMap();
    printf("---------------------------------------------------\n");
    d *distance = new d[graph.numNode];
    double start_bfs = omp_get_wtime();
    bfsSeq(distance, graph, 0);
    double bfs_seq_time = omp_get_wtime() - start_bfs;
    printf("bfs_seq_time: %.12lfs\n", bfs_seq_time);
    for (int i = 0; i < graph.numNode; i++) {
        printf("Node %d: distance = %lf\n", i, distance[i]);
    }
    return 0;
}


// void mergesort(int* a, int* tmp, int n, int bc);
// void medianofunion(int* a, int n, int& ia, int* b, int m, int& ib);
// void recmerge(int* a, int n, int* b, int m, int* c, int bc);
// void mycopy(int* src, int n, int* dst, int bc);

// // allocate memory
// // int* v = new int[length]; // array to be sorted
// // int* t = new int[length]; // temporary workspace
// // int* v2 = new int[length]; // copy for checking

// // int mergesort_count = 0;
// // int merge_count = 0;

// int main(int argc, char** argv) {

//     // read inputs
//     if(argc != 5) {
//         cout << "Usage: ./a.out seed length basecase nthreads" << endl;
//         return 1;
//     }
//     srand(atoi(argv[1]));
//     int length = atoi(argv[2]);
//     int bc = atoi(argv[3]);
//     int nthreads = atoi(argv[4]);

//     // fstream myfile;
//     // myfile.open("test.txt");
//     // myfile << "Writing this to a file.\n";
    
//     // // allocate memory
//     int* v = new int[length]; // array to be sorted
//     int* t = new int[length]; // temporary workspace
//     int* v2 = new int[length]; // copy for checking
//     int* v_origin = new int[length]; // original backup

//     // initialize input randomly
//     for (int i = 0; i < length; i++) {
//         v[i] = v2[i] = v_origin[i] = rand() % length;
//     }
//     // sort array using STL (and time it)
//     double start_sort = omp_get_wtime();
//     sort(v2,v2+length);
//     double elapsed_sort = omp_get_wtime() - start_sort;

//     printf("num_thread\tbc\t\ttime_sort\ttime\tspeedup\t\tefficiency (%)\n");

//     // sort array using mergesort (and time it)
//     for (int i=1; i<=44; i++) {
//         omp_set_num_threads(i);
//         copy(v_origin, v_origin+length, v);
//         double start_mergeSort = omp_get_wtime();
//         bc = int(length / i);
        
//         #pragma omp parallel   // [Enze]
//         {
//             #pragma omp single  // [Enze]
//             mergesort(v, t, length, bc);  // mergesort(v, t, length, bc);
//         }
//         double elapsed_mergeSort = omp_get_wtime() - start_mergeSort;
//         printf("%d\t\t%d\t%.4lf\t\t%.4lf\t%.4lf\t\t%.4lf\n", i, bc, elapsed_sort, elapsed_mergeSort, elapsed_sort/elapsed_mergeSort, elapsed_sort/elapsed_mergeSort/i*100.0);
//     }

//     // #pragma omp parallel   // [Enze]
//     // {
//     //     #pragma omp single  // [Enze]
//     //     mergesort(v, t, length, i);  // mergesort(v, t, length, bc);
//     // }

    

//     // check outputs, make sure they match
//     for(int i = 0; i < length; i++) {
//         assert(v[i] == v2[i]);
//     }

//     // // report timings
//     // cout << "time:   \t" << elapsed_mergeSort << '\t' << elapsed_sort << endl;
//     // cout << "speedup:\t" << elapsed_sort / elapsed_mergeSort << endl;

//     // printf("mergesort_count: %d\tmerge_count: %d\n", mergesort_count, merge_count);
//     // myfile.close();


//     delete [] v2;
//     delete [] t;
//     delete [] v;

// }

// void mycopy(int* src, int n, int* dst, int bc) {
//     if (n <= bc) {
//         copy(src, src+n, dst);
//         return;
//     }
//     int mid = n / 2;
//     #pragma omp task  // [Enze]
//     mycopy(src, mid, dst, bc);
//     // #pragma omp task  // [Enze]
//     mycopy(src + mid, n - mid, dst + mid, bc);
//     #pragma omp taskwait  // [Enze]
// }

// // sorts array a of length n, tmp is workspace of length n,
// // bc is base case size to switch to STL sort
// void mergesort(int* a, int* tmp, int n, int bc)
// {
//     if(n <= bc) {
//         sort(a, a+n);
//         // printf("thread: %d offset: %d", omp_get_thread_num(), (a - v)/4);

//         // mergesort_count ++;
//         return;
//     }
    
//     // sort left and right recursively
//     int mid = n / 2;
//     #pragma omp task  // [Enze]
//     mergesort(a, tmp, mid, bc);
//     // #pragma omp task  // [Enze]
//     mergesort(a + mid, tmp + mid, n - mid, bc);
//     #pragma omp taskwait  // [Enze]

//     // merge left and right into tmp and copy back into a (using STL)
//     // #pragma omp parallel   // [Enze]
//     // {
//     //     #pragma omp single  // [Enze]
//     recmerge(a, mid, a+mid, n-mid, tmp, bc);
//     // }

//     // copy(tmp,tmp+n,a);
//     // #pragma omp parallel   // [Enze]
//     // {
//     //     #pragma omp single  // [Enze]
//     mycopy(tmp, n, a, bc);
//     // }
// }

// // merges sorted arrays a (length n) and b (length m) into array c (length n+m),
// // bc is base case size to switch to STL merge
// void recmerge(int* a, int n, int* b, int m, int* c, int bc) {

//     if(n+m<=bc){
//         merge(a, a+n, b, b+m, c);
//         // merge_count ++;
//         return;
//     }

//     // compute median of union with i elements of a and j elements of b <= median
//     int i, j;
//     medianofunion(a, n, i, b, m, j);

//     // merge left and right recursively
//     #pragma omp task  // [Enze]
//     recmerge(a, i, b, j, c, bc);
//     // #pragma omp task  // [Enze]
//     recmerge(a+i, n-i, b+j, m-j, c+i+j, bc);
//     #pragma omp taskwait  // [Enze]
// }
    
// // computes median of union of array a of length n and array b of length m
// // assuming elements of a and b are already internally sorted; upon return:
// // ma is number of elements in a less than or equal to median of union,
// // mb is number of elements in b less than or equal to median of union
// void medianofunion(int *a, int n, int& ma, int *b, int m, int& mb) 
// {
//     // enforce that a is smaller of two arrays
//     if(n > m) {
//         medianofunion(b,m,mb,a,n,ma);
//         return;
//     }

//     // run binary search in array a
//     int i = 0;
//     int j = n;
//     while (i <= j) {
//         // get middle two elements of each array (use extremes to handle corner cases)
//         ma = (i + j) / 2;
//         mb = (n + m + 1) / 2 - ma;
//         int la = (ma > 0) ? a[ma - 1]: INT_MIN; 
//         int lb = (mb > 0) ? b[mb - 1] : INT_MIN;
//         int ra = (ma < n) ? a[ma] : INT_MAX;
//         int rb = (mb < m) ? b[mb] : INT_MAX;

//         // check for complete (la <= {ra,rb} and lb <= {ra,rb})
//         if (la <= rb && lb <= ra)
//             return;

//         // continue search
//         if (la > rb) 
//             j = ma - 1;
//         else // lb > ra
//             i = ma + 1;
//     }
// }

