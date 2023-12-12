#pragma once
enum EPlayerStance
{
	CLIMB,
	SWIM,
	SPRINT,
	STAND,
	CROUCH,
	PRONE,
	DRIVING,
	SITTING
};

enum EItemType
{
	HAT,
	PANTS,
	SHIRT,
	MASK,
	BACKPACK,
	VEST,
	GLASSES,
	GUN,
	SIGHT,
	TACTICAL,
	GRIP,
	BARREL,
	MAGAZINE,
	FOOD,
	WATER,
	MEDICAL,
	MELEE,
	FUEL,
	TOOL,
	BARRICADE,
	STORAGE,
	BEACON,
	FARM,
	TRAP,
	STRUCTURE,
	SUPPLY,
	THROWABLE,
	GROWER,
	OPTIC,
	REFILL,
	FISHER,
	CLOUD,
	MAP,
	KEY,
	BOX,
	ARREST_START,
	ARREST_END,
	TANK,
	GENERATOR,
	DETONATOR,
	CHARGE,
	LIBRARY,
	FILTER,
	SENTRY,
	VEHICLE_REPAIR_TOOL,
	TIRE,
	COMPASS,
	OIL_PUMP
};


#define CameraBase 0x01B2A120
#define GameBase 0x01A88500
#define PlayerBase 0x1050
#define VehicleBase 0x2860
#define ConfigBase 0x1278
//ViewMatrix
//UnityPlayer.dll + CameraBase->0x1A0->0x48->0xB0->0x4E8

struct _MODECONFIGDATA//UnityPlayer.dll + GameBase->0xB0->0x60->ConfigBase
{
	struct _GAMEPLAY
	{
		uint64_t self() { return 0x58; }
		uint64_t Crosshair = 0x15;// byte
		uint64_t Chart = 0x17;// byte
		uint64_t Satellite = 0x18;//byte
		uint64_t Compass = 0x19;//byte
		uint64_t Allow_Shoulder_Camera = 0x20;//byte
		uint64_t Can_Suicide = 0x21;//byte
		uint64_t Timer_Exit = 0x28;//uint32_t
		uint64_t Timer_Respawn = 0x2C;//uint32_t
		uint64_t Timer_Home = 0x30;//uint32_t
		uint64_t Timer_Exit_Group = 0x34;//uint32_t
	}_Gameplay;
}ModeConfigData;

struct _InteractableVehicle// UnityPlayer.dll + GameBase->0xB0->0x60->VehicleBase->0x10->index * 8 + 0x20
{
	struct _ASSET
	{
		uint64_t self() { return 0x170; }
		struct _VEHICLENAME
		{
			uint64_t self() { return 0x50; }
			uint64_t Length = 0x10;// uint32_t
			uint64_t Value = 0x14;// wchar_t*
		}_vehicleName;
	}_asset;
	uint64_t lastUpdatePos = 0x2A0;// vector3
}InteractableVehicle;

struct _STEAMPLAYER// UnityPlayer.dll + GameBase->0xB0->0x60->PlayerBase->0x10->index * 8 + 0x20
{
	struct _PLAYERID
	{
		uint64_t self() { return 0x20; }
		struct _CHARACTERNAME
		{
			uint64_t self() { return 0x18; }
			uint64_t Length = 0x10;// uint32_t
			uint64_t Value = 0x14;// wchar_t*
		}_characterName;
	}_playerID;
	struct _PLAYER
	{
		uint64_t self() { return 0x30; }
		uint64_t onLocalPluginWidgetFlagsChanged = 0x28;
		struct _ANIMATOR
		{
			uint64_t self() { return 0x38; }
			uint64_t scopeSway = 0xB0;// Vector2
		}_animator;
		struct _EQUIPMENT
		{
			uint64_t self() { return 0x50; }
			struct _ASSET
			{
				uint64_t self() { return 0xF8; }
				struct _NAME
				{
					uint64_t self() { return 0x10; }
					uint64_t Length = 0x10;// uint32_t
					uint64_t Value = 0x14;// wchar_t*
				}name;
				uint64_t type = 0xEC;// uint32_t
				uint64_t range = 0x140;// float
				uint64_t baseSpreadAngleRadians = 0x260;// float
				uint64_t aimingRecoilMultiplier = 0x280;// float
				uint64_t ballisticSteps = 0x2B0;// byte
				uint64_t ballisticTravel = 0x2B4;// float
				uint64_t muzzleVelocity = 0x2B8;
				uint64_t bulletGravityMultiplier = 0x2BC;// float

