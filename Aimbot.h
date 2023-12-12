#pragma once

class Aimbot
{
private:
	clock_t time, oldtime = 0;
	Vector3 random;
public:
	Aimbot()
	{
		
	}
	Vector2 CalcuateAngles(Vector3 src, Vector3 dest, Vector2 startAngle, Vector2 ScopeSway, float missChance, float smoothing)
	{
		
		time = clock();
		if (time - oldtime > 100 && GetAsyncKeyState(VK_LBUTTON))
		{
			oldtime = time;
			float factor = 2.0f;
			random = ((Vector3((float)(rand() % (int)(1000 * factor) - rand() % (int)(1000 * factor)) / 1000.f, (float)(rand() % (int)(1000 * factor) - rand() % (int)(1000 * factor)) / 1000.f, (float)(rand() % (int)(1000 * factor) - rand() % (int)(1000 * factor)) / 1000.f)) * sqrt(missChance));
		}
			
		dest += random;

		bool isRight = WorldToScreen(dest).x > Camera.screen.x / 2;

		Vector3 delta = dest - src;
		Vector2 angle = Vector2(
			1.5708f - atan2(delta.y, sqrt(delta.z * delta.z + delta.x * delta.x)),
			atan2(dest.x - src.x, dest.z - src.z)
		) * 57.29577f - ScopeSway;
		if (!smoothing)
			return angle;

		Vector2 diff = Vector2(angle.x - startAngle.x, fabs(angle.y - startAngle.y));
		diff = Vector2(diff.x, fmod(diff.y, 360.0));
		Vector2 minAngle = Vector2(diff.x, min(diff.y, 360.f - diff.y));
		Vector2 ret = Vector2(
			startAngle.x + minAngle.x * (1.f - smoothing),
			isRight ? startAngle.y + minAngle.y * (1.f - smoothing) : startAngle.y - minAngle.y * (1.f - smoothing)
		);
		
		return ret;
	}
};