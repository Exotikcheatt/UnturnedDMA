#pragma once
#include "vector.h"

class Skeleton
{
private:
	Vector3 HEAD, NECK, lSHOULDER, rSHOULDER, HIP, lHIP, rHIP, lFOOT, rFOOT, LINE;

	Vector3 rotatex(Vector3 origin, Vector3 points, float angle) {
		points -= origin;
		angle = angle * 3.14159f / 180.0;
		float temp = points.y;
		points.y = points.y * cos(angle) - points.z * sin(angle);
		points.z = temp * sin(angle) + points.z * cos(angle);
		return points + origin;
	}
	Vector3 rotatey(Vector3 origin, Vector3 points, float angle) {
		points -= origin;
		angle = (angle * 3.14159f) / 180.0;
		float temp = points.z;
		points.z = points.z * cos(angle) - points.x * sin(angle);
		points.x = temp * sin(angle) + points.x * cos(angle);
		return points + origin;
	}
	Vector3 rotatez(Vector3 origin, Vector3 points, float angle) {
		points -= origin;
		angle = angle * 3.14159f / 180.0;
		float temp = points.x;
		points.x = points.x * cos(angle) - points.y * sin(angle);
		points.y = temp * sin(angle) + points.y * cos(angle);
		return points + origin;
	}
public:
	Skeleton() {}
	Skeleton(Vector3 baseBone, Vector2 looking, unsigned int stance)
	{
		switch (stance)
		{
		case 0:
		{
			lFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(-0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			rFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			lHIP = lFOOT + Vector3(0, 0.75f, 0);
			rHIP = rFOOT + Vector3(0, 0.75f, 0);
			HIP = baseBone + Vector3(0, 0.75f, 0);
			lSHOULDER = rotatey(lHIP, rotatex(lHIP, lHIP + Vector3(-0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			rSHOULDER = rotatey(rHIP, rotatex(rHIP, rHIP + Vector3(0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			NECK = rotatey(HIP, rotatex(HIP, HIP + Vector3(0, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			HEAD = rotatey(NECK, rotatex(NECK, NECK + Vector3(0, 0.4f, 0), (looking.x - 90.f) * 1.0f), looking.y);
			if (Camera.translation.distance(HEAD) < 25.f)
			{
				if (Camera.translation.distance(HEAD) > 5.f)
					LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, Camera.translation.distance(HEAD) - 5.f), (looking.x - 90.f)), looking.y);
				else
					LINE = HEAD;
			}
			else
				LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, 20.f), (looking.x - 90.f)), looking.y);
		}break;
		case 1:
		{
			Vector3 temp = baseBone + Vector3(0, 1.f, 0);
			lFOOT = rotatey(temp, rotatex(temp, temp + Vector3(-0.2f, 0.12f, -1.0f), (looking.x - 90.f) * 0.f), looking.y);
			rFOOT = rotatey(temp, rotatex(temp, temp + Vector3(0.2f, 0.12f, -1.0f), (looking.x - 90.f) * 0.f), looking.y);
			lHIP = rotatey(lFOOT, rotatex(lFOOT, lFOOT + Vector3(0, 0.f, 0.7f), (looking.x - 90.f) * 0.f), looking.y);
			rHIP = rotatey(rFOOT, rotatex(rFOOT, rFOOT + Vector3(0, 0.f, 0.7f), (looking.x - 90.f) * 0.f), looking.y);
			HIP = rotatey(temp, rotatex(temp, temp + Vector3(0, 0.12f, -0.3f), (looking.x - 90.f) * 0.f), looking.y);
			lSHOULDER = rotatey(lHIP, rotatex(lHIP, lHIP + Vector3(0, 0.125f, 0.7f), (looking.x - 90.f) * 0.5f), looking.y);
			rSHOULDER = rotatey(rHIP, rotatex(rHIP, rHIP + Vector3(0, 0.125f, 0.7f), (looking.x - 90.f) * 0.5f), looking.y);
			NECK = rotatey(HIP, rotatex(HIP, HIP + Vector3(0, 0.125f, 0.7f), (looking.x - 90.f) * 0.5f), looking.y);
			HEAD = rotatey(NECK, rotatex(NECK, NECK + Vector3(0, 0.3f, 0.2f), (looking.x - 90.f) * 1.0f), looking.y);
			if (Camera.translation.distance(HEAD) < 25.f)
			{
				if (Camera.translation.distance(HEAD) > 5.f)
					LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, Camera.translation.distance(HEAD) - 5.f), (looking.x - 90.f)), looking.y);
				else
					LINE = HEAD;
			}
			else
				LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, 20.f), (looking.x - 90.f)), looking.y);
		}break;
		case 2:
		{
			lFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(-0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			rFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			lHIP = lFOOT + Vector3(0, 0.75f, 0);
			rHIP = rFOOT + Vector3(0, 0.75f, 0);
			HIP = baseBone + Vector3(0, 0.75f, 0);
			lSHOULDER = rotatey(lHIP, rotatex(lHIP, lHIP + Vector3(-0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			rSHOULDER = rotatey(rHIP, rotatex(rHIP, rHIP + Vector3(0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			NECK = rotatey(HIP, rotatex(HIP, HIP + Vector3(0, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			HEAD = rotatey(NECK, rotatex(NECK, NECK + Vector3(0, 0.4f, 0), (looking.x - 90.f) * 1.0f), looking.y);
			if (Camera.translation.distance(HEAD) < 25.f)
			{
				if (Camera.translation.distance(HEAD) > 5.f)
					LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, Camera.translation.distance(HEAD) - 5.f), (looking.x - 90.f)), looking.y);
				else
					LINE = HEAD;
			}
			else
				LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, 20.f), (looking.x - 90.f)), looking.y);
		}break;
		case 3:
		{
			lFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(-0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			rFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			lHIP = lFOOT + Vector3(0, 0.75f, 0);
			rHIP = rFOOT + Vector3(0, 0.75f, 0);
			HIP = baseBone + Vector3(0, 0.75f, 0);
			lSHOULDER = rotatey(lHIP, rotatex(lHIP, lHIP + Vector3(-0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			rSHOULDER = rotatey(rHIP, rotatex(rHIP, rHIP + Vector3(0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			NECK = rotatey(HIP, rotatex(HIP, HIP + Vector3(0, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			HEAD = rotatey(NECK, rotatex(NECK, NECK + Vector3(0, 0.4f, 0), (looking.x - 90.f) * 1.0f), looking.y);
			if (Camera.translation.distance(HEAD) < 25.f)
			{
				if (Camera.translation.distance(HEAD) > 5.f)
					LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, Camera.translation.distance(HEAD) - 5.f), (looking.x - 90.f)), looking.y);
				else
					LINE = HEAD;
			}
			else
				LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, 20.f), (looking.x - 90.f)), looking.y);
		}break;
		case 4:
		{
			lFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(-0.2f, 0.15f, -0.65f), (looking.x - 90.f) * 0.f), looking.y);
			rFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0.2f, 0, 0.1f), (looking.x - 90.f) * 0.f), looking.y);
			lHIP = rotatey(lFOOT, rotatex(lFOOT, lFOOT + Vector3(0, 0.35f, 0.5f), (looking.x - 90.f) * 0.f), looking.y);
			rHIP = rotatey(rFOOT, rotatex(rFOOT, rFOOT + Vector3(0, 0.5f, -0.25f), (looking.x - 90.f) * 0.f), looking.y);
			HIP = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0, 0.5f, -0.15f), (looking.x - 90.f) * 0.f), looking.y);
			lSHOULDER = rotatey(lHIP, rotatex(lHIP, lHIP + Vector3(-0.1f, 0.5f, 0.45f), (looking.x - 90.f) * 0.65f), looking.y);
			rSHOULDER = rotatey(rHIP, rotatex(rHIP, rHIP + Vector3(0.1f, 0.5f, 0.45f), (looking.x - 90.f) * 0.65f), looking.y);
			NECK = rotatey(HIP, rotatex(HIP, HIP + Vector3(0, 0.5f, 0.45f), (looking.x - 90.f) * 0.65f), looking.y);
			HEAD = rotatey(NECK, rotatex(NECK, NECK + Vector3(0, 0.3f, 0.1f), (looking.x - 90.f) * 0.9f), looking.y);
			if (Camera.translation.distance(HEAD) < 25.f)
			{
				if (Camera.translation.distance(HEAD) > 5.f)
					LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, Camera.translation.distance(HEAD) - 5.f), (looking.x - 90.f)), looking.y);
				else
					LINE = HEAD;
			}
			else
				LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, 20.f), (looking.x - 90.f)), looking.y);
		}break;
		case 5:
		{
			lFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(-0.2f, 0.12f, -1.0f), (looking.x - 90.f) * 0.f), looking.y);
			rFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0.2f, 0.12f, -1.0f), (looking.x - 90.f) * 0.f), looking.y);
			lHIP = rotatey(lFOOT, rotatex(lFOOT, lFOOT + Vector3(0, 0.f, 0.7f), (looking.x - 90.f) * 0.f), looking.y);
			rHIP = rotatey(rFOOT, rotatex(rFOOT, rFOOT + Vector3(0, 0.f, 0.7f), (looking.x - 90.f) * 0.f), looking.y);
			HIP = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0, 0.12f, -0.3f), (looking.x - 90.f) * 0.f), looking.y);
			lSHOULDER = rotatey(lHIP, rotatex(lHIP, lHIP + Vector3(0, 0.125f, 0.7f), (looking.x - 90.f) * 0.5f), looking.y);
			rSHOULDER = rotatey(rHIP, rotatex(rHIP, rHIP + Vector3(0, 0.125f, 0.7f), (looking.x - 90.f) * 0.5f), looking.y);
			NECK = rotatey(HIP, rotatex(HIP, HIP + Vector3(0, 0.125f, 0.7f), (looking.x - 90.f) * 0.5f), looking.y);
			HEAD = rotatey(NECK, rotatex(NECK, NECK + Vector3(0, 0.3f, 0.2f), (looking.x - 90.f) * 1.0f), looking.y);
			if (Camera.translation.distance(HEAD) < 25.f)
			{
				if (Camera.translation.distance(HEAD) > 5.f)
					LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, Camera.translation.distance(HEAD) - 5.f), (looking.x - 90.f)), looking.y);
				else
					LINE = HEAD;
			}
			else
				LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, 20.f), (looking.x - 90.f)), looking.y);
		}break;
		case 6:
		{
			lFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(-0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			rFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			lHIP = lFOOT + Vector3(0, 0.75f, 0);
			rHIP = rFOOT + Vector3(0, 0.75f, 0);
			HIP = baseBone + Vector3(0, 0.75f, 0);
			lSHOULDER = rotatey(lHIP, rotatex(lHIP, lHIP + Vector3(-0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			rSHOULDER = rotatey(rHIP, rotatex(rHIP, rHIP + Vector3(0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			NECK = rotatey(HIP, rotatex(HIP, HIP + Vector3(0, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			HEAD = rotatey(NECK, rotatex(NECK, NECK + Vector3(0, 0.4f, 0), (looking.x - 90.f) * 1.0f), looking.y);
			if (Camera.translation.distance(HEAD) < 25.f)
			{
				if (Camera.translation.distance(HEAD) > 5.f)
					LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, Camera.translation.distance(HEAD) - 5.f), (looking.x - 90.f)), looking.y);
				else
					LINE = HEAD;
			}
			else
				LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, 20.f), (looking.x - 90.f)), looking.y);
		}break;
		case 7:
		{
			lFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(-0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			rFOOT = rotatey(baseBone, rotatex(baseBone, baseBone + Vector3(0.2f, 0, 0), (looking.x - 90.f) * 0.f), looking.y);
			lHIP = lFOOT + Vector3(0, 0.75f, 0);
			rHIP = rFOOT + Vector3(0, 0.75f, 0);
			HIP = baseBone + Vector3(0, 0.75f, 0);
			lSHOULDER = rotatey(lHIP, rotatex(lHIP, lHIP + Vector3(-0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			rSHOULDER = rotatey(rHIP, rotatex(rHIP, rHIP + Vector3(0.1f, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			NECK = rotatey(HIP, rotatex(HIP, HIP + Vector3(0, 0.6f, 0), (looking.x - 90.f) * 0.5f), looking.y);
			HEAD = rotatey(NECK, rotatex(NECK, NECK + Vector3(0, 0.4f, 0), (looking.x - 90.f) * 1.0f), looking.y);
			if (Camera.translation.distance(HEAD) < 25.f)
			{
				if (Camera.translation.distance(HEAD) > 5.f)
					LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, Camera.translation.distance(HEAD) - 5.f), (looking.x - 90.f)), looking.y);
				else
					LINE = HEAD;
			}
			else
				LINE = rotatey(HEAD, rotatex(HEAD, HEAD + Vector3(0, 0, 20.f), (looking.x - 90.f)), looking.y);
		}break;
		default:
			break;
		}
	}
	Vector3 head()
	{
		return HEAD;
	}
	Vector3 neck()
	{
		return NECK;
	}
	Vector3 lshoulder()
	{
		return lSHOULDER;
	}
	Vector3 rshoulder()
	{
		return rSHOULDER;
	}
	Vector3 hip()
	{
		return HIP;
	}
	Vector3 lhip()
	{
		return lHIP;
	}
	Vector3 rhip()
	{
		return rHIP;
	}
	Vector3 lfoot()
	{
		return lFOOT;
	}
	Vector3 rfoot()
	{
		return rFOOT;
	}
	Vector3 aimLine()
	{
		return LINE;
	}
};