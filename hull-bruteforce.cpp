#include "hull-bruteforce.h"
#include <algorithm>
#include <stack>
#include <iostream>

bool Point::operator==(Point const& arg2) const
{
  return ((x==arg2.x) && (y==arg2.y));
}

std::ostream& operator<<(std::ostream& os, Point const& p)
{
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>>(std::istream& os, Point & p)
{
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//return value is (on left, on right)
std::pair<bool,bool> get_location(
		float const& p1x, //check which side of the line (p1x,p1y)-->(p2x,p2y) 
		float const& p1y, //point (qx,qy) is on
		float const& p2x,
		float const& p2y,
		float const& qx,
		float const& qy
		) 
{
	Point dir   = {p2x-p1x,p2y-p1y};
	Point norm  = {dir.y, -dir.x};
	Point point = {qx-p1x,qy-p1y};
	//scalar product is positive if on right side
	float scal_prod = norm.x*point.x + norm.y*point.y;
	return std::make_pair( (scal_prod<0), (scal_prod>0));
}

// Check the orientation of 3 points, 0 if colinear, 1 if clockwise, 2 if counterclockwise
int orientation(Point p, Point q, Point r) 
{ 
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0)
      return 0;  // colinear 
    return (val > 0) ? 1: 2; // clock (1) or counterclock (2) wise 
}

// Returns a set of indices of points that form convex hull
std::set<int> hullBruteForce(std::vector<Point>const& points)
{
  int num_points = points.size(); // Size of points
  if (num_points < 3)
    throw "bad number of points"; // If less than 3, this is kinda useless

  // Result set (all indices)
  std::set<int> hull_indices;

  for (unsigned i = 0; i < points.size(); ++i) // For all points
  {
    for (unsigned j = i + 1; j < points.size(); ++j) // For all points
    {
      int x1 = points[i].x; // Get x at i
      int y1 = points[i].y; // Get y at i
      int x2 = points[j].x; // Get x at j
      int y2 = points[j].y; // Get y at j

      int a1 = y1 - y2; // Get the y-difference (a value)
      int b1 = x2 - x1; // Get the x-difference (b value)
      int c1 = (x1 * y2) - (y1 * x2); // Get the c value

      unsigned right = 0; // Number on right
      unsigned left = 0; // Number on left
      unsigned onLine = 0; // Number on the line

      for (unsigned k = 0; k < points.size(); ++k) // For all points
      {
        if (a1 * points[k].x + b1 * points[k].y + c1 < 0)
        {
          left++; // If on left, increment this
          if(right > 0)
            break;
        }
        else if (a1 * points[k].x + b1 * points[k].y + c1 > 0)
        {
          right++; // If on right, increment this
          if(left > 0)
            break;
        }
        else if (a1 * points[k].x + b1 * points[k].y + c1 == 0)
          onLine++;
      }

      if (right + onLine == points.size() || left + onLine == points.size())
      {
        // If either are the size of the number of points, then all points lie on one side
        hull_indices.insert(i); // Add i to indices
        hull_indices.insert(j); // Add j to indices
      }
    }
  }

  return hull_indices; // Return the set of indices
}

// Returns a vector of indices that form covex hull
std::vector<int> hullBruteForce2(std::vector<Point>const& points)
{
	int num_points = points.size(); // Size of points
	if (num_points < 3)
    throw "bad number of points"; // If less than 3, this is kinda useless

	// Vector of indices for the result
  std::vector<int> hull_indices;
  
  int leftmost = 0; // Find the leftmost point, smallest x
  for (int i = 1; i < num_points; ++i) 
    if (points[i].x < points[leftmost].x)
      leftmost = i; // x is to the left, if less than current leftmost
  
  // Go counterclockwise until we reach leftmost point again 
  int curr = leftmost; // Start from leftmost
  int point; // Point to check counterclockwise-itivity
  do
  { 
    hull_indices.push_back(curr); // Push back current point into vector
  
    point = (curr + 1) % num_points; // Set point
    for (int i = 0; i < num_points; ++i) 
    {
      // If counterclockwise, update point
      if (orientation(points[curr], points[i], points[point]) == 2) 
        point = i;
    }
    curr = point; // Use point as current and move to next set
  }
  while (curr != leftmost); // While not looped back to start
	
  return hull_indices; // Return vector of indices
}
