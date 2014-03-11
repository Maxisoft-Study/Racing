#pragma once
#include "stdafx.h"
class InGameOverlay :
	virtual public sf::Drawable,
	virtual public Updatable
{
	friend class sf::RenderTarget;
public:
    InGameOverlay(sf::RenderWindow& window, const Car* carptr);
    ~InGameOverlay(void);

	virtual void update(float delta) final;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	
	sf::Font speedFont;
	sf::Text speedText;
	const Car* car;
};
