#include "tsp.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <fstream>

int TotalCity; // Global for size of city
std::vector<int> final_path; // Final tour
bool* visited; // Array of already visited nodes
int final_res = std::numeric_limits<int>::max(); // Final minimum weight
std::vector<int> second_min;

// Copy solution to final_path
void copyToFinal(int curr_path[]) 
{ 
  final_path.clear(); // Clear final path
  for (int i = 0; i < TotalCity; ++i)
    final_path.push_back(curr_path[i]); // Push nodes back
  final_path.push_back(curr_path[0]); // Push first node back
}

int getLength(const std::vector<int>& sol, const MAP& map)
{
  int prev = 0; // Prev node
  int total = 0; // Total counter

  for(int i = 1; i < TotalCity + 1; ++i)
  {
    total += map[prev][sol[i]]; // Add to total
    prev = sol[i]; // Set prev to curr
  }
  return total; // Return this tour length
}

// Minimum with edge at i
int firstMin(std::vector<std::vector<int>> adj, int i) 
{
  int min = std::numeric_limits<int>::max(); 
  for (int k = 0; k < TotalCity; ++k) 
    if (adj[i][k] < min && i != k) 
      min = adj[i][k]; // Set to minimum 
  return min; 
}
  
// Second minimum with edge at i
int secondMin(std::vector<std::vector<int>> adj, int i) 
{ 
  int first = std::numeric_limits<int>::max(), second = std::numeric_limits<int>::max(); 
  for (int j = 0; j < TotalCity; ++j) 
  {
    if (i == j) 
      continue; // If same edge, discard
  
    if (adj[i][j] <= first)
    {
      second = first; // Set second to first
      first = adj[i][j]; // Set this to first
    } 
    else if (adj[i][j] <= second && adj[i][j] != first) 
      second = adj[i][j]; // Set this to second
  } 
  return second; 
}

void secondMin(std::vector<std::vector<int>> adj) 
{ 
  for(int i = 0; i < TotalCity; ++i)
  {
    int first = std::numeric_limits<int>::max(), second = std::numeric_limits<int>::max(); 
    for (int j = 0; j < TotalCity; ++j) 
    {
      if (i == j) 
        continue; // If same edge, discard
  
      if (adj[i][j] <= first)
      {
        second = first; // Set second to first
        first = adj[i][j]; // Set this to first
      } 
      else if (adj[i][j] <= second && adj[i][j] != first) 
        second = adj[i][j]; // Set this to second
    } 
    second_min.push_back(second); 
  }
  
}

// Recursive function to calculate shortest tour
void TSPRec(std::vector<std::vector<int>> adj, int curr_bound, int curr_weight, int level, int curr_path[]) 
{
  if (level == TotalCity) // Level == TotalCity means all nodes covered
  {
    if (adj[curr_path[level-1]][curr_path[0]] != 0) // Check if there is an edge from last to first
    {
      int curr_res = curr_weight + adj[curr_path[level-1]][curr_path[0]]; // Current solution weight
      if (curr_res < final_res)
      { 
        //std::cout << curr_res << std::endl;
        copyToFinal(curr_path); // Copy to final path if better
        final_res = curr_res; 
      }
    }
    return; 
  }
  
  for (int i = 0; i < TotalCity; ++i) // Build search space
  {
    if (adj[curr_path[level - 1]][i] != 0 && visited[i] == false) // Check next vertex (diagonal and not visited)
    {
      int temp = curr_bound;
      curr_weight += adj[curr_path[level - 1]][i]; 
  
      curr_bound -= ((second_min[curr_path[level - 1]]) + second_min[i] / 2); // Other levels 

      if (curr_bound + curr_weight < final_res) // If total weight less than final weight
      {
        curr_path[level] = i; // Set level to i
        visited[i] = true; // Set as visited
        TSPRec(adj, curr_bound, curr_weight, level + 1, curr_path); // Call recursive function for the next level 
      }

      curr_weight -= adj[curr_path[level-1]][i]; // Reset current weight
      curr_bound = temp; // Reset current bound

      for(int j = 0; j < TotalCity; ++j)
        visited[j] = false; // Reset visited array

      for (int j = 0; j <= level - 1; ++j) 
        visited[curr_path[j]] = true; // Set nodes to visited
    } 
  } 
} 

std::vector<int> SolveTSP(char const* filename)
{
  std::vector<std::vector<int>> cities; // Matrix (Vector of vector of ints) of all distances

  std::ifstream in(filename, std::ifstream::in); // Open file
  if(!in.is_open())
    return final_path; // Return empty vector

  in >> TotalCity; // Read in the number of cities

  for(int i = 0; i < TotalCity; ++i)
  {
    std::vector<int> row; // Current row
    for(int j = 0; j < TotalCity; ++j)
      row.push_back(std::numeric_limits<int>::max()); // Push back INT_MAX (No path from here to here)

    cities.push_back(row); // Push back entire row
  }

  for(int i = 0; i < TotalCity; ++i)
  {
    for(int j = i + 1; j < TotalCity; ++j)
    {
      if(!in.good())
        return final_path; // Return empty vector
      in >> cities[i][j]; // Read in number
      cities[j][i] = cities[i][j]; // Set opposite thingy to the same value (bidrectional)
    }
  }

  secondMin(cities);

  visited = new bool [TotalCity]; // Create visited nodes array
  int* curr_path = new int [TotalCity + 1]; // Create current path array
  int curr_bound = 0; // Set current bound to 0

  for(int i = 0; i < TotalCity; ++i)
  {
    curr_path[i] = -1; // Init current paths to -1
    visited[i] = false; // Init all visited nodes to false
  }
  
  for (int i = 0; i < TotalCity; ++i)
    curr_bound += firstMin(cities, i) + second_min[i]; // Compute initial bound

  curr_bound /= 2;
  visited[0] = true; // Set first vertex to visited
  curr_path[0] = 0; // Set current path to 0
  TSPRec(cities, curr_bound, 0, 1, curr_path); // Recursive function call for weight = 0 and level = 1

  delete[] visited;
  delete[] curr_path;
  return final_path; // Return best possible trip
}