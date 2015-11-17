#include "Particle.hpp"

Particle::Particle() {
	alive_ = true;
	lifeMax_ = 1.0f;
	life_ = 1.0f;
	fade_ = 0.01f;
	position_ = glm::vec3(0.0f);
	direction_= glm::vec3(0.0f);
}

Particle::Particle(float life, float fade, glm::vec3 position) {
	alive_ = true;
	lifeMax_ = life;
	life_ = life;
	fade_ = fade;
	position_ = position;
	direction_= glm::vec3(0.0f);
}

void Particle::kill() {
	alive_ = false;
	life_ = 0.0f;
}

void Particle::live() {
	alive_ = true;
	life_ = lifeMax_;
}

void Particle::decreaseLife() {
	life_ -= fade_;
	if (life_ <= 0) {
		kill();
	}
}

glm::vec3 Particle::getPosition() {
	return position_;
}

float Particle::getLifeMax() {
	return lifeMax_;
}

float Particle::getLife() {
	return life_;
}
