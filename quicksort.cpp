#include "quicksort.h"
#include <algorithm>

unsigned partition(int* arr, int l , int r)
{
  int pivot = arr[r]; // Calculate pivot 
  int i = (l - 1); // Index (lowest - 1)
  for (int j = l; j < r; j++) 
  {
    if (arr[j] <= pivot) 
    { 
      i++; // Smaller element index
      std::swap(arr[i], arr[j]); 
    } 
  } 
  std::swap(arr[i + 1], arr[r]);
  return (i + 1); // Return index + 1
}

void quickSort(int* arr, int l, int r)
{
  if (l < r) 
  {
    int pivot = partition(arr, l, r); // Get pivot
    quickSort(arr, l, pivot - 1); // Call quicksort without pivot
    quickSort(arr, pivot + 1, r); // Call quicksort without pivot
  }
}

void quicksort(int* arr, unsigned l, unsigned r)
{
  quickSort(arr, l, r - 1); // Correct index
}