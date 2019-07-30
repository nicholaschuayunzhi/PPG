#pragma once
#include "stdafx.h"

using namespace DirectX;
class Shader;
class Graphics;
class Scene;
class SceneObject;

class ForwardPass
{
public:
    ForwardPass(Graphics& graphics);
    void Render(Graphics& graphics, Scene& scene);
private:
    void DrawSceneRecursive(SceneObject& obj, XMMATRIX model);
    std::unique_ptr<Shader> shader;
    //values for convenience
    Graphics* _Graphics;
    Scene* _Scene;
};
