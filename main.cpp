
#include <iostream>
#include <string>
using namespace std;

#include "include/olcConsoleGameEngine.h"


struct sBall{
	float px, py;
	float vx, vy;
	float ax, ay;
	float radius;

	int id;
};


class CirclePhysics: public olcConsoleGameEngine{

public:
	CirclePhysics(){
		m_sAppName = L"こんにちは";
	}


private:
	vector<pair<float, float>> modelCircle;
	vector<sBall> vecBalls;
	sBall *pSelectedBall = nullptr;

	void AddBall(float x, float y, float r = 5.0f){
		sBall b;

		b.px = x,
		b.py = y;

		b.vx = 0;
		b.vy = 0;

		b.ax = 0;
		b.ay = 0;

		b.radius = r;
		b.id = vecBalls.size();

		vecBalls.emplace_back(b);
	}

	bool DoOverlap(
		float x1, 
		float y1,
		float r1,
		float x2,
		float y2, 
		float r2
	){
		float distance_squared = pow(x1 - x2, 2) + pow(y1 - y2, 2);
		float sum_radius_squared = pow(r1 + r2, 2);

		if (distance_squared <= sum_radius_squared){
			return true;
		}

		return false;
	}

	float GetDistanceBetweenBallCenters(
		float x1, 
		float y1,
		float x2,
		float y2
	){
		float distance_squared = pow(x1 - x2, 2) + pow(y1 - y2, 2);
		float distance = sqrtf(distance_squared);

		return distance;
	}

	bool IsPointInsideCircle(
		float x, 
		float y,
		float r,
		float x_point,
		float y_point
	){
		float distance_squared = pow(x - x_point, 2) + pow(y - y_point, 2);

		if (distance_squared <= pow(r, 2)){
			return true;
		}

		return false;
	}


public: 
	bool OnUserCreate(){
		modelCircle.push_back({0.0f, 0.0f});
		int nPoints = 40;
		for (int i = 0; i < nPoints; i++){
			modelCircle.push_back(
				{
					cosf(i / (float)(nPoints - 1) * 2.0f * 3.14159f),
					sinf(i / (float)(nPoints - 1) * 2.0f * 3.14159f)
				}
			);
		}

		float fDefaultRad = 20.0f;
		AddBall(ScreenWidth() * 0.25f, ScreenHeight() * 0.5f, fDefaultRad);
		AddBall(ScreenWidth() * 0.75f, ScreenHeight() * 0.5f, fDefaultRad);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime){
		if (m_mouse[0].bPressed){
			pSelectedBall = nullptr;
			for (auto &ball: vecBalls){
				if (
					IsPointInsideCircle(
						ball.px,
						ball.py,
						ball.radius,
						m_mousePosX,
						m_mousePosY
					)
				){
					pSelectedBall = &ball;
					break;
				}
			}
		}

		if (m_mouse[0].bHeld){
			if (pSelectedBall != nullptr){
				pSelectedBall->px = m_mousePosX;
				pSelectedBall->py = m_mousePosY;
			} 
		}

		if (m_mouse[0].bReleased){
			pSelectedBall = nullptr;
		}

		for (auto &ball: vecBalls){
			for (auto &target: vecBalls){
				// Skipping if we are comparing the same balls
				if (ball.id == target.id){
					continue;
				}

				// Skipping if we have no collision
				if (
					not DoOverlap(
						ball.px,
						ball.py,
						ball.radius,
						target.px,
						target.py,
						target.radius
					)
				){
					continue;
				}

				float distance_between_centers = GetDistanceBetweenBallCenters(
					ball.px,
					ball.py,
					target.px,
					target.py
				);
				float overlap = - 0.5f * (ball.radius + target.radius - distance_between_centers);

				// displace current ball
				ball.px -= overlap * (ball.px - target.px) / distance_between_centers;
				ball.py -= overlap * (ball.py - target.py) / distance_between_centers;

				// displace target ball
				target.px += overlap * (target.px - target.px) / distance_between_centers;
				target.py += overlap * (target.py - target.py) / distance_between_centers;
				
			}
		}


		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');

		for (auto ball: vecBalls){
			DrawWireFrameModel(
				modelCircle, 
				ball.px,
				ball.py,
				atan2f(ball.vy, ball.vx),
				ball.radius,
				FG_WHITE
			);
		}

		return true;
	}
};

int main(){
	CirclePhysics game;

	if (game.ConstructConsole(160 * 8, 120 * 8, 1, 1)){
		game.Start();
	} else {
		wcout << "Could not constrict console" << endl;
	}

	return 0;
};