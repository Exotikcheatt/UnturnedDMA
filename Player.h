#pragma once

#include "PlayerBone.h"
class Player
{
private:
	struct address
	{
		uint64_t SteamPlayer;

		uint64_t _animator;
		uint64_t _equipment;
		uint64_t _life;
		uint64_t _movement;
		uint64_t _look;
		uint64_t _skills;
		uint64_t _stance;
		uint64_t _interact;

		uint64_t _characterName;
		uint64_t scopeSway;
		uint64_t equipmentName;
		uint64_t type;
		uint64_t range;
		uint64_t useRecoilAim;
		uint64_t baseSpreadAngleRadians;
		uint64_t ballisticSteps;
		uint64_t ballisticTravel;
		uint64_t ballisticDrop;
		uint64_t _health;
		uint64_t _range;
		uint64_t _radius;
		uint64_t _offset;
		uint64_t wasTriggerJustPulled;
		uint64_t isAiming;
		uint64_t ammo;
		uint64_t zoom;
		uint64_t shouldZoomUsingEyes;
		uint64_t point;
		uint64_t input;
		uint64_t rotate;
		uint64_t _isDead;
		uint64_t _reputation;
		uint64_t _vehicleName;
		uint64_t lastUpdatePos;
		uint64_t _isGrounded;
		uint64_t _isMoving;
		uint64_t snapshot;
		uint64_t seat;
		uint64_t _angles;
		uint64_t isIgnoringInput;
		uint64_t mainCameraZoomFactor;
		uint64_t scopeCameraZoomFactor;
		uint64_t _perspective;
		uint64_t isZoomed;
		uint64_t allowFreecamWithoutAdmin;
		uint64_t allowWorkzoneWithoutAdmin;
		uint64_t allowSpecStatsWithoutAdmin;
		uint64_t stance;
		uint64_t shouldOverrideSalvageTime;
		uint64_t overrideSalvageTimeValue;
		uint64_t _isAdmin;
	};
	struct value
	{
		std::wstring _characterName;
		bool isLocal;
		Vector2 scopeSway;
		std::wstring equipmentName;
		uint32_t type;
		float range;
		byte useRecoilAim;
		float baseSpreadAngleRadians;
		float aimingRecoilMultiplier;
		byte ballisticSteps;
		float ballisticTravel;
		float muzzleVelocity;
		float bulletGravityMultiplier;
		uint16_t _health;
		float _range;
		float _radius;
		float _offset;
		byte wasTriggerJustPulled;
		byte isAiming;
		byte ammo;
		float zoom;
		byte shouldZoomUsingEyes;
		Vector3 point;
		Vector3 input;
		Vector3 rotate;
		byte _isDead;
		int _reputation;
		std::wstring _vehicleName;
		Vector3 lastUpdatePos;
		byte _isGrounded;
		byte _isMoving;
		Vector3 snapshot;
		byte seat;
		Vector2 _angles;
		byte isIgnoringInput;
		float mainCameraZoomFactor;
		float scopeCameraZoomFactor;
		uint32_t _perspective;
		byte isZoomed;
		byte allowFreecamWithoutAdmin;
		byte allowWorkzoneWithoutAdmin;
		byte allowSpecStatsWithoutAdmin;
		uint32_t stance;
		float overrideSalvageTimeValue = -100.f;
		bool shouldOverrideSalvageTime;
		byte _isAdmin;

		float FoundationPositionOffset;
		float customRotationOffset;

		Vector2 ScopeSway;
		uint32_t count = 0;
		clock_t time = 0, oldtime = 0;
		Vector3 lastsnapshot;
		Vector3 velocity;

		struct posTime
		{
			Vector3 pos;
			clock_t time;
		};
		std::vector<posTime> lastsnap;
		bool skip;
		uint32_t skipCount;
	};
	std::vector<address> a;
	std::vector<value> v;
	std::vector<uint64_t> SP;
	uint8_t page0[0x1000] = { 0 };
	uint8_t page1[0x1000] = { 0 };
	uint8_t var0[0x100] = { 0 };
	uint8_t var1[0x100] = { 0 };
	uint64_t pSteamPlayer;


	struct Menu
	{
		bool EnableAimbot = false,
			EnableCompass = false,
			EnableGps = false,
			EnableAimdot = false,
			EnableSalvage = false;

