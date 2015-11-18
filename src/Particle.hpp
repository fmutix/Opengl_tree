#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/vec3.hpp>

class Particle {
public:
	Particle();
	Particle(float life, float fade, glm::vec3 position);

	void fall();
	void kill();
	void live();
	void decreaseLife();

	glm::vec3 getPosition();
	float getLifeMax();
	float getLife();

private:
	bool alive_;
	bool grown_ = false;
	float lifeMax_;
	float life_;
	float fade_;
	glm::vec3 position_;
	glm::vec3 direction_;
};

#endif // PARTICLE_HPP
