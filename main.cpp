
#include <iostream>
#include <string>
#include <random>
using namespace std;

#include "include/olcConsoleGameEngine.h"


struct sBall{
	float px, py;
	float vx, vy;
	float ax, ay;
	float radius;
	float mass;

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
		b.mass = r * 10.0f;

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

	float getRandomFloat(float min, float max) {
		// Create random engine and distribution
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(min, max);

		return dist(gen);
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

		float defaultRad = 50.0f;
		int numberOfBalls = 20;
		// AddBall(ScreenWidth() * 0.25f, ScreenHeight() * 0.5f, defaultRad);
		// AddBall(ScreenWidth() * 0.75f, ScreenHeight() * 0.5f, defaultRad);

		for (int i = 0; i < numberOfBalls; i++){
			AddBall(
				rand() % ScreenWidth(), 
				rand() % ScreenHeight(), 
				getRandomFloat(30.0f, 100.0f)
			);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime){
		if (m_mouse[0].bPressed || m_mouse[1].bPressed){
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

		if (m_mouse[1].bReleased){
			if (pSelectedBall != nullptr){
				pSelectedBall->vx = 5.0f * ((pSelectedBall->px) - (float)m_mousePosX);
				pSelectedBall->vy = 5.0f * ((pSelectedBall->py) - (float)m_mousePosY);
			}

			pSelectedBall = nullptr;
		}

		vector<pair<sBall*, sBall*>> vecCollidingPairs;

		// Update Ball Positions
		for (auto &ball: vecBalls){
			ball.ax = -ball.vx * 0.8f;
			ball.ay = -ball.vy * 0.8f;

			ball.vx += ball.ax * fElapsedTime;
			ball.vy += ball.ay * fElapsedTime;
			ball.px += ball.vx * fElapsedTime;
			ball.py += ball.vy * fElapsedTime;

			if (ball.px < 0){
				ball.px += (float)ScreenWidth();
			}
			if (ball.px >= ScreenWidth()){
				ball.px -= (float)ScreenWidth();
			}
			if (ball.py < 0){
				ball.py += (float)ScreenHeight();
			}
			if (ball.py >= ScreenHeight()){
				ball.py -= (float)ScreenHeight();
			}

			if (ball.vx * ball.vx + ball.vy * ball.vy < 0.01f){
				ball.vx = 0;
				ball.vy = 0;
			} 
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

				vecCollidingPairs.push_back({&ball, &target});

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
				target.px += overlap * (ball.px - target.px) / distance_between_centers;
				target.py += overlap * (ball.py - target.py) / distance_between_centers;
				
			}
		}

		// Work out dynamic collistions
		for (auto c : vecCollidingPairs){
			sBall *b1 = c.first;
			sBall *b2 = c.second;

			float distance = GetDistanceBetweenBallCenters(
				b1->px,
				b1->py,
				b2->px,
				b2->py
			);

			float normalX = (b2->px - b1->px) / distance;
			float normalY = (b2->py - b1->py) / distance;

			float tangentX = -normalY;
			float tangentY = normalX;

			float dpTan1 = b1->vx * tangentX + b1->vy * tangentY;
			float dpTan2 = b2->vx * tangentX + b2->vy * tangentY;

			float dpNorm1 = b1->vx * normalX + b1->vy * normalY;
			float dpNorm2 = b2->vx * normalX + b2->vy * normalY;

			float momentum1 = (dpNorm1 * (b1->mass - b2->mass) + 2.0f * b2->mass * dpNorm2) / (b1->mass + b2->mass);
			float momentum2 = (dpNorm2 * (b2->mass - b1->mass) + 2.0f * b1->mass * dpNorm1) / (b1->mass + b2->mass);

			b1->vx = tangentX * dpTan1 + normalX * momentum1;
			b1->vy = tangentY * dpTan1 + normalY * momentum1;

			b2->vx = tangentX * dpTan2 + normalX * momentum2;
			b2->vy = tangentY * dpTan2 + normalY * momentum2;
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

		for (auto collidingBallsPair: vecCollidingPairs){
			DrawLine(
				collidingBallsPair.first->px,
				collidingBallsPair.first->py,
				collidingBallsPair.second->px,
				collidingBallsPair.second->py,
				PIXEL_SOLID,
				FG_RED
			);
		}

		if (pSelectedBall != nullptr){
			DrawLine(
				pSelectedBall->px,
				pSelectedBall->py,
				m_mousePosX,
				m_mousePosY,
				PIXEL_SOLID,
				FG_BLUE
			);
		}

		return true;
	}
};

int main(){
	CirclePhysics game;

	if (game.ConstructConsole(160 * 8, 100 * 8, 1, 1)){
		game.Start();
	} else {
		wcout << "Could not constrict console" << endl;
	}

	return 0;
};