		int AimbotTarget;
		float Smoothing = 0.f,
			FOV = 0.f,
			Spread = 0.f,
			Recoil = 0.f,
			RenderDist = 0.f,
			Range = 0.f,
			Offset = 0.f,
			Pitch = 0.f,
			Yaw = 0.f,
			Roll = 0.f,
			Time = 0.f,
			ExitTimer = 0.f,
			SalvageTime = 0.f,
			MissChance = 0.f;
	}gui;

	uint32_t LocalID = -1, TargetID = -1, oldTargetID = -1;
	float shortest_distance;
	bool aimbotSet = false;

	Skeleton Bone;
	clock_t time;
	void UpdatePlayer(uint64_t index)
	{

		if (a.size() != SP.size())// If change is detected
		{
			std::vector<address> A;
			std::vector<value> V;
			A.resize(SP.size());
			V.resize(SP.size());

			bool isValid = false;

			if (a.size() > SP.size())// Player Left
			{
				for (int i = 0; i < a.size(); i++)
					for (int _i = 0; _i < SP.size(); _i++)
						if (a.at(i).SteamPlayer == SP.at(_i))
						{
							A.at(_i) = a.at(i);
							V.at(_i) = v.at(i);
							if (v.at(i).isLocal) { LocalID = _i; }
							break;
						}
				//remove invalid from list
				a = A;
				v = V;
				A.empty();
				V.empty();
			}
			else// Player Joined
			{
				for (int _i = 0; _i < SP.size(); _i++)
				{
					isValid = false;
					for (int i = 0; i < a.size(); i++)
					{
						if (a.at(i).SteamPlayer == SP.at(_i))
						{
							isValid = true;
							A.at(_i) = a.at(i);
							V.at(_i) = v.at(i);
							if (v.at(i).isLocal) { LocalID = _i; }
							break;
						}
					}
					if (!isValid) // Found missing player adding to list
					{
						address _A;
						value _V;
						// =========== INIT NEW PLAYER ============

						// SteamPlayer
						_A.SteamPlayer = SP.at(_i);
						_V.lastsnap.resize(200);
						read(_A.SteamPlayer, (uint64_t)page1, 0x1000);
						uint64_t Player = *(uint64_t*)&page1[SteamPlayer._player.self()];
						uint64_t PlayerId = *(uint64_t*)&page1[SteamPlayer._playerID.self()];
						_V._characterName = read<std::wstring>({ PlayerId + SteamPlayer._playerID._characterName.self(), SteamPlayer._playerID._characterName.Value });
						// SteamPlayer->_Player
						read(Player, (uint64_t)page1, 0x1000);
						_A._animator = *(uint64_t*)&page1[SteamPlayer._player._animator.self()];
						_A._equipment = *(uint64_t*)&page1[SteamPlayer._player._equipment.self()];
						_A._life = *(uint64_t*)&page1[SteamPlayer._player._life.self()];
						_A._movement = *(uint64_t*)&page1[SteamPlayer._player._movement.self()];
						_A._look = *(uint64_t*)&page1[SteamPlayer._player._look.self()];
						_A._skills = *(uint64_t*)&page1[SteamPlayer._player._skills.self()];
						_A._stance = *(uint64_t*)&page1[SteamPlayer._player._stance.self()];
						_A._interact = *(uint64_t*)&page1[SteamPlayer._player._interact.self()];
						_V.isLocal = *(uint64_t*)&page1[SteamPlayer._player.onLocalPluginWidgetFlagsChanged] ? true : false;
						// Update Local Player
						if (_V.isLocal)
						{
							LocalID = _i;
							_A.allowFreecamWithoutAdmin = _A._look + SteamPlayer._player._look.allowFreecamWithoutAdmin;
							_A.allowWorkzoneWithoutAdmin = _A._look + SteamPlayer._player._look.allowWorkzoneWithoutAdmin;
							_A._perspective = _A._look + SteamPlayer._player._look._perspective;
							_A.isIgnoringInput = _A._look + SteamPlayer._player._look.isIgnoringInput;
							_A._angles = _A._look + SteamPlayer._player._look._angles;
						}
						// =========== END INIT NEW PLAYER ============
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
			if (index == LocalID)
			{
				V.ScopeSway = read<Vector2>({ A._animator + SteamPlayer._player._animator.scopeSway });
				read(A._look, (uint64_t)page0, 0x200);
				V.isZoomed = *(byte*)&page0[SteamPlayer._player._look.isZoomed];
				V._angles = *(Vector2*)&page0[SteamPlayer._player._look._angles];
				V.isIgnoringInput = *(byte*)&page0[SteamPlayer._player._look.isIgnoringInput];
				//if (*(bool*)&page0[SteamPlayer._player._look.allowFreecamWithoutAdmin] != EnableFreeCam)
					//write<bool>(A._look + SteamPlayer._player._look.allowFreecamWithoutAdmin, EnableFreeCam);

				read(A._equipment, (uint64_t)page0, 0x200);
				read(*(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()], (uint64_t)page1, 0x300);
				V.type = *(uint32_t*)&page1[SteamPlayer._player._equipment._asset.type];
				switch (V.type)
				{
				case GUN:
				{
					V.range = *(float*)&page1[SteamPlayer._player._equipment._asset.range];
					V.ballisticSteps = *(byte*)&page1[SteamPlayer._player._equipment._asset.ballisticSteps];
					V.ballisticTravel = *(float*)&page1[SteamPlayer._player._equipment._asset.ballisticTravel];
					V.muzzleVelocity = *(float*)&page1[SteamPlayer._player._equipment._asset.muzzleVelocity];
					V.bulletGravityMultiplier = *(float*)&page1[SteamPlayer._player._equipment._asset.bulletGravityMultiplier];
					V.aimingRecoilMultiplier = *(uint32_t*)&page1[SteamPlayer._player._equipment._asset.aimingRecoilMultiplier];
					V.baseSpreadAngleRadians = *(uint32_t*)&page1[SteamPlayer._player._equipment._asset.baseSpreadAngleRadians];
				}break;
				case BARRICADE:
				{
					V._health = *(uint16_t*)&page1[SteamPlayer._player._equipment._asset._health];
					V._range = *(float*)&page1[SteamPlayer._player._equipment._asset._range];
					V._radius = *(float*)&page1[SteamPlayer._player._equipment._asset._radius];
					V._offset = *(float*)&page1[SteamPlayer._player._equipment._asset._offset];
				}break;
				case CHARGE:
				{
					V._health = *(uint16_t*)&page1[SteamPlayer._player._equipment._asset._health];
					V._range = *(float*)&page1[SteamPlayer._player._equipment._asset._range];
					V._radius = *(float*)&page1[SteamPlayer._player._equipment._asset._radius];
					V._offset = *(float*)&page1[SteamPlayer._player._equipment._asset._offset];
				}break;
				case STRUCTURE:
				{
					V._health = *(uint16_t*)&page1[SteamPlayer._player._equipment._asset._health];
					V._range = *(float*)&page1[SteamPlayer._player._equipment._asset._range];
				}break;
				default:
					break;
				}
				read(*(uint64_t*)&page0[SteamPlayer._player._equipment._useable.self()], (uint64_t)page1, 0x300);
				switch (V.type)
				{
				case GUN:
				{
					V.isAiming = *(byte*)&page1[SteamPlayer._player._equipment._useable.isAiming];
					V.ammo = *(byte*)&page1[SteamPlayer._player._equipment._useable.ammo];
					V.zoom = *(float*)&page1[SteamPlayer._player._equipment._useable.zoom];
				}break;
				case BARRICADE:
				{
					V.point = *(Vector3*)&page1[SteamPlayer._player._equipment._useable.point];
					V.input = *(Vector3*)&page1[SteamPlayer._player._equipment._useable.input];
					V.rotate = *(Vector3*)&page1[SteamPlayer._player._equipment._useable.rotate];
				}break;
				case CHARGE:
				{
					V.point = *(Vector3*)&page1[SteamPlayer._player._equipment._useable.point];
					V.input = *(Vector3*)&page1[SteamPlayer._player._equipment._useable.input];
					V.rotate = *(Vector3*)&page1[SteamPlayer._player._equipment._useable.rotate];
				}break;
				case STRUCTURE:
				{
					V.customRotationOffset = *(float*)&page1[SteamPlayer._player._equipment._useable.customRotationOffset];
					V.FoundationPositionOffset = *(float*)&page1[SteamPlayer._player._equipment._useable.FoundationPositionOffset];
				}break;
				default:
					break;
				}
				read(A._interact, (uint64_t)page0, 0x200);
				V.overrideSalvageTimeValue = *(float*)&page0[SteamPlayer._player._interact.overrideSalvageTimeValue];
				V.shouldOverrideSalvageTime = *(bool*)&page0[SteamPlayer._player._interact.shouldOverrideSalvageTime];
				if (V.shouldOverrideSalvageTime != gui.EnableSalvage)
				{
					write<bool>(A._interact + SteamPlayer._player._interact.shouldOverrideSalvageTime, gui.EnableSalvage);
				}
				if (V.overrideSalvageTimeValue != gui.SalvageTime)
				{
					if (V.shouldOverrideSalvageTime)
						write<float>(A._interact + SteamPlayer._player._interact.overrideSalvageTimeValue, gui.SalvageTime);
				}
				a.at(index) = A;
				v.at(index) = V;
				return;
			}
			if (!V.skipCount)
			{
				read(A._movement, (uint64_t)page0, 0x200);
				uint64_t _Vehicle = *(uint64_t*)&page0[SteamPlayer._player._movement._Vehicle.self()];
				if (_Vehicle)
				{
					read(_Vehicle, (uint64_t)page1, 0x300);
					if (V._vehicleName.size() == 0)
						V._vehicleName = read<std::wstring>({ *(uint64_t*)&page1[SteamPlayer._player._movement._Vehicle._asset.self()] + SteamPlayer._player._movement._Vehicle._asset._vehicleName.self(), SteamPlayer._player._movement._Vehicle._asset._vehicleName.Value });
					V.lastUpdatePos = *(Vector3*)&page1[SteamPlayer._player._movement._Vehicle.lastUpdatePos];
				}
				else
					V._vehicleName.resize(0);
				V._isGrounded = *(byte*)&page0[SteamPlayer._player._movement._isGrounded];
				V._isMoving = *(byte*)&page0[SteamPlayer._player._movement._isMoving];
				V.seat = *(byte*)&page0[SteamPlayer._player._movement.seat];
				V.snapshot = *(Vector3*)&page0[SteamPlayer._player._movement.snapshot];
				float distance = Camera.translation.distance(V.snapshot);
				if (_Vehicle)
					distance = Camera.translation.distance(V.lastUpdatePos);
				if (distance > gui.RenderDist)
				{
					V.skipCount = 100 + rand() % 100;
					V.equipmentName.resize(0);
				}
				else
				{
					V.skipCount = 0;
					auto base = WorldToScreen(V.snapshot);
					if (_Vehicle)
						base = WorldToScreen(V.lastUpdatePos);
					if (base.z < 0.f)
						V.skip = false;
				}
			}
			else
				V.skipCount--;
			if (!V.skip)// if Player is on screen and within max render update rest of player
			{
				for (int o = 1; o < V.lastsnap.size(); o++)
				{
					V.lastsnap.at(o - 1).pos = V.lastsnap.at(o).pos;
					V.lastsnap.at(o - 1).time = V.lastsnap.at(o).time;
				}
				V.lastsnap.at(V.lastsnap.size() - 1).pos = V.snapshot;
				V.lastsnap.at(V.lastsnap.size() - 1).time = time;
				V.velocity = 0;
				int count = 0;
				for (int o = V.lastsnap.size() - 1; o >= 0; o--)
				{
					if (V.lastsnap.at(o).time != 0)
					{
						if (time - V.lastsnap.at(o).time > 200)
						{

							V.velocity /= count;

							Vector3 test = (V.snapshot - V.lastsnap.at(o).pos) * 0.1f;
							float diff = test.distance(V.velocity * 0.2f);
							V.velocity *= 0.2f;
							V.velocity += test;
							V.velocity *= 0.5f;
							break;
						}
						else
						{
							count++;
							V.velocity += (V.snapshot - V.lastsnap.at(o).pos);
						}
					}
				}
				V._isDead = read<byte>(A._life + SteamPlayer._player._life._isDead);
				if (V._isDead)
				{
					std::wstring txt = V._characterName + L"(" + std::to_wstring(V._reputation) + L") " + std::to_wstring((int)V.snapshot.distance(Camera.translation)) + L"m";
					D2D1_COLOR_F C = color(0, 0, 255, 100);
					auto base = WorldToScreen(V.snapshot);
					draw->TextW(Vector2(base.x - txt.size() * (2.95f * (10 / 12.5f)), base.y), txt, 10, C);
					a.at(index) = A;
					v.at(index) = V;
					return;
				}
				read(A._look, (uint64_t)page0, 0x200);
				V._angles = *(Vector2*)&page0[SteamPlayer._player._look._angles];
				read(A._equipment, (uint64_t)page0, 0x200);
				uint64_t _asset = *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()];
				if (_asset && V.equipmentName.size() == 0)
				{
					read(_asset, (uint64_t)page1, 0x200);
					V.equipmentName = read<std::wstring>({ *(uint64_t*)&page1[SteamPlayer._player._equipment._asset.name.self()] + SteamPlayer._player._equipment._asset.name.Value });
				}
				else if (!_asset)
					V.equipmentName.resize(0);
				V.stance = read<uint32_t>(A._stance + SteamPlayer._player._stance._stance);
				V._reputation = read<int>(A._skills + SteamPlayer._player._skills._reputation);

				Bone = Skeleton(V.snapshot, V._angles, V.stance);
				if (V._vehicleName.size() == 0)
				{
					Vector3 w2sHead;
					D2D1_COLOR_F c;
					float dist = Camera.translation.distance(Bone.head()) - Camera.translation.distance(Bone.aimLine());
					float lineSize = Bone.head().distance(Bone.aimLine());
					if (dist < 0)
						c = color(0, 255 - (255 * (-dist / lineSize)), 255 * (-dist / lineSize), 255);
					else
						c = color(255 * (dist / lineSize), 255 - (255 * (dist / lineSize)), 0, 255);
					auto a = WorldToScreen(Bone.aimLine());
					auto b = WorldToScreen(Bone.head());

					if (LocalID != -1)
					{
						Vector3 tBone = 0;
						switch (gui.AimbotTarget)
						{
						case 0:
							tBone = Bone.head();
							break;
						case 1:
							tBone = Bone.neck();
							break;
						case 2:
							tBone = Bone.hip();
							break;
						default:
							break;
						}
						float distance = Camera.translation.distance(tBone);
						tBone += V.velocity * (distance / v.at(LocalID).ballisticTravel);// Calcuate Target Velocity
						tBone.y += distance * tanf(asin(distance * (9.81f * v.at(LocalID).bulletGravityMultiplier) / (v.at(LocalID).muzzleVelocity * v.at(LocalID).muzzleVelocity)) * 0.5f);

						tBone = WorldToScreen(tBone);

						if (tBone.z < 0)
						{
							dist = Vector2(tBone.x, tBone.y).distance(Camera.screen / 2);
							if (gui.FOV >= dist && shortest_distance > dist && LocalID != -1 && !V._isDead && v.at(LocalID).range > V.snapshot.distance(Camera.translation))
							{
								shortest_distance = dist;
								TargetID = index;
							}
						}
					}

					if (a.z < 0 && b.z < 0) { draw->Line(Vector2(a.x, a.y), Vector2(b.x, b.y), c, 1); }

					a = WorldToScreen(Bone.neck());
					if (a.z < 0 && b.z < 0) { draw->Line(Vector2(a.x, a.y), Vector2(b.x, b.y), c, 1); }
					a = WorldToScreen(Bone.lshoulder());
					b = WorldToScreen(Bone.lhip());
					if (a.z < 0 && b.z < 0) { draw->Line(Vector2(a.x, a.y), Vector2(b.x, b.y), c, 1); }
					a = WorldToScreen(Bone.rshoulder());
					b = WorldToScreen(Bone.rhip());
					if (a.z < 0 && b.z < 0) { draw->Line(Vector2(a.x, a.y), Vector2(b.x, b.y), c, 1); }
					a = WorldToScreen(Bone.lshoulder());
					b = WorldToScreen(Bone.rshoulder());
					if (a.z < 0 && b.z < 0) { draw->Line(Vector2(a.x, a.y), Vector2(b.x, b.y), c, 1); }
					a = WorldToScreen(Bone.lhip());
					b = WorldToScreen(Bone.rhip());
					if (a.z < 0 && b.z < 0) { draw->Line(Vector2(a.x, a.y), Vector2(b.x, b.y), c, 1); }
					a = WorldToScreen(Bone.lhip());
					b = WorldToScreen(Bone.lfoot());
					if (a.z < 0 && b.z < 0) { draw->Line(Vector2(a.x, a.y), Vector2(b.x, b.y), c, 1); }
					a = WorldToScreen(Bone.rhip());
					b = WorldToScreen(Bone.rfoot());
					if (a.z < 0 && b.z < 0) { draw->Line(Vector2(a.x, a.y), Vector2(b.x, b.y), c, 1); }

					auto base = WorldToScreen(V.snapshot);
					if (base.z < 0)
					{
						float scale = 3000 / Camera.translation.distance(Bone.head());
						if (scale >= 12.5f)
							scale = 12.5f;
						else if (scale <= 7.f)
							scale = 7.f;
						std::wstring txt = V._characterName + L"(" + std::to_wstring(V._reputation) + L") " + std::to_wstring((int)V.snapshot.distance(Camera.translation)) + L"m";
						float offset = 0.f;
						draw->TextW(Vector2(base.x - txt.size() * (2.95f * (scale / 12.5f)), base.y + offset), txt.c_str(), scale, c);
						if (V.equipmentName.size())
						{
							offset += scale;
							txt = V.equipmentName;
							draw->TextW(Vector2(base.x - txt.size() * (2.95f * (scale / 12.5f)), base.y + offset), txt.c_str(), scale, c);
						}
						if (V._vehicleName.size())
						{
							offset += scale;
							txt = V._vehicleName;
							draw->TextW(Vector2(base.x - txt.size() * (2.95f * (scale / 12.5f)), base.y + offset), txt.c_str(), scale, c);
						}
					}
				}
				else
				{
					auto base = WorldToScreen(V.lastUpdatePos);
					if (base.z < 0)
					{
						float scale = 3000 / Camera.translation.distance(Bone.head());
						if (scale >= 12.5f)
							scale = 12.5f;
						else if (scale <= 7.f)
							scale = 7.f;
						std::wstring txt = V._characterName + L"(" + std::to_wstring(V._reputation) + L") " + std::to_wstring((int)V.lastUpdatePos.distance(Camera.translation)) + L"m";
						D2D1_COLOR_F C = (oldTargetID == index) ? RED : BLUE;
						float offset = scale * 3.0f;
						offset *= V.seat;
						draw->TextW(Vector2(base.x - txt.size() * (2.95f * (scale / 12.5f)), base.y + offset), txt.c_str(), scale, C);
						if (V.equipmentName.size())
						{
							offset += scale;
							txt = V.equipmentName;
							draw->TextW(Vector2(base.x - txt.size() * (2.95f * (scale / 12.5f)), base.y + offset), txt.c_str(), scale, C);
						}
						if (V._vehicleName.size())
						{
							offset += scale;
							txt = V._vehicleName;
							draw->TextW(Vector2(base.x - txt.size() * (2.95f * (scale / 12.5f)), base.y + offset), txt.c_str(), scale, C);
						}
					}
				}
			}
			a.at(index) = A;
			v.at(index) = V;
		}
	}

	uint32_t PlayerSize()
	{
		uint32_t ret = 0;
		read(pSteamPlayer, (uint64_t)var0, 0x20);
		ret = *(uint32_t*)(var0 + 0x18);
		SP.resize(ret);
		read(*(uint64_t*)(var0 + 0x10), (uint64_t)page0, 0x1000);
		for (int i = 0; i < ret; i++)
		{
			SP.at(i) = *(uint64_t*)&page0[0x20 + 8 * i];
		}
		return ret;
	}
	Aimbot aimbot;
public:
	bool desyncFreeBuild = false;
	Player(uint64_t pSteamPlayer) : pSteamPlayer(pSteamPlayer)
	{
		a.resize(0);
		v.resize(0);
		SP.resize(0);
	}

	void EnableAimbot(bool value)
	{
		gui.EnableAimbot = value;
	}

	bool EnableAimbot()
	{
		return gui.EnableAimbot;
	}

	void EnableCompass(bool value)
	{
		gui.EnableCompass = value;
	}

	bool EnableCompass()
	{
		return gui.EnableCompass;
	}

	void EnableGps(bool value)
	{
		gui.EnableGps = value;
	}

	bool EnableGps()
	{
		return gui.EnableGps;
	}

	void EnableAimdot(bool value)
	{
		gui.EnableAimdot = value;
	}

	bool EnableAimdot()
	{
		return gui.EnableAimdot;
	}

	void EnableSalvage(bool value)
	{
		gui.EnableSalvage = value;
	}

	bool EnableSalvage()
	{
		return gui.EnableSalvage;
	}

	void AimbotTarget(int value)
	{
		gui.AimbotTarget = value;
	}

	int AimbotTarget()
	{
		return gui.AimbotTarget;
	}

	void Smoothing(float value)
	{
		gui.Smoothing = value;
	}

	float Smoothing()
	{
		return gui.Smoothing;
	}

	void FOV(float value)
	{
		gui.FOV = value;
	}

	float FOV()
	{
		return gui.FOV;
	}

	void Spread(float value)
	{
		gui.Spread = value;
	}

	float Spread()
	{
		return gui.Spread;
	}

	void Recoil(float value)
	{
		gui.Recoil = value;
	}

	float Recoil()
	{
		return gui.Recoil;
	}

	void RenderDist(float value)
	{
		gui.RenderDist = value;
	}

	float RenderDist()
	{
		return gui.RenderDist;
	}

	void Range(float value)
	{
		gui.Range = value;
	}

	float Range()
	{
		return gui.Range;
	}

	void Offset(float value)
	{
		gui.Offset = value;
	}

	float Offset()
	{
		return gui.Offset;
	}

	void Pitch(float value)
	{
		gui.Pitch = value;
	}

	float Pitch()
	{
		return gui.Pitch;
	}

	void Yaw(float value)
	{
		gui.Yaw = value;
	}

	float Yaw()
	{
		return gui.Yaw;
	}

	void Roll(float value)
	{
		gui.Roll = value;
	}

	float Roll()
	{
		return gui.Roll;
	}

	void AimMissChance(float value)
	{
		gui.MissChance = value;
	}

	float AimMissChance()
	{
		return gui.MissChance;
	}

	void Time(float value)
	{
		gui.Time = value;
	}

	float Time()
	{
		return gui.Time;
	}

	void ExitTimer(float value)
	{
		gui.ExitTimer = value;
	}

	float ExitTimer()
	{
		return gui.ExitTimer;
	}

	void SalvageTime(float value)
	{
		gui.SalvageTime = value;
	}

	bool SalvageTime()
	{
		return gui.SalvageTime;
	}

	void Update()
	{
		time = clock();
		if (LocalID != -1)
		{
			if (v.at(LocalID).type == GUN)
			{
				bool key = GetAsyncKeyState(VK_LBUTTON);
				if (gui.Smoothing == 0.f)
					key = GetAsyncKeyState(VK_RBUTTON);
				if ((gui.Spread / 10.f != v.at(LocalID).baseSpreadAngleRadians || gui.Recoil != v.at(LocalID).aimingRecoilMultiplier))
				{
					read(a.at(LocalID)._equipment, (uint64_t)page0, 0x200);
					if (read<uint32_t>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset.type }) == GUN)
					{
						if (gui.Spread / 10.f != v.at(LocalID).baseSpreadAngleRadians)
							write<float>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset.baseSpreadAngleRadians }, gui.Spread / 10.f);
						if (gui.Recoil != v.at(LocalID).aimingRecoilMultiplier)
							write<Vector4>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset.aimingRecoilMultiplier }, Vector4(1.f, 1.25f, 0.85f, 0.7f) * gui.Recoil);

					}
				}
				if (TargetID != -1 && (gui.EnableAimdot || gui.EnableAimbot))
				{
					Vector3 h = 0;
					switch (gui.AimbotTarget)
					{
					case 0:
						h = Skeleton(v.at(TargetID).snapshot, v.at(TargetID)._angles, v.at(TargetID).stance).head();
						break;
					case 1:
						h = Skeleton(v.at(TargetID).snapshot, v.at(TargetID)._angles, v.at(TargetID).stance).neck();
						break;
					case 2:
						h = Skeleton(v.at(TargetID).snapshot, v.at(TargetID)._angles, v.at(TargetID).stance).hip();
						break;
					default:
						break;
					}
					float distance = Camera.translation.distance(h);
					h += v.at(TargetID).velocity * (distance / v.at(LocalID).ballisticTravel);// Calcuate Target Velocity
					h.y += distance * tanf(asin(distance * (9.81f * v.at(LocalID).bulletGravityMultiplier) / (v.at(LocalID).muzzleVelocity * v.at(LocalID).muzzleVelocity)) * 0.5f);
					Vector3 W2s = WorldToScreen(h);
					if (W2s.z < 0)
					{
						if (gui.EnableAimdot)
							draw->Line(Vector2(W2s.x, W2s.y), Camera.screen / 2, RED);
						if (gui.EnableAimbot && v.at(LocalID).type == GUN)
						{
							if (key && read<uint32_t>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset.type }) == GUN)
							{
								if (!aimbotSet)
								{
									if (gui.Smoothing == 0.f)
										write<byte>({ a.at(LocalID)._look + SteamPlayer._player._look.isIgnoringInput }, 0x1);
									aimbotSet = true;
								}
								write<Vector2>({ a.at(LocalID)._look + SteamPlayer._player._look._angles }, aimbot.CalcuateAngles(Camera.translation, h, read<Vector2>({ a.at(LocalID)._look + SteamPlayer._player._look._angles }), (v.at(LocalID).isZoomed ? v.at(LocalID).ScopeSway : 0.f), gui.MissChance, gui.Smoothing));
							}
							draw->RoundRect(Camera.screen / 2 - gui.FOV, gui.FOV * 2, gui.FOV * 2, CLEAR, 0, 0, color(255, 255, 255, 200), 1);
						}
					}
				}
				if (!key && aimbotSet)
				{
					if (v.at(LocalID).isIgnoringInput)
						write<byte>({ a.at(LocalID)._look + SteamPlayer._player._look.isIgnoringInput }, 0x0);
					aimbotSet = false;
				}
			}
			else if (v.at(LocalID).type == BARRICADE || v.at(LocalID).type == CHARGE)
			{
				Vector3 state = Vector3(gui.Pitch, gui.Yaw, gui.Roll) * 360.f;
				Vector3 input = v.at(LocalID).input;
				Vector3 rotate = v.at(LocalID).rotate;
				if (input != state || desyncFreeBuild)
				{
					read(a.at(LocalID)._equipment, (uint64_t)page0, 0x200);
					if (read<bool>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._useable.self()] + SteamPlayer._player._equipment._useable.inputWantsToRotate }))
					{
						desyncFreeBuild = true;
						gui.Pitch = v.at(LocalID).input.x / 360.f;
						gui.Yaw = v.at(LocalID).input.y / 360.f;
						gui.Roll = v.at(LocalID).input.z / 360.f;
					}
					else
					{
						if (!read<bool>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._useable.self()] + SteamPlayer._player._equipment._useable.isUsing }) && read<uint32_t>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset.type }) == BARRICADE)
						{
							write<Vector3>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._useable.self()] + SteamPlayer._player._equipment._useable.rotate }, state);
							write<Vector3>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._useable.self()] + SteamPlayer._player._equipment._useable.input }, state);
						}
						desyncFreeBuild = false;
					}
				}
				if (v.at(LocalID)._range != gui.Range * 13.f)
				{
					read(a.at(LocalID)._equipment, (uint64_t)page0, 0x200);
					if (read<uint32_t>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset.type }) == BARRICADE)
						write<float>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset._range }, gui.Range * 13.f);
				}

				if (v.at(LocalID).type == CHARGE && v.at(LocalID)._range != (gui.Offset - 0.5f) * 3.f)
				{
					read(a.at(LocalID)._equipment, (uint64_t)page0, 0x200);
					if(read<uint32_t>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset.type }) == CHARGE)
						write<float>({ *(uint64_t*)&page0[SteamPlayer._player._equipment._asset.self()] + SteamPlayer._player._equipment._asset._offset }, (gui.Offset - 0.5f) * 3.f);
				}
			}
			else if(v.at(LocalID).isIgnoringInput)
				write<byte>({ a.at(LocalID)._look + SteamPlayer._player._look.isIgnoringInput }, 0x0);
		}
		oldTargetID = TargetID;
		TargetID = -1;
		shortest_distance = 999999.f;
		clock_t time = clock();
		uint32_t size = PlayerSize();
		for (int i = 0; i < size; i++)
		{
			UpdatePlayer(i);
		}
	}

	Vector3 GetFreeBuild()
	{
		return Vector3(gui.Pitch, gui.Yaw, gui.Roll);
	}

	bool GetShouldSalvage()
	{
		if (LocalID != -1)
			return v.at(LocalID).shouldOverrideSalvageTime;
		return 0;
	}

	float GetSalvageTime()
	{
		if (LocalID != -1)
			return v.at(LocalID).overrideSalvageTimeValue;
		return -100.f;
	}
};