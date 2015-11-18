#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdlib>

class Utils{
public:
	Utils() {}
	static float randBounded(float min, float max) {
		float random = (float) rand() / (float) RAND_MAX;
		float delta = max - min;

		return min + (random * delta);
	}
};

#endif // UTILS_HPP
