#pragma once
class ModeConfig
{
private:
	uint64_t _Gameplay;
	uint8_t page[0x100];
public:
	ModeConfig(uint64_t _Gameplay) : _Gameplay(_Gameplay)
	{
		read(_Gameplay, (uint64_t)page, 0x100);
	}

	void update()
	{
		read(_Gameplay, (uint64_t)page, 0x100);
	}

	bool GetCrosshair()
	{
		return *(bool*)(&page[ModeConfigData._Gameplay.Crosshair]);
	}

	bool GetChart()
	{
		return *(bool*)(&page[ModeConfigData._Gameplay.Chart]);
	}

	bool GetSatellite()
	{
		return *(bool*)(&page[ModeConfigData._Gameplay.Satellite]);
	}

	bool GetCompass()
	{
		return *(bool*)(&page[ModeConfigData._Gameplay.Compass]);
	}

	bool GetAllow_Shoulder_Camera()
	{
		return *(bool*)(&page[ModeConfigData._Gameplay.Allow_Shoulder_Camera]);
	}

	bool GetCan_Suicide()
	{
		return *(bool*)(&page[ModeConfigData._Gameplay.Can_Suicide]);
	}

	uint32_t GetTimer_Exit()
	{
		return *(uint32_t*)(&page[ModeConfigData._Gameplay.Timer_Exit]);
	}

	uint32_t GetTimer_Respawn()
	{
		return *(uint32_t*)(&page[ModeConfigData._Gameplay.Timer_Respawn]);
	}

	uint32_t GetTimer_Home()
	{
		return *(uint32_t*)(&page[ModeConfigData._Gameplay.Timer_Home]);
	}

	uint32_t GetTimer_Exit_Group()
	{
		return *(uint32_t*)(&page[ModeConfigData._Gameplay.Timer_Exit_Group]);
	}

	void SetCrosshair(bool data)
	{
		write<bool>(_Gameplay + ModeConfigData._Gameplay.Crosshair, data); update();
	}

	void SetChart(bool data)
	{
		write<bool>(_Gameplay + ModeConfigData._Gameplay.Chart, data); update();
	}

	void SetSatellite(bool data)
	{
		write<bool>(_Gameplay + ModeConfigData._Gameplay.Satellite, data); update();
	}

	void SetCompass(bool data)
	{
		write<bool>(_Gameplay + ModeConfigData._Gameplay.Compass, data); update();
	}

	void SetAllow_Shoulder_Camera(bool data)
	{
		write<bool>(_Gameplay + ModeConfigData._Gameplay.Allow_Shoulder_Camera, data); update();
	}

	void SetCan_Suicide(bool data)
	{
		write<bool>(_Gameplay + ModeConfigData._Gameplay.Can_Suicide, data); update();
	}

	void SetTimer_Exit(uint32_t data)
	{
		write<uint32_t>(_Gameplay + ModeConfigData._Gameplay.Timer_Exit, data); update();
	}

	void SetTimer_Respawn(uint32_t data)
	{
		write<uint32_t>(_Gameplay + ModeConfigData._Gameplay.Timer_Respawn, data); update();
	}

	void SetTimer_Home(uint32_t data)
	{
		write<uint32_t>(_Gameplay + ModeConfigData._Gameplay.Timer_Home, data); update();
	}

	void SetTimer_Exit_Group(uint32_t data)
	{
		write<uint32_t>(_Gameplay + ModeConfigData._Gameplay.Timer_Exit_Group, data); update();
	}
};
