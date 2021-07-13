#include "Camera.h"

float Camera::FRICTION = 0.1f;
float Camera::FORCE = 1e1f;

sf::View Camera::getView() {
	return this->view;
}

Entity *Camera::getFollowed() {
	return this->followed;
}

void Camera::clearMovement() {
	this->acceleration = sf::Vector2f(0.0f, 0.0f);
	this->velocity = sf::Vector2f(0.0f, 0.0f);
}

void Camera::setFollowed(Entity *followed) {
	this->follow = true;
	this->followed = followed;
}

void Camera::disableFollow() {
	this->follow = false;
}

void Camera::toggleFollow() {
	this->follow = !this->follow;
}

void Camera::zoom(float delta) {
	if (delta == 0.0f)
		return;

	sf::Vector2f size = this->view.getSize();

	if (delta > 0) {
		size.x /= delta;
		size.y /= delta;
	}
	else {
		size.x *= -delta;
		size.y *= -delta;
	}

	this->view.setSize(size);
}

void Camera::move(Direction d) {
	float div = this->view.getSize().x / 1e3f;
	float force = Camera::FORCE * div;

	switch (d) {
		case Direction::UP:
			this->accelerate(sf::Vector2f(0.0f, -force));
			break;
		case Direction::RIGHT:
			this->accelerate(sf::Vector2f(force, 0.0f));
			break;
		case Direction::DOWN:
			this->accelerate(sf::Vector2f(0.0f, force));
			break;
		case Direction::LEFT:
			this->accelerate(sf::Vector2f(-force, 0.0f));
			break;
		default:
			break;
	}
}

void Camera::update() {
	if (this->follow)
		this->position = this->followed->getPosition();
	else {
		Entity::update();
		this->acceleration = sf::Vector2f(-this->velocity.x * Camera::FRICTION, 
										  -this->velocity.y * Camera::FRICTION);
	}

	this->view.setCenter(this->position);
}

Camera::Camera()
	:Entity(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f), 1.0f),
	follow(false), view(sf::View()), followed(nullptr) {
}

Camera::Camera(sf::RenderWindow *window)
	:Entity(window->getView().getCenter(), sf::Vector2f(0.0f, 0.0f), 1.0f), 
	follow(false), view(window->getView()), followed(nullptr) {
}

Camera::~Camera() {
	
}
