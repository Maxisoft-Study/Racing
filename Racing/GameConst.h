#pragma once
#include <vector>
class GameConst
{
public:
	GameConst(void){}
	~GameConst(void){}
	static std::vector<int> *handlers;
	static void init()
	{
		GameConst::handlers = new std::vector<int>;
	}
};

