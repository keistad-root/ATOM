#include<array>
#include<numeric>
#include<iostream>

int main() {
	std::array<int, 10> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::cout << std::accumulate(arr.begin() + 1, arr.end(), 0) << std::endl;

	return 0;
}