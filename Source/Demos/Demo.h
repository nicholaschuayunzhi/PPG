#pragma once

class Graphics;

class Demo
{
public:
    virtual void Start(Graphics& graphics) = 0;
    virtual void Update(Graphics& graphics, float deltaTime) = 0;
    virtual void End() = 0;
};
