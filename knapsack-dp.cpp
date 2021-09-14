#include "knapsack-dp.h"
#include <iostream>
#include <numeric>

////////////////////////////////////////////////////////////
Item::Item( int const& weight, int const& value ) 
	: weight(weight), value(value) 
{
}

////////////////////////////////////////////////////////////
Item::Item( Item const& original ) 
	: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, Item const& item) {
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>> (std::istream& os, Item & item) {
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector< std::vector<int> > Table; //2-dimensional table

////////////////////////////////////////////////////////////
//the returned value is a vector of indices
std::vector<int> knapsackDP( std::vector<Item> const& items, int const& W )
{
	int num_items = items.size(); // Size of items
  int i, w;
  Table K(W + 1); // Create table
  
  // Loop to create columns
  for (int i = 0; i < W + 1; i++)
    K[i] = std::vector<int>(num_items + 1);

  for (i = 0; i <= num_items; i++)
  {
    for (w = 0; w <= W; w++)  
    { 
      if (i == 0 || w == 0) 
        K[w][i] = 0; // Set first row and col to 0
      else if (items[i - 1].weight <= w)
        K[w][i] = std::max(items[i - 1].value + K[w - items[i - 1].weight][i - 1], K[w][i - 1]); // Get max
      else
        K[w][i] = K[w][i - 1]; // Set to prev
    } 
  } 

	//print final table - for debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { std::cout << items[n].weight << "," << items[n].value<<"   "; }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << K[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
  //end do not delete this code 

	//figure out which items are in the bag based on the table
	std::vector<int> bag;
  int temp = W; // Set to max weight
  for (int i = num_items; i > 0 && temp > 0; i--) 
  {
    if (K[temp][i] == K[temp][i - 1])
      continue; // Not in result
    else
    {
      bag.push_back(i - 1); // Add index to result
      temp -= items[i - 1].weight; // Subtract weight
    }
  }
	return bag;
}

////////////////////////////////////////////////////////////
int valueBag(std::vector<Item> const& items, std::vector<int> const& bag)
{
	std::vector<int>::const_iterator it = bag.begin(), it_e = bag.end();

	int accum = 0;
	//std::cout << "Bag ";
	for ( ; it != it_e; ++it) { 
		accum += items[ *it ].value; 
		//std::cout << *it << " ";
	}
	//std::cout << std::endl;
	return accum;
}

////////////////////////////////////////////////////////////
//prototype
//notice that auxiliary function returns value of the vector of items
//the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux( std::vector<Item> const&, int const&, int, Table& );

////////////////////////////////////////////////////////////
//function to kick start
std::vector<int> knapsackRecMem(std::vector<Item> const& items, int const& W )
{
	int num_items = items.size(); // Size of items
  Table dp(W + 1); // Create table with rows
  
  for (int i = 0; i < W + 1; i++)
    dp[i] = std::vector<int>(num_items); // Create columns for table

  for (int i = 0; i < num_items; i++) 
    for (int j = 0; j < W + 1; j++)
      dp[j][i] = -1; // Inititalize to -1

  knapsackRecMemAux(items, W, num_items - 1, dp); // Recursive call

  Table T(W + 1);
  for (int i = 0; i < W + 1; i++)
    T[i] = std::vector<int>(num_items + 1); // Create columns for table

  // Fix values for the table
  for(int i = 0; i < W + 1; ++i)
  {
    for(int j = 0; j <= num_items; ++j)
    {
      if(i == 0 || j == 0)
        T[i][j] = 0;
      else
        T[i][j] = dp[i][j - 1];
    }
  }

	//print table - debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { 
            std::cout << items[n].weight << "," << items[n].value<<"   "; 
        }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << T[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 

	//figure out which items are in the bag based on the table
	std::vector<int> bag;
  int temp = W; // Set to max weight
  for (int i = num_items; i > 0 && temp > 0; i--) 
  {
    if (T[temp][i] == T[temp][i - 1])
      continue; // Not in result
    else
    {
      bag.push_back(i - 1); // Add index to result
      temp -= items[i - 1].weight; // Subtract weight
    }
  }
	return bag;
}

////////////////////////////////////////////////////////////
//the real recursive function
int knapsackRecMemAux(std::vector<Item> const& items, int const& W, int i, Table& dp)
{
  if (i < 0)
    return 0; // Base case
  if (dp[W][i] != -1)
    return dp[W][i]; // Base case

  if (items[i].weight > W)
  {
    dp[W][i] = knapsackRecMemAux(items, W, i - 1, dp); // Store value
    return dp[W][i];
  }
  else
  {
    dp[W][i] = std::max(items[i].value + knapsackRecMemAux(items, W - items[i].weight, i - 1, dp), knapsackRecMemAux(items, W, i - 1, dp)); // Store max value
    return dp[W][i]; 
  } 
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
