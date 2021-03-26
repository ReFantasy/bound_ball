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

bool CollisionBetween(S s_old, S s_new, Eigen::Vector3f norm, Eigen::Vector3f pt, float radius = 1.0f)
{
	auto d2 = (s_new.position - pt).dot(norm);
	d2 -= radius;
	if (d2 > 0)
		return false;
	else
		return true;

	/*auto d1 = (s_old.position - pt).dot(norm);
	auto d2 = (s_new.position - pt).dot(norm);
	if (d1 * d2 < 0) return true;

	if (d1 > 0)
	{
		d1 -= radius;
	}
	else
	{
		d1 += radius;
	}
	if (d2 > 0)
	{
		d2 -= radius;
	}
	else
	{
		d2 += radius;
	}

	if (d1 * d2 < 0) 
		return true;
	else 
		return false;*/
}


S CollisionResponse(S s_new, Eigen::Vector3f ground_normal)
{
	
	/*s_new.velocity = -s_new.velocity*0.6;
	return s_new;*/
	Eigen::Vector3f pre_vn = s_new.velocity.dot(ground_normal)*ground_normal;
	Eigen::Vector3f pre_vt = s_new.velocity-pre_vn;

	float cr = 0.73;
	float u = 0.1;
	Eigen::Vector3f vn = -cr*pre_vn;

	Eigen::Vector3f vt = pre_vt-std::min(u*pre_vn.norm(),pre_vt.norm())*pre_vt.normalized();
	//Eigen::Vector3f vt = pre_vt*u;

	S res = s_new;
	res.velocity = (vn + vt);
	return res;
}

S Integrate(S s, Eigen::Vector3f a, float Timestep)
{
	S s_new = s;
	s_new.position = s_new.position + s_new.velocity * Timestep;
	s_new.velocity = s_new.velocity + a * Timestep;
	return s_new;
}



float h = 0.005;            // timestep
int n = 0;                 // step number
float t = 0;               // current time
float t_max = 600000000.f; // end time
S s;                       // initial position and velocity

int main()
{
	

	s.position = Eigen::Vector3f{ 0.0,10.0,-7.0 };
	s.velocity = Eigen::Vector3f{ 6.0,0.0,0.0 };
	Eigen::Vector3f wind_velocity{ 0.0,0,0 };      // Wind speed
	float arc = 0.4f;                              // Air Resistance coefficient
	float ball_mass = 0.5f;                        // weight of ball

	Eigen::Vector3f ground_normal{ 0,1,0 };
	Eigen::Vector3f ground_point{ 1,0,0 };

	//std::cin.get();

	Renderer ball;

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

			if (CollisionBetween(s, s_new, ground_normal, ground_point))
			{
				// calculate first collision and reintegrate
				float d1 = (s.position - ground_point).dot(ground_normal);
				float d2 = (s_new.position - ground_point).dot(ground_normal);
				float f = std::abs(d1) /(std::abs(d1)+ std::abs(d2));

				Timestep = f * Timestep;
				s_new = Integrate(s, a, Timestep);

				s_new = CollisionResponse(s_new, ground_normal);
			}

			TimestepRemaining = TimestepRemaining - Timestep;

			s = s_new;

			if (TimestepRemaining < 0.05 * h)break;
		}

		n = n + 1;
		t = n * h;
	}

	return 0;
}

