#pragma once
#include "stdafx.h"
class GameObject;

class GameObject
{
public:
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
	};

	const virtual GameObject::GameObjectTypes getGType(void) const;

	static bool IsTypeOf(const GameObject& gobj, const GameObject::GameObjectTypes t);
protected:
        GameObject();
        virtual ~GameObject(void);
};
