#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/vec3.hpp>

class Particle {
public:
	Particle();
	Particle(float age, float idle, float fade, glm::vec3 position);

	void process();
	void fall();
	void kill();
	void live();
	void decreaseAge();
	void decreaseIdle();

	bool getReady();
	void setReady(bool ready);
	bool getAlive();
	glm::vec3 getPosition();
	float getAgeMax();
	float getAge();

private:
	bool ready_;
	bool alive_;
	int state_ = 0; // 0: growing, 1: falling, 2: felt
	float ageMax_;
	float age_;
	float idleMax_;
	float idle_;
	float fade_;
	glm::vec3 position_;
	glm::vec3 direction_;
};

#endif // PARTICLE_HPP
