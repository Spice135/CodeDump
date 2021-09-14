#include "perm-lexicographical.h"
#include <algorithm>
#include <iostream>

bool nextPermLexicographical(std::vector<int> & p)
{
  std::size_t length = p.size(); // Get length of the vector
  
  if (length == 0)
    return false; // If empty, return false

  std::size_t i = length - 1; // Get last - 1 index
  while (i > 0 && p[i - 1] >= p[i])
    i--;

  if (i == 0)
    return false; // Return false if gotten to start
  
  std::size_t j = length - 1; // Get last - 1 index
  while (p[j] <= p[i - 1])
    j--;

  std::swap(p[i - 1], p[j]); // Swap the 2  
  std::reverse(&p[i], &p[length]); // Reverse the suffix
  return true; // Return true
}