				uint64_t _build = 0x170;// uint32_t
				uint64_t _health = 0x174;// uint16_t
				uint64_t _range = 0x178;// float
				uint64_t _radius = 0x17C;// float
				uint64_t _offset = 0x180;// float



			}_asset;
			struct _USEABLE
			{
				uint64_t self() { return 0x100; }
				uint64_t wasTriggerJustPulled = 0x1ED;// byte
				uint64_t isAiming = 0x1EF;// byte
				uint64_t ammo = 0x243;// byte
				uint64_t zoom = 0x248;// float
				uint64_t shouldZoomUsingEyes = 0x24C;// byte

				uint64_t inputWantsToRotate = 0xC0;// byte
				uint64_t isUsing = 0xC2;// byte
				uint64_t point = 0xF8;// Vector3
				uint64_t input = 0x11C;// Vector3
				uint64_t rotate = 0x110;// Vector3

				uint64_t FoundationPositionOffset = 0x70;// float
				uint64_t customRotationOffset = 0x6C;// float
			}_useable;
		}_equipment;
		struct _LIFE
		{
			uint64_t self() { return 0x58; }
			uint64_t _isDead = 0xC8;// byte
		}_life;
		struct _SKILLS
		{
			uint64_t self() { return 0x68; }
			uint64_t _reputation = 0x60;// int32_t
		}_skills;
		struct _MOVEMENT
		{
			uint64_t self() { return 0x70; }
			struct _VEHICLE
			{
				uint64_t self() { return 0xD8; }
				struct __ASSET
				{
					uint64_t self() { return 0x170; }
					struct _VEHICLENAME
					{
						uint64_t self() { return 0x50; }
						uint64_t Length = 0x10;// uint32_t
						uint64_t Value = 0x14;// wchar_t*
					}_vehicleName;
				}_asset;
				uint64_t lastUpdatePos = 0x2A0;// vector3
			}_Vehicle;
			uint64_t _isGrounded = 0x104;// byte
			uint64_t _isMoving = 0x145;// byte
			uint64_t snapshot = 0x184;// Vector3
			uint64_t seat = 0x1D4;// byte
		}_movement;
		struct _LOOK
		{
			uint64_t self() { return 0x78; }
			uint64_t _angles = 0x9C;// Vector2
			uint64_t isIgnoringInput = 0xD6;// byte
			uint64_t mainCameraZoomFactor = 0x15C;// float
			uint64_t scopeCameraZoomFactor = 0x160;// float
			uint64_t _perspective = 0x170;// uint32_t
			uint64_t isZoomed = 0x174;// byte
			uint64_t allowFreecamWithoutAdmin = 0x175;// byte
			uint64_t allowWorkzoneWithoutAdmin = 0x176;// byte
			uint64_t allowSpecStatsWithoutAdmin = 0x177;// byte
		}_look;
		struct _STANCE
		{
			uint64_t self() { return 0x80; }
			uint64_t _stance = 0x38;// uint32_t
		}_stance;
		struct _INTERACT
		{
			uint64_t self() { return 0x98; }
			uint64_t shouldOverrideSalvageTime = 0x38;// byte
			uint64_t overrideSalvageTimeValue = 0x3C;// float
		}_interact;

	}_player;
	uint64_t _isAdmin = 0xA0;// byte
}SteamPlayer;