#include "e-dijkstra.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <functional>
#include <queue>
#include <set>

void addEdge(int u, int v, int w, std::list<std::pair<int, int>>* adj) // Add an edge to adjacency matrix
{
  adj[u].push_back(std::make_pair(v, w));
  adj[v].push_back(std::make_pair(u, w));
}

std::vector<int> shortestPath(int N, int src, std::list<std::pair<int, int>>* adj) // Return shortest path from source to all other vertices
{
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
  std::vector<int> dist(N, std::numeric_limits<int>::max());
  pq.push(std::make_pair(0, src));
  dist[src] = 0;
  while (!pq.empty())
  {
    int u = pq.top().second;
    pq.pop();
    std::list<std::pair<int, int>>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
      int v = (*i).first;
      int weight = (*i).second;
      if (dist[v] > dist[u] + weight)
      {
        dist[v] = dist[u] + weight;
        pq.push(std::make_pair(dist[v], v));
      }
    }
  }

  return dist;
}

bool e_dijkstra(char const *filename, int range)
{
  int N; // Number of locations
  int K; // Max recharges
  int M; // Number of edges

  std::ifstream in(filename, std::ifstream::in); // Open file
  if(!in.is_open())
    return false; // Return not valid

  in >> N; // Read in the number of locations
  in >> K; // Read in the number of recharges
  in >> M; // Read in the number of edges

  std::list<std::pair<int, int>>* adj = new std::list<std::pair<int, int>> [N];// = new std::list<std::pair<int, int>(N); // Allocate memory for adjacency matrix

  for(int i = 0; i < M; ++i)
  {
    int start_edge;
    int end_edge;
    int cost;
    in >> start_edge; // Edge that we start at
    in >> end_edge; // Edge that we move towards
    in >> cost; // Cost of that traversal
    addEdge(start_edge, end_edge, cost, adj); // Add edge to adjacency matrix
  }

  std::set<int> NotVisited;
  std::set<int> Visited;
  for(int i = 0; i < N; ++i)
  {
    std::vector<int> result = shortestPath(N, i, adj);
    for(int j = 0; j < static_cast<int>(result.size()); ++j)
      if(result[j] > range)
        NotVisited.insert(j);
      else
        Visited.insert(j);
  }

  for(auto it = Visited.begin(); it != Visited.end(); ++it)
    NotVisited.erase(*it);

  delete[] adj;

  if(NotVisited.empty())
    return true;
  else
  {
    while (!NotVisited.empty())
    {
      std::cout << ' ' << (*NotVisited.begin()) << std::endl;
      NotVisited.erase(NotVisited.begin());
    }
    return false;
  }
}
