/*
#include <ball.h>

#include <maths.h>

Ball::Ball()
{
	sphere_collider = Sphere({ 0, 0, 0 }, 0.5f);
	mass = 1.0f;
	velocity = { 0, 0, 0 };
}

Ball::Ball(vec3 position, float radius, float mass, vec3 velocity)
{
	sphere_collider = Sphere(position, radius);
	this->mass = mass;
	this->velocity = velocity;

	energy = 0.5f * mass + mass * GRAVITY * position.y;
}

vec3 Ball::computeNextPosition(float deltaTime)
{
	float height = sphere_collider.center.y;

	float speed = sqrtf(fabsf((2 * (energy - mass * GRAVITY * height)) / mass));

	// Update velocity direction (influenced by gravity)
	// Normalize so we have direction multiplied by calculated speed (so 0 energy lost)
	velocity = speedFromNewton(normalized(velocity) * speed, vec3{ 0.f, GRAVITY, 0.f }, deltaTime);

	// New sphere position if no collision
	return positionFromNewton(sphere_collider.center, velocity, deltaTime);
}


void Ball::updatePosition(vec3 position)
{
	sphere_collider.center = position;
}
*/