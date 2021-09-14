#include "lis.h"
#include <iostream>

#include <algorithm>

std::vector<unsigned> 
longest_increasing_subsequence(std::vector<int> const& sequence)
{
    //allocated/reserve table, init all to 0 - including init case
  unsigned n = sequence.size();
  std::vector<int> d(n, 1), p(n, -1);
  for (unsigned i = 0; i < n; i++)
  {
    for (unsigned j = 0; j < i; j++)
    {
      if (sequence[j] < sequence[i] && d[i] < d[j] + 1)
      {
        d[i] = d[j] + 1;
        p[i] = j;
      }
    }
  }

  int ans = d[0], pos = 0;
  for (unsigned i = 1; i < n; i++)
  {
    if (d[i] > ans)
    {
      ans = d[i];
      pos = i;
    }
  }

  std::vector<unsigned> indices;
  while (pos != -1)
  {
    indices.push_back(pos);
    pos = p[pos];
  }
  std::reverse(indices.begin(), indices.end());
  return indices;
}
