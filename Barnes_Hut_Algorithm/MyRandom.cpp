#include "MyRandom.h"

float MyRandom::getRandomFloat(float range_beg, float range_end) {
	std::random_device random_device;
	std::mt19937 gen(random_device());
	std::uniform_real_distribution<> distribution(range_beg, range_end);
	return distribution(gen);
}
