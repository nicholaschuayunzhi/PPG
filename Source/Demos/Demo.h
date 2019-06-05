#pragma once
#include "Input.h"

class Graphics;

class Demo
{
public:
    virtual void Start(Graphics& graphics) = 0;
    virtual void Update(Graphics& graphics, Input input, float deltaTime) = 0;
    virtual void End() = 0;
 };
