#pragma once
class Updatable
{
public:

	Updatable()
	{
	}

	virtual ~Updatable()
	{
	}

	virtual void update(float delta) = 0;
};

