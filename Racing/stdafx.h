// stdafx.h : fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
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




// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
