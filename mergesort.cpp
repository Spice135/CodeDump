#include "mergesort.h"

int* left;
int* right;

void merge(int *arr, int l, int m, int r)
{
  int n1, n2, i, j, k;

  n1 = m - l + 1;
  n2 = r - m;

  //int* left = new int[n1];
  //int* right = new int[n2];

  for (i = 0; i < n1; i++)
    left[i] = arr[l + i]; // Set left array

  for (j = 0 ; j < n2; j++)
    right[j] = arr[m + 1 + j]; // Set right array

  i = 0, j = 0, k = l; // Reset values

  while (i < n1 && j < n2)
  {
    if (left[i] <= right[j])
      arr[k++] = left[i++]; // Copy left value
    else
      arr[k++] = right[j++]; // Copy right value
  }

  while (i < n1)
    arr[k++] = left[i++]; // Extra values
  while (j < n2)
    arr[k++] = right[j++]; // Extra values
}

void mergeSort(int *arr, unsigned l, unsigned r)
{
  if (l < r)
  {
    int m = l + ((r - l) / 2); // Calculate midpoint

    mergeSort(arr, l, m); // Call on left side
    mergeSort(arr, m + 1, r); // Call on right side
    
    if (arr[m] > arr[m + 1])
      merge(arr, l, m, r);
  }
}

void mergesort(int *arr, unsigned l, unsigned r)
{
  left = new int[r];
  right = new int[r];
  mergeSort(arr, l, r - 1); // Correct the indices
  delete[] left; // Clean up memory
  delete[] right; // Clean up memory
}
