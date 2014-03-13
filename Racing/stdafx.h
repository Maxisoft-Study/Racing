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
#include <set>

#include <unordered_map>


//#include <memory>
#include <thread>
#include <mutex>


#include <sstream>

#include <cmath>

//////////////////////////////////////////////////////////////////////////
/// EXTERNAL HEADERS
//////////////////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <Box2D/Box2D.h>
#include <yaml-cpp/yaml.h>
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// BOOST HEADERS
//////////////////////////////////////////////////////////////////////////
/*#ifdef _MSC_VER
#include <boost/config/compiler/visualc.hpp>
#endif*/

#include <boost/algorithm/string/predicate.hpp>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/bimap.hpp>

//FILESYSTEM
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

//LOG
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
//////////////////////////////////////////////////////////////////////////

typedef unsigned int uint;

inline void dassert(bool b)
{
#ifdef _DEBUG
	assert(b);
#endif // _DEBUG
}


//////////////////////////////////////////////////////////////////////////
/// PROJECT HEADERS
//////////////////////////////////////////////////////////////////////////
#include "Updatable.h"
#include "GameObjectTypes.h"
#include "GConst.h"
#include "GGlobals.h"
#include "Utils.h"
#include "GameObject.h"
#include "TileMap.h"

#include "YamlConverters.h"
//////////////////////////////////////////////////////////////////////////