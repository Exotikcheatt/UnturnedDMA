#pragma once

class UserInterface
{
private:

	HWND last_window;
	class ToggleKey
	{
	private:
		bool keyState = false;
		bool retState = false;
		int key = 0;
	public:
		ToggleKey(int key) : key(key) {}
		int getKey() { return key; }
		void setKey(int key) { this->key = key; }
		bool state()
		{
			if (GetAsyncKeyState(key))
			{
				if (keyState == false)
				{
					retState = !retState;
					keyState = true;
				}
			}
			else
				keyState = false;
			return retState;
		}
	}*tk_Menu;

	bool bSetFocus = false;
	void SetFocus()
	{
		if (GetAsyncKeyState(tk_Menu->getKey()) && !bSetFocus)
		{
			if (tk_Menu->state())
			{
				last_window = GetForegroundWindow();
				SetWindowLong(__GRAPHICS::hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_PALETTEWINDOW);
				DWORD lockTimeOut = 0;
				HWND  hCurrWnd = GetForegroundWindow();
				DWORD dwThisTID = GetCurrentThreadId(),
					dwCurrTID = GetWindowThreadProcessId(hCurrWnd, 0);
				if (dwThisTID != dwCurrTID)
				{
					AttachThreadInput(dwThisTID, dwCurrTID, TRUE);
					SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &lockTimeOut, 0);
					SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
					AllowSetForegroundWindow(ASFW_ANY);
				}
				SetForegroundWindow(__GRAPHICS::hwnd);
				if (dwThisTID != dwCurrTID)
				{
					SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0,
						(PVOID)lockTimeOut, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
					AttachThreadInput(dwThisTID, dwCurrTID, FALSE);
				}
			}
			else
			{
				SetWindowLong(__GRAPHICS::hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_PALETTEWINDOW);
				if (last_window)
					SetForegroundWindow(last_window);
			}
			bSetFocus = true;
		}
		else
			bSetFocus = false;
	}
	class Switch
	{
	private:
		D2D1_COLOR_F onColor, offColor;
		D2D1_COLOR_F onColorHighlight = color(50, 120, 240), offColorHighlight = color(185, 185, 185);
		D2D1_COLOR_F onColorNormal = color(20, 100, 240), offColorNormal = color(204, 204, 204);
		Vector2 pos = 0;
		clock_t time = 0, oldTime = 0;
		int speed = 120;
		float animateProgress = 0.f;
		bool isCursorInRegon(Vector2 pos, Vector2 size)
		{
			POINT p;
			GetCursorPos(&p);
			Vector2 cursorPos = Vector2(p.x, p.y);
			return cursorPos.x > pos.x && cursorPos.y > pos.y && cursorPos.x <= pos.x + size.x && cursorPos.y <= pos.y + size.y;
		}
		bool isDown = false;
	public:
		bool state = false;
		Switch()
		{

		}
		void animate(Vector2 pos)
		{
			this->pos = pos;
			time = clock();
			if (oldTime == 0)
				oldTime = time - speed;
			float size = 15.f;
			if (GetAsyncKeyState(VK_LBUTTON) && !isDown && isCursorInRegon(pos, Vector2(size * 1.66f, size)))
			{
				oldTime = time;
				animateProgress = state ? 1.f : 0.f;
				state = !state;
				isDown = true;
			}
			else if(!GetAsyncKeyState(VK_LBUTTON))
				isDown = false;
			if (isCursorInRegon(pos, Vector2(size * 1.66f, size)))
			{
				onColor = onColorHighlight;
				offColor = offColorHighlight;
			}
			else
			{
				onColor = onColorNormal;
				offColor = offColorNormal;
			}

			if (time - oldTime < speed)
			{
				if (state)
					animateProgress = (float)(time - oldTime) / (float)speed;
				else
					animateProgress = 1.f - (float)(time - oldTime) / (float)speed;
			}
			else
			{
				if (state)
					animateProgress = 1.f;
				else
					animateProgress = 0.f;
			}

			
			
			D2D1_COLOR_F c = color(
				(onColor.r * (animateProgress) + offColor.r * (1.0f - animateProgress)) * 255.f,
				(onColor.g * (animateProgress) + offColor.g * (1.0f - animateProgress)) * 255.f,
				(onColor.b * (animateProgress) + offColor.b * (1.0f - animateProgress)) * 255.f
			);

			draw->RoundRect(pos, Vector2(size), size, c);
			draw->RoundRect(pos + Vector2(size * 0.66f, 0), Vector2(size), size, c);
			draw->RoundRect(pos + Vector2(size * 0.33f + 1, 0), Vector2(size - 2, size), 0, c);
			draw->RoundRect(pos + 2 + Vector2((size * 0.66f) * animateProgress, 0), size - 4, size - 4, WHITE);
		}

	};

	class SliderBar
	{
	private:
		D2D1_COLOR_F onColor, offColor;
		D2D1_COLOR_F onColorHighlight = color(50, 120, 240), offColorHighlight = color(175, 175, 175);
		D2D1_COLOR_F onColorNormal = color(20, 100, 240), offColorNormal = color(150, 150, 150);
		Vector2 pos = 0;
		float width = 0.f, offset = 0.f;

		bool isCursorInRegon(Vector2 pos, Vector2 size)
		{
			POINT p;
			GetCursorPos(&p);
			Vector2 cursorPos = Vector2(p.x, p.y);
			return cursorPos.x > pos.x && cursorPos.y > pos.y && cursorPos.x <= pos.x + size.x && cursorPos.y <= pos.y + size.y;
		}
		bool isDown = false;
	public:
		float progress = 0.f;
		SliderBar(float width)
		{
			this->width = width;
		}
		void animate(Vector2 pos)
		{
			this->pos = pos;
			float size = 18.f;
			if (GetAsyncKeyState(VK_LBUTTON) && isDown)
			{
				POINT p;
				GetCursorPos(&p);
				Vector2 cursorPos = Vector2(p.x, p.y);
				progress = ((cursorPos.x - offset) - pos.x) / (width - size);
				if (progress > 1.f)
					progress = 1.f;
				else if (progress < 0.f)
					progress = 0.f;
			}
			else if (GetAsyncKeyState(VK_LBUTTON) && isCursorInRegon(pos + Vector2(((width - size) * progress), 0), size))
			{
				isDown = true;
				POINT p;
				GetCursorPos(&p);
				Vector2 cursorPos = Vector2(p.x, p.y);
				offset = cursorPos.x - (pos.x + ((width - size) * progress));
			}
			else
				isDown = false;

			if (isCursorInRegon(pos + Vector2(((width - size) * progress), 0), size))
			{
				onColor = onColorHighlight;
				offColor = offColorHighlight;
			}
			else
			{
				onColor = onColorNormal;
				offColor = offColorNormal;
			}

			draw->RoundRect((pos + Vector2(0, size / 2 - 1)), Vector2(((width - size) * progress), 2), 0, onColorNormal);
			draw->RoundRect((pos + Vector2(((width - size) * progress), size / 2 - 1)), Vector2(width - ((width - size) * progress), 2), 0, offColorNormal);
			draw->RoundRect((pos + Vector2(((width - size) * progress), 0)), Vector2(size, size), size, offColor);
			draw->RoundRect((pos + Vector2(((width - size) * progress), 0)) + 3, size - 6, size - 6, WHITE);
		}

	};
	
	class SelectionBox
	{
	private:
		D2D1_COLOR_F color0, color1;
		D2D1_COLOR_F colorNormal = color(48, 52, 67), colorHighlight = color(64, 68, 82);
		Vector2 pos = 0;
		std::vector<std::string> list;
		float width = 0.f;

		bool isCursorInRegon(Vector2 pos, Vector2 size)
		{
			POINT p;
			GetCursorPos(&p);
			Vector2 cursorPos = Vector2(p.x, p.y);
			return cursorPos.x > pos.x && cursorPos.y > pos.y && cursorPos.x <= pos.x + size.x && cursorPos.y <= pos.y + size.y;
		}
		
		bool isDown0 = false, isDown1 = false;
	public:
		int index = 0;
		SelectionBox(float width, std::vector<std::string> list)
		{
			this->width = width;
			this->list = list;
		}
		void animate(Vector2 pos)
		{
			this->pos = pos;
			float size = 15.f;
			if (GetAsyncKeyState(VK_LBUTTON) && !isDown0 && isCursorInRegon(pos, size))
			{
				index--;
				if (index < 0)
					index = list.size() - 1;
				isDown0 = true;
			}
			else if (!GetAsyncKeyState(VK_LBUTTON))
				isDown0 = false;

			if (GetAsyncKeyState(VK_LBUTTON) && !isDown1 && isCursorInRegon(pos + Vector2(width - size, 0), size))
			{
				index++;
				if (index >= list.size())
					index = 0;
				isDown1 = true;
			}
			else if (!GetAsyncKeyState(VK_LBUTTON))
				isDown1 = false;

			if (!isCursorInRegon(pos, size))
				color0 = colorNormal;
			else
				color0 = colorHighlight;

			if (!isCursorInRegon(pos + Vector2(width - size, 0), size))
				color1 = colorNormal;
			else
				color1 = colorHighlight;

			draw->RoundRect(pos, Vector2(width, size), 4, colorNormal);
			draw->RoundRect(pos, size, 4, color0);
			draw->TextA(pos + Vector2(3, - 1.5), "<", size, color(200, 200, 200));
			draw->RoundRect(pos + Vector2(width - size, 0), size, 4, color1);
			draw->TextA(pos + Vector2(width - size, 0) + Vector2(3, -1.5), ">", size, color(200, 200, 200));
			draw->TextA(pos + Vector2(width / 2.f - ((float)list.at(index).size() * (size / 1.8275f) / 2), -1.f), list.at(index), size, color(200, 200, 200));
		}
	};

	class Text
	{
	private:
		Vector2 pos;
		float size = 0;
	public:
		Text(float size)
		{
			this->size = size;
		}
		void animate(Vector2 pos, std::string str)
		{
			this->pos = pos;
			draw->TextA(pos, str, size, color(200, 200, 200));
		}
	};

	Switch *Weapon, *Visual, *Aimbot, *AimDot, *Gps, *Compass, *Buildable, *Misc, *Salvage;
	Text *WeaponTxt, *VisualTxt, *AimbotTxt, *AimDotTxt, *GpsTxt, *CompassTxt, * BuildableTxt, *MiscTxt, *SalvageTxt;
	SliderBar *Spread, *Recoil, *Smoothing, *AimbotFov, *RenderDistance, *Pitch, * Roll, * Yaw, * Offset, *Range, *SalvageTime, *ExitTimer, *AimMissChance;
	Text *SpreadTxt, *RecoilTxt, *SmoothingTxt, *AimbotFovTxt, * RenderDistanceTxt, *PitchTxt, *RollTxt, *YawTxt, *RangeTxt, *OffsetTxt, *SalvageTimeTxt, *ExitTimerTxt, *AimMissChanceTxt;
	SelectionBox *AimTarget;
	Text *AimTargetTxt;
	float ySize = 0.f;
