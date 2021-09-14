#ifndef PERM_JOHNSON_TROTTER_H
#define PERM_JOHNSON_TROTTER_H
#include <vector>

class PermJohnsonTrotter
{
	private:
    bool isMobile();
    bool isMobile(unsigned p);
    int findLargest();
    std::vector<int> digits;
    std::vector<int> directions;

	public:
		PermJohnsonTrotter(int size);
		bool Next();
		std::vector<int> const& Get() const;
};
#endif
