#pragma once
#include "stdafx.h"

namespace YAML {



	//CONVERT sf VECTOR2i
	template<>
	struct convert<sf::Vector2i> {
		static Node encode(const sf::Vector2i& rhs) {
			Node node;
			node['x'] = rhs.x;
			node['y'] = rhs.y;
			return node;
		}

		static bool decode(const Node& node, sf::Vector2i& rhs) {
			if (!node.IsSequence() || node.size() < 2)
				return false;

			rhs.x = node['x'].as<int>();
			rhs.y = node['y'].as<int>();
			return true;
		}
	};


}