public:
	UserInterface()
	{
		tk_Menu = new ToggleKey(VK_DELETE);

		Weapon = new Switch();
		WeaponTxt = new Text(15);
		Visual = new Switch();
		VisualTxt = new Text(15);
		Visual = new Switch();
		VisualTxt = new Text(15);
		Buildable = new Switch();
		BuildableTxt = new Text(15);
		Misc = new Switch();
		MiscTxt = new Text(15);

		Aimbot = new Switch();
		AimbotTxt = new Text(15);
		AimTarget = new SelectionBox(90, { XorString("Head"), XorString("Neck"), XorString("Hip") });
		AimTargetTxt = new Text(15);
		AimMissChance = new SliderBar(100);
		AimMissChanceTxt = new Text(15);
		Spread = new SliderBar(100);
		SpreadTxt = new Text(15);
		Recoil = new SliderBar(100);
		RecoilTxt = new Text(15);
		Smoothing = new SliderBar(100);
		SmoothingTxt = new Text(15);
		AimbotFov = new SliderBar(100);
		AimbotFovTxt = new Text(15);

		RenderDistance = new SliderBar(120);
		RenderDistanceTxt = new Text(15);
		AimDot = new Switch();
		AimDotTxt = new Text(15);
		Gps = new Switch();
		GpsTxt = new Text(15);
		Compass = new Switch();
		CompassTxt = new Text(15);
		
		Pitch = new SliderBar(90);
		PitchTxt = new Text(15);
		Yaw = new SliderBar(90);
		YawTxt = new Text(15);
		Roll = new SliderBar(90);
		RollTxt = new Text(15);
		Offset = new SliderBar(100);
		OffsetTxt = new Text(15);
		Range = new SliderBar(100);
		RangeTxt = new Text(15);

		Salvage = new Switch();
		SalvageTxt = new Text(15);
		SalvageTime = new SliderBar(100);
		SalvageTimeTxt = new Text(15);
		ExitTimer = new SliderBar(100);
		ExitTimerTxt = new Text(15);
	}

	bool isAimbotEnabled()
	{
		return Aimbot->state;
	}

	void isAimbotEnabled(bool state)
	{
		Aimbot->state = state;
	}

	bool isAimDotEnabled()
	{
		return AimDot->state;
	}

	void isAimDotEnabled(bool state)
	{
		AimDot->state = state;
	}

	bool CompassEnabled()
	{
		return Compass->state;
	}

	void CompassEnabled(bool state)
	{
		Compass->state = state;
	}

	bool GpsEnabled()
	{
		return Gps->state;
	}

	void GpsEnabled(bool state)
	{
		Gps->state = state;
	}

	void aimbotFov(float value)
	{
		AimbotFov->progress = value;
	}

	float aimbotFov()
	{
		return AimbotFov->progress;
	}

	float aimbotSmoothing()
	{
		return Smoothing->progress;
	}

	void aimbotSmoothing(float value)
	{
		Smoothing->progress = value;
	}

	float weaponSpread()
	{
		return Spread->progress;
	}

	void weaponSpread(float value)
	{
		Spread->progress = value;
	}

	float weaponRecoil()
	{
		return Recoil->progress;
	}

	void weaponRecoil(float value)
	{
		Recoil->progress = value;
	}

	void renderDistance(float value)
	{
		RenderDistance->progress = value;
	}

	float renderDistance()
	{
		return RenderDistance->progress;
	}

	void enableSalvage(bool value)
	{
		Salvage->state = value;
	}

	bool enableSalvage()
	{
		return Salvage->state;
	}

	void salvageTime(float value)
	{
		SalvageTime->progress = value;
	}

	float salvageTime()
	{
		return SalvageTime->progress;
	}

	void exitTimer(float value)
	{
		ExitTimer->progress = value;
	}

	float exitTimer()
	{
		return ExitTimer->progress;
	}

	float aimTarget()
	{
		return AimTarget->index;
	}

	void aimTarget(float value)
	{
		AimTarget->index = value;
	}

	void range(float value)
	{
		Range->progress = value;
	}

	float range()
	{
		return Range->progress;
	}

	void offset(float value)
	{
		Offset->progress = value;
	}

	float offset()
	{
		return Offset->progress;
	}

	void pitch(float value)
	{
		Pitch->progress = value;
	}

	float pitch()
	{
		return Pitch->progress;
	}

	void yaw(float value)
	{
		Yaw->progress = value;
	}

	float yaw()
	{
		return Yaw->progress;
	}

	void roll(float value)
	{
		Roll->progress = value;
	}

	float roll()
	{
		return Roll->progress;
	}

	void missChance(float value)
	{
		AimMissChance->progress = value;
	}

	float missChance()
	{
		return AimMissChance->progress;
	}

	void SetFreeBuild(Vector3 angles)
	{
		Pitch->progress = angles.x;
		Yaw->progress = angles.y;
		Roll->progress = angles.z;
	}

	void Draw()
	{
		SetFocus();
		if (tk_Menu->state())
		{
			Vector2 pos = Vector2(20, 20);
			draw->RoundRect(pos, Vector2(300, ySize), 10, color(20, 20, 20));
			draw->RoundRect(pos + 3, Vector2(300, ySize) - 6, 6, color(85, 85, 85));
			Weapon->animate(pos += Vector2(20, 20));
			WeaponTxt->animate(pos += Vector2(35, 0), XorString("Weapon"));
			pos += Vector2(-35, 20);
			if (Weapon->state)
			{
				pos += Vector2(20, 0);
				Aimbot->animate(pos);
				AimbotTxt->animate(pos += Vector2(35, 0), XorString("Toggle Aimbot"));
				pos += Vector2(-35, 20);
				if (Aimbot->state)
				{
					pos += Vector2(20, 0);
					AimTarget->animate(pos);
					AimTargetTxt->animate(pos += Vector2(110, 0), XorString("Aimbot Target"));
					AimMissChance->animate(pos += Vector2(-110, 20));
					AimMissChanceTxt->animate(pos += Vector2(110, 0), XorString("MissChance ") + std::to_string((int)(AimMissChance->progress * 100.f)) + XorString("%"));
					Smoothing->animate(pos += Vector2(-110, 20));
					SmoothingTxt->animate(pos += Vector2(110, 0), XorString("Smoothing ") + std::to_string((int)(Smoothing->progress * 100.f)) + XorString("%"));
					AimbotFov->animate(pos += Vector2(-110, 20));
					AimbotFovTxt->animate(pos += Vector2(110, 0), XorString("FOV ") + std::to_string((int)(AimbotFov->progress * 500.f)) + XorString("px"));
					pos += Vector2(-130, 20);
				}
				Spread->animate(pos);
				SpreadTxt->animate(pos += Vector2(110, 0), XorString("Spread ") + std::to_string((int)(Spread->progress * 100.f)) + XorString("%"));
				Recoil->animate(pos += Vector2(-110, 20));
				RecoilTxt->animate(pos += Vector2(110, 0), XorString("Recoil ") + std::to_string((int)(Recoil->progress * 100.f)) + XorString("%"));
				pos += Vector2(-130, 20);
			}
			Visual->animate(pos);
			VisualTxt->animate(pos += Vector2(35, 0), XorString("Visuals"));
			pos += Vector2(-35, 20);
			if (Visual->state)
			{
				pos += Vector2(20, 0);
				RenderDistance->animate(pos);
				RenderDistanceTxt->animate(pos += Vector2(130, 0), XorString("Render ") + std::to_string((int)(RenderDistance->progress * 3000.f)) + XorString("m"));
				Compass->animate(pos += Vector2(-130, 20));
				CompassTxt->animate(pos += Vector2(35, 0), XorString("Compass"));
				Gps->animate(pos += Vector2(-35, 20));
				GpsTxt->animate(pos += Vector2(35, 0), XorString("Gps"));
				AimDot->animate(pos += Vector2(-35, 20));
				AimDotTxt->animate(pos += Vector2(35, 0), XorString("Aim Dot"));
				pos += Vector2(-55, 20);
			}
			Buildable->animate(pos);
			BuildableTxt->animate(pos += Vector2(35, 0), XorString("Buildable"));
			pos += Vector2(-35, 20);
			if (Buildable->state)
			{
				pos += Vector2(20, 0);
				Range->animate(pos);
				RangeTxt->animate(pos += Vector2(110, 0), XorString("Range ") + std::to_string((int)(Range->progress * 13.f)) + XorString("m"));
				Offset->animate(pos += Vector2(-110, 20));
				OffsetTxt->animate(pos += Vector2(110, 0), XorString("Offset ") + std::to_string((Offset->progress - 0.5f) * 3.f).substr(0, 5) + XorString("m"));
				Pitch->animate(pos += Vector2(-110, 20));
				PitchTxt->animate(pos += Vector2(100, 0), XorString("Pitch ") + std::to_string((int)(Pitch->progress * 360.f)));
				Yaw->animate(pos += Vector2(-100, 20));
				YawTxt->animate(pos += Vector2(100, 0), XorString("Yaw ") + std::to_string((int)(Yaw->progress * 360.f)));
				Roll->animate(pos += Vector2(-100, 20));
				RollTxt->animate(pos += Vector2(100, 0), XorString("Roll ") + std::to_string((int)(Roll->progress * 360.f)));
				pos += Vector2(-120, 20);
			}

			Misc->animate(pos);
			MiscTxt->animate(pos += Vector2(35, 0), XorString("Misc"));
			pos += Vector2(-35, 20);
			if (Misc->state)
			{
				pos += Vector2(20, 0);
				Salvage->animate(pos);
				SalvageTxt->animate(pos += Vector2(35, 0), XorString("Enable Salvage"));
				pos += Vector2(-35, 20);
				if (Salvage->state)
				{
					pos += Vector2(20, 0);
					SalvageTime->animate(pos);
					SalvageTimeTxt->animate(pos += Vector2(110, 0), XorString("Time ") + std::to_string((int)(SalvageTime->progress * 10000.f)) + XorString("ms"));
					pos += Vector2(-130, 20);
				}
				ExitTimer->animate(pos);
				ExitTimerTxt->animate(pos += Vector2(110, 0), XorString("Exit Timer ") + std::to_string((int)(ExitTimer->progress * 30.f)) + XorString("s"));
				pos += Vector2(-130, 20);
			}
			ySize = pos.y;
		}
	}
};