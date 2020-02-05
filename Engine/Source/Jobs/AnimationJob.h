#pragma once
#include "stdafx.h"

class Scene;
class Bone;
class Animator;
class Animation;

class AnimationJob
{
public:
    void Update(Scene& scene, float deltaTime);
private:
    void UpdateBones(Animator& animator);
    void UpdateBone(Bone* bone,Animator& animator, const DirectX::XMMATRIX& transform, float time);
};

