#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

__global__ void TestDouble(double *arr, float *device_double_time, int size) {

    int xid = blockIdx.x * blockDim.x + threadIdx.x;

    clock_t start = clock();

    if (xid < size) {
        for (int i = 0; i < 256; i++)
            arr[xid] = arr[xid] * arr[xid] + arr[xid];
    }

    clock_t end = clock();
    device_double_time[xid] = (float)(end - start);

}
