#include "Particle.hpp"

Particle::Particle() {
	alive_ = true;
	life_ = 1.0f;
	fade_ = 0.1f;
	position_ = glm::vec3(0.0f);
	direction_= glm::vec3(0.0f);
}

Particle::Particle(float life, float fade, glm::vec3 position) {
	alive_ = true;
	life_ = life;
	fade_ = fade;
	position_ = position;
	direction_= glm::vec3(0.0f);
}

void Particle::kill() {
	alive_ = false;
}

void Particle::live() {
	alive_ = true;
}

void Particle::decreaseLife() {
	life_-= fade_;
	if (life_ <= 0) {
		kill();
	}
}
