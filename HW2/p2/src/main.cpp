
#include <chrono>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>

constexpr size_t gMag = 10'000UL; // size of the 2D array (gMag x gMag)

using Vector2D = std::vector<std::vector<uint8_t>>;

int main(int argc, char** argv) {
	using hrc = std::chrono::high_resolution_clock;

	Vector2D test1;
	test1.resize(gMag);
	std::for_each(test1.begin(), test1.end(), [](auto&& t){ t.resize(gMag); });

	//// COLUMN MAJOR TEST ////
	auto start = hrc::now();
	for (int i = 0; i < gMag; ++i) {
		for (int j = 0; j < gMag; ++j) {
			test1[i][j] = 5; // sets each element to 5
		}
	}
	auto end = hrc::now();

	test1.clear(); // this will in-turn clean all the inner vectors

	std::chrono::duration<double, std::milli> elapse = end - start;
	std::cout << "Column major test took: " << elapse.count() << "ms" << std::endl;



	Vector2D test2;
	test2.resize(gMag);
	std::for_each(test2.begin(), test2.end(), [](auto&& t){ t.resize(gMag); });

	//// ROW MAJOR TEST ////
	start = hrc::now();
	for (int i = 0; i < gMag; ++i) {
		for (int j = 0; j < gMag; ++j) {
			test2[j][i] = 5;
		}
	}
	end = hrc::now();

	elapse = end - start;
	std::cout << "Row major test took: " << elapse.count() << "ms" << std::endl;
}