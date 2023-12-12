#pragma once

class Vehicle
{
private:
	struct address
	{
		uint64_t InteractableVehicle;
		uint64_t _asset;
		uint64_t lastUpdatePos;

	};
	struct value
	{
		std::wstring _vehicleName;
		Vector3 lastUpdatePos;

		bool skip = false;
		uint32_t skipCount = 0;
	};
	std::vector<address> a;
	std::vector<value> v;
	std::vector<uint64_t> IV;
	uint8_t page0[0x1000] = { 0 };
	uint8_t page1[0x1000] = { 0 };
	uint8_t var0[0x100] = { 0 };
	uint8_t var1[0x100] = { 0 };
	uint64_t pInteractableVehicle;
	float maxRender = 1000.f;

	void UpdateVehicle(uint64_t index)
	{

		if (a.size() != IV.size())// If change is detected
		{
			std::vector<address> A;
			std::vector<value> V;
			A.resize(IV.size());
			V.resize(IV.size());

			bool isValid = false;

			if (a.size() > IV.size())// Vehicle Left
			{
				for (int i = 0; i < a.size(); i++)
					for (int _i = 0; _i < IV.size(); _i++)
						if (a.at(i).InteractableVehicle == IV.at(_i))
						{
							A.at(_i) = a.at(i);
							V.at(_i) = v.at(i);
							break;
						}
				//remove invalid from list
				a = A;
				v = V;
				A.empty();
				V.empty();
			}
			else// Vehicle Added
			{
				for (int _i = 0; _i < IV.size(); _i++)
				{
					isValid = false;
					for (int i = 0; i < a.size(); i++)
					{
						if (a.at(i).InteractableVehicle == IV.at(_i))
						{
							isValid = true;
							A.at(_i) = a.at(i);
							V.at(_i) = v.at(i);
							break;
						}
					}
					if (!isValid) // Found missing Vehicle adding to list
					{
						address _A;
						value _V;
						// =========== INIT NEW Vehicle ============

						// InteractableVehicle
						_A.InteractableVehicle = IV.at(_i);
						read(_A.InteractableVehicle, (uint64_t)page1, 0x1000);
						_A._asset = *(uint64_t*)&page1[InteractableVehicle._asset.self()];
						_V.lastUpdatePos = *(Vector3*)&page1[InteractableVehicle.lastUpdatePos];
						_V._vehicleName = read<std::wstring>({ _A._asset + InteractableVehicle._asset._vehicleName.self(), InteractableVehicle._asset._vehicleName.Value});
						
						// =========== END INIT NEW Vehicle ============
						A.at(_i) = _A;
						V.at(_i) = _V;
					}
				}
				//added to list and update
				a = A;
				v = V;
				A.empty();
				V.empty();
			}
		}
		else// no change detected update normal
		{
			address A = a.at(index);
			value V = v.at(index);
			V.skip = true;
			read(A.InteractableVehicle, (uint64_t)page1, 0x1000);
			if (!V.skipCount)
			{
				V.lastUpdatePos = *(Vector3*)&page1[InteractableVehicle.lastUpdatePos];
				float distance = Camera.translation.distance(V.lastUpdatePos);
				if (distance > maxRender)
				{
					V.skipCount = 100 + rand() % 100;
					V._vehicleName.resize(0);
				}
				else
				{
					V.skipCount = 0;
					auto base = WorldToScreen(V.lastUpdatePos);
					if (base.z < 0.f)
						V.skip = false;
				}
			}
			else
				V.skipCount--;
			if (!V.skip)// if vehicle is on screen and within max render update rest of vehicle
			{
				A._asset = *(uint64_t*)&page1[InteractableVehicle._asset.self()];
				V._vehicleName = read<std::wstring>({ A._asset + InteractableVehicle._asset._vehicleName.self(), InteractableVehicle._asset._vehicleName.Value });

				auto base = WorldToScreen(V.lastUpdatePos);
				if (base.z < 0)
				{
					if(V._vehicleName.size() == 0)
						V._vehicleName = read<std::wstring>({ A._asset + InteractableVehicle._asset._vehicleName.self(), InteractableVehicle._asset._vehicleName.Value });
					float dist = Camera.translation.distance(V.lastUpdatePos);
					float scale = 3000 / dist;
					if (scale >= 12.5f)
						scale = 12.5f;
					else if (scale <= 7.f)
						scale = 7.f;
					std::wstring txt = V._vehicleName + L" " + std::to_wstring((int)dist) + L"m";
					draw->TextW(Vector2(base.x - txt.size() * (2.95f * (scale / 12.5f)), base.y), txt.c_str(), scale, WHITE);
				}
			}
			a.at(index) = A;
			v.at(index) = V;
		}
	}

	uint32_t VehicleSize()
	{
		uint32_t ret = 0;
		read(pInteractableVehicle, (uint64_t)var0, 0x20);
		ret = *(uint32_t*)(var0 + 0x18);
		IV.resize(ret);
		read(*(uint64_t*)(var0 + 0x10), (uint64_t)page0, 0x1000);
		for (int i = 0; i < ret; i++)
		{
			IV.at(i) = *(uint64_t*)&page0[0x20 + 8 * i];
		}
		return ret;
	}
public:
	Vehicle(uint64_t pInteractableVehicle) : pInteractableVehicle(pInteractableVehicle)
	{
		a.resize(0);
		v.resize(0);

	}

	void SetRenderDistance(float dist)
	{
		maxRender = dist;
	}

	void Update()
	{
		uint32_t size = VehicleSize();
		for (int i = 0; i < size; i++)
		{
			UpdateVehicle(i);
		}
	}
	void Draw()
	{

	}
};