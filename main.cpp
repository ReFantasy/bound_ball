#include <iostream>
#include "render_ball.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <Eigen/Dense>
#include <filesystem>


Eigen::Vector3f g{ 0,-9.8,0 };//acceleration of gravity

struct S
{
	Eigen::Vector3f position = Eigen::Vector3f::Zero();
	Eigen::Vector3f velocity = Eigen::Vector3f::Zero();
};

bool CollisionBetween(S s_old, S s_new)
{
	return false;
}


S CollisionResponse(S s_new)
{
	// just revert velocity
	s_new.velocity = -s_new.position;
	return s_new;
}

S Integrate(S s, Eigen::Vector3f a, float Timestep)
{
	S s_new = s;
	s_new.position = s_new.position + s_new.velocity * Timestep;
	s_new.velocity = s_new.velocity + a * Timestep;
	return s_new;
}



float h = 0.01;            // timestep
int n = 0;                 // step number
float t = 0;               // current time
float t_max = 600000000.f; // end time
S s;                       // initial position and velocity

int main()
{
	Renderer ball;

	Eigen::Vector3f wind_velocity{ 0.3,0,0 };      // Wind speed
	float arc = 0.4f;                        // Air Resistance coefficient
	float ball_mass = 0.01f;                         // weight of ball


	while (t < t_max)
	{
		//printf("current speed: %.4f,%.4f,%.4f\n", s.velocity.x(), s.velocity.y(), s.velocity.z());
		if (!ball.Render(glm::vec3(s.position.x(), s.position.y(), s.position.z()))) break;

		float TimestepRemaining = h;
		float Timestep = TimestepRemaining;
		while (TimestepRemaining > 0)
		{
			// determine accelerations
			Eigen::Vector3f a = g + arc / ball_mass * (wind_velocity - s.velocity);

			// integrate from state s using derivative dots for time Timestep
			auto s_new = Integrate(s, a, Timestep);

			if (CollisionBetween(s, s_new))
			{
				// calculate first collision and reintegrate
				float f = 0.2;

				Timestep = f * Timestep;
				s_new = Integrate(s, a, Timestep);

				s_new = CollisionResponse(s_new);
			}

			TimestepRemaining = TimestepRemaining - Timestep;

			s = s_new;
		}

		n = n + 1;
		t = n * h;
	}

	return 0;
}

