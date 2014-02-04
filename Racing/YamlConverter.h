#pragma once
#include "stdafx.h"
#include "CarControler.h"

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

	//CONVERT to CarControlKeysDef
	template<>
	struct convert<CarControlKeysDef> {
		static Node encode(const CarControlKeysDef& rhs) {
			Node node;
			node["movebackward"] = racing::STRING_TO_SFML_KEYBOARD_KEY.right.find(rhs.MoveBackWard)->second;
			node["moveforward"] = racing::STRING_TO_SFML_KEYBOARD_KEY.right.find(rhs.MoveForward)->second;
			node["rotateleft"] = racing::STRING_TO_SFML_KEYBOARD_KEY.right.find(rhs.RotateLeft)->second;
			node["rotateright"] = racing::STRING_TO_SFML_KEYBOARD_KEY.right.find(rhs.RotateRight)->second;
			return node;
		}

		static bool decode(const Node& node, CarControlKeysDef& rhs) {
			rhs.MoveBackWard = racing::STRING_TO_SFML_KEYBOARD_KEY.left.find(node["movebackward"].as<std::string>())->second;
			rhs.MoveForward = racing::STRING_TO_SFML_KEYBOARD_KEY.left.find(node["moveforward"].as<std::string>())->second;
			rhs.RotateLeft = racing::STRING_TO_SFML_KEYBOARD_KEY.left.find(node["rotateleft"].as<std::string>())->second;
			rhs.RotateRight = racing::STRING_TO_SFML_KEYBOARD_KEY.left.find(node["rotateright"].as<std::string>())->second;
			return true;
		}
	};


}