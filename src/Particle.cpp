#include "Particle.hpp"
#include "Utils.hpp"

Particle::Particle() {
	ready_ = true;
	alive_ = false;
	ageMax_ = 1.0f;
	age_ = 1.0f;
	idle_ = 1.0f;
	idleMax_ = 1.0f;
	fade_ = 0.01f;
	position_ = glm::vec3(0.0f);
	direction_= glm::vec3(0.0f, -0.05f, 0.0f);
}

Particle::Particle(float age, float idle, float fade, glm::vec3 position) {
	ready_ = true;
	alive_ = false;
	ageMax_ = age;
	age_ = age;
	idle_ = idle;
	idleMax_ = idle;
	fade_ = fade;
	position_ = position;
	direction_= glm::vec3(0.0f, -0.005f, 0.0f);
}

void Particle::process() {
	if (alive_) {
		if (state_ == 0) {
			decreaseAge();
		}
		else if (state_ == 1) {
			fall();
		}
		else {
			decreaseIdle();
		}
	}
}

void Particle::fall() {
	position_ += direction_;
	if (position_.y <= 0) {
		position_.y = 0.0f;
		state_ = 2;
	}
}

void Particle::kill() {
	ready_ = false;
	alive_ = false;
	state_ = -1;
}

void Particle::live() {
	alive_ = true;
	age_ = ageMax_;
	idle_ = idleMax_;
	position_ = glm::vec3(
		Utils::randBounded(-1.17335, 1.1498) * 0.2f,
		Utils::randBounded(4.0, 7.49918) * 0.2f,
		Utils::randBounded(-1.45231, 1.7167) * 0.2f
	);
	state_ = 0;
}

void Particle::decreaseAge() {
	age_ -= fade_;
	if (age_ <= 0) {
		age_ = 0.0f;
		state_ = 1;
	}
}

void Particle::decreaseIdle() {
	idle_ -= fade_;
	if (idle_ <= 0) {
		idle_ = 0.0f;
		kill();
	}
}

bool Particle::getReady() {
	return ready_;
}

void Particle::setReady(bool ready) {
	ready_ = ready;
}

bool Particle::getAlive() {
	return alive_;
}

glm::vec3 Particle::getPosition() {
	return position_;
}

float Particle::getAgeMax() {
	return ageMax_;
}

float Particle::getAge() {
	return age_;
}
