// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <unordered_map>

#include <memory>
#include <thread>

#include <sstream>

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <Box2D/Box2D.h>
#include <yaml-cpp/yaml.h>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

typedef unsigned int uint;

inline void dassert(bool b)
{
#ifdef _DEBUG
	assert(b);
#endif // _DEBUG
}

#include "GConst.h"
#include "GGlobals.h"
#include "Utils.h"
#include "GameObject.h"
#include "TileMap.h"




// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme
