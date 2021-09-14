#include "perm-jt.h"

PermJohnsonTrotter::PermJohnsonTrotter(int size) : digits(), directions()
{
  for (int i = 1; i <= size; i++)
  {
    digits.push_back(i);
    directions.push_back(0);
  }
}
 
bool PermJohnsonTrotter::Next()
{
  int k;
  if(isMobile())
  {
    k = findLargest();

    if (directions[k] == 0)
    {
      std::swap(digits[k], digits[k - 1]);
      std::swap(directions[k], directions[k - 1]);
      k -= 1;
    }
    else
    {
      std::swap(digits[k], digits[k + 1]);
      std::swap(directions[k], directions[k + 1]);
      k += 1;
    }

    for (unsigned i = 0; i < digits.size(); i++)
      if (digits[i] > digits[k])
      {
        if (directions[i] == 0)
        {
          directions[i] = 1;
        }
        else
        {
          directions[i] = 0;
        }
      }
    return true;
  }
  else
    return false;
}

std::vector<int> const& PermJohnsonTrotter::Get() const
{
  return digits;
}

bool PermJohnsonTrotter::isMobile()
{
  for (unsigned i = 0; i < digits.size(); i++)
    if (isMobile(i))
      return true;
  return false;
}

bool PermJohnsonTrotter::isMobile(unsigned p)
{
  if (p == 0 && directions[p] == 0)
    return false;
  else if (p == digits.size() - 1 && directions[p] == 1)
    return false;
  else
    if (directions[p] == 0)
    {
      if (digits[p] > digits[p-1])
        return true;
    }
    else
    {
      if (digits[p] > digits[p+1])
        return true;
    }
  return false;
}

int PermJohnsonTrotter::findLargest()
{
  std::vector<int> mobile;
  for (unsigned i = 0; i < digits.size(); i++)
    if (isMobile(i))
      mobile.push_back(i);

  int largest = mobile[0];
  for (unsigned p = 1; p < mobile.size(); p++)
    if (digits[mobile[p]] > digits[largest])
      largest = mobile[p];

  return largest;
}
