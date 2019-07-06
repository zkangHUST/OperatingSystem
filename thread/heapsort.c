#include "heapsort.h"
void heap_sort(int* nums, int size)
{
    int i;

    build_max_heap(nums, size);
    for (i = size - 1; i > 0; i--) {
        swap(&nums[0], &nums[i]);
        size -= 1;
        max_heapify(nums, 0, size);
    }
}

void max_heapify(int *nums, int i, int size)
{
    int l = 2 * i + 1, r = l + 1, largest;

    if (l < size && nums[l] > nums[i]) {
        largest = l;
    } else {
        largest = i;
    }
    if (r < size && nums[r] > nums[largest]) {
        largest = r;
    }  
    if (largest != i) {
        swap(&nums[i], &nums[largest]);
        max_heapify(nums, largest, size);
    }
}

void build_max_heap(int* nums, int size)
{
    int i;
    for (i = size / 2; i >= 0; i--) {
        max_heapify(nums, i, size);
    }
}

void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}