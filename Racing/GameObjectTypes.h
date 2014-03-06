#pragma once
////////////////////////////////////////////////////////////
/// \brief Enumeration des types des GameObjects present dans le jeu
///
////////////////////////////////////////////////////////////
enum GameObjectTypes : unsigned long long
{
	UnsetType = 0,
	GameObjectType = 1,
	SpriteGameObjectType = 1 << 1 | GameObjectType,
	BoxGameObjectType = 1 << 2 | GameObjectType,

	MixedGameObjectType = 1 << 3 | BoxGameObjectType | SpriteGameObjectType,

	CarType = 1 << 4 | MixedGameObjectType,
	WheelType = 1 << 5 | MixedGameObjectType,

	CheckpointType = 1 << 6 | BoxGameObjectType,
	GroundType = 1 << 7 | BoxGameObjectType,
	BarrierLimitType = 1 << 8 | BoxGameObjectType,
};