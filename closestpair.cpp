#include "closestpair.h"
#include <algorithm>
#include <cmath>
#include <iostream>

std::ostream& operator<< (std::ostream& os, Point const& p)
{
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p)
{
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux(std::vector<Point> pointsX, std::vector<Point> pointsY);
bool compareX (Point p, Point q) { return p.x < q.x; }
bool compareY (Point p, Point q) { return p.y < q.y; }
float dist(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

////////////////////////////////////////////////////////////////////////////////
float closestPair(std::vector<Point> const& points)
{
	int size = points.size();

	if (size == 0)
    throw "zero size subset";
	if (size == 1)
    return std::numeric_limits<float>::max();

  std::vector<Point> pointsX, pointsY; // Create 2 vectors, set to points
  pointsX = pointsY = points;
  std::sort(pointsX.begin(), pointsX.end(), compareX); // Sort by x-values
  std::sort(pointsY.begin(), pointsY.end(), compareY); // Sort by y-values

  return closestPair_aux(pointsX, pointsY); // Return auxiliary func
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux(std::vector<Point> pointsX, std::vector<Point> pointsY)
{
	int n = pointsX.size(); // Get size

	if (n == 0)
    throw "zero size subset";
	if (n == 1)
    return std::numeric_limits<float>::max();

  if (n <= 3) // Base case (brute force)
  {
    float min = std::numeric_limits<float>::max(); // Set min to float max
    for (int i = 0; i < n; ++i)
      for (int j = i + 1 ; j < n; ++j)
        if (dist(pointsX[i], pointsX[j]) < min)
          min = dist(pointsX[i], pointsX[j]); // If dist is less, set to min
    return min; // Return smallest dist
  }

  int mid = n / 2; // Find midpoint
  Point mid_point = pointsX[mid]; // Get that point

  //  Divide the set in two equal-sized parts (left and right).
  std::vector<Point> pointsY_left(pointsY.cbegin(), pointsY.cbegin() + mid);
  std::vector<Point> pointsY_right(pointsY.cbegin() + mid, pointsY.cend());
  std::vector<Point> pointsX_left(pointsX.cbegin(), pointsX.cbegin() + mid);
  std::vector<Point> pointsX_right(pointsX.cbegin() + mid, pointsX.cend());

  // Get smaller between left and right (recursive)
  float d_left = closestPair_aux(pointsX_left, pointsY_left);
  float d_right = closestPair_aux(pointsX_right , pointsY_right);

  float d = std::min(d_left, d_right); // Get minimum between both sides

  std::vector<Point> strip; // Remove all points farther than d
  for (int i = 0; i < n; ++i)
    if (abs(pointsX[i].x - mid_point.x) < d) // Check same point
      strip.push_back(pointsX[i]);

  for (int i = 0; i < int(strip.size()); ++i)
    for (int j = i + 1; j < int(strip.size()) && (strip[j].y - strip[i].y) < d; ++j)
      if (dist(strip[i], strip[j]) < d)
        d = dist(strip[i], strip[j]); // Smallest distance between the point and all of its neighbors

  strip.clear(); // Clear the strip
  for (int i = 0; i < n; ++i)
    if (abs(pointsY[i].x - mid_point.x) < d) // Check across left and right (Eg: test 6)
      strip.push_back(pointsY[i]);

  for (int i = 0; i < int(strip.size()); ++i)
    for (int j = i + 1; j < int(strip.size()) && (strip[j].y - strip[i].y) < d; ++j)
      if (dist(strip[i], strip[j]) < d)
        d = dist(strip[i], strip[j]); // Smallest distance between the point and all of its neighbors
     return d; // Return smallest between both
}
