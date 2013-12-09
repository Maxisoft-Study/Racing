#pragma once
class Ground
{
        friend class sf::RenderTarget;
public:
        Ground(void);
        ~Ground(void);
private:
        float frictionCoeff;
};
