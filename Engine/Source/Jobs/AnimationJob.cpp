#include "stdafx.h"

#include "AnimationJob.h"
#include "Scene/Scene.h"
#include "Scene/Model/Skeleton.h"

using namespace DirectX;

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

template <typename T>
int CurrentKeyIndex(std::vector<T> keys, double time)
{
    float duration = time;
    for (UINT i = 0; i < keys.size() - 1; ++i)
    {
        if (duration < keys[i + 1].m_Time)
        {
            return i;
        }
    }
    return -1;
}

void AnimationJob::Update(Scene& scene, float deltaTime)
{
    for (auto sceneObj : scene.m_Objects)
    {
        if (!sceneObj->m_Animator.m_IsEnabled) continue;

        Animator& animator = sceneObj->m_Animator;
        Skeleton* skeleton = animator.m_Skeleton;
        Animation& animation = skeleton->m_Animations[animator.m_AnimIndexChosen];

        animator.m_TimeElapsed += deltaTime * animation.m_TicksPerSecond;
        animator.m_TimeElapsed = fmod(animator.m_TimeElapsed, animation.m_Duration);
        UpdateBone(skeleton->m_Root, animator, skeleton->m_RootTransform, animator.m_TimeElapsed);
    }
}

void AnimationJob::UpdateBone(Bone* bone, Animator& animator, const XMMATRIX& parentTransform, float time)
{
    Skeleton* skeleton = animator.m_Skeleton;
    Animation& animation = skeleton->m_Animations[animator.m_AnimIndexChosen];
    std::string& boneName = bone->m_Name;
    NodeAnimation& nodeAnim = animation.m_NodeAnimations[boneName];
    float t = 0;

    // Translation
    XMVECTOR interpPos = nodeAnim.m_Positions[0].m_Position;
    if (nodeAnim.m_Positions.size() > 1)
    {
        int posKeyIdx = CurrentKeyIndex<NodeAnimation::PositionKey>(nodeAnim.m_Positions, time);
        int nPosKeyIdx = posKeyIdx + 1;

        NodeAnimation::PositionKey posKey = nodeAnim.m_Positions[posKeyIdx];
        NodeAnimation::PositionKey nPosKey = nodeAnim.m_Positions[nPosKeyIdx];

        t = (time - posKey.m_Time) / (nPosKey.m_Time - posKey.m_Time);
        interpPos = XMVectorLerp(posKey.m_Position, nPosKey.m_Position, t);
    }
    XMMATRIX translation = XMMatrixTranslationFromVector(interpPos);

    // Rotation
    XMVECTOR interpQuat = nodeAnim.m_Rotations[0].m_Quaternion;
    if (nodeAnim.m_Rotations.size() > 1)
    {
        int rotKeyIdx = CurrentKeyIndex<NodeAnimation::RotationKey>(nodeAnim.m_Rotations, time);
        int nRotKeyIdx = rotKeyIdx + 1;

        NodeAnimation::RotationKey rotKey = nodeAnim.m_Rotations[rotKeyIdx];
        NodeAnimation::RotationKey nRotKey = nodeAnim.m_Rotations[nRotKeyIdx];

        t = (time - rotKey.m_Time) / (nRotKey.m_Time - rotKey.m_Time);
        interpQuat = XMQuaternionSlerp(rotKey.m_Quaternion, nRotKey.m_Quaternion, t);

    }
    XMMATRIX rotation = XMMatrixRotationQuaternion(interpQuat);

    // Scaling
    float interpScale = nodeAnim.m_Scalings[0].m_Scaling.x;

    if (nodeAnim.m_Scalings.size() > 1)
    {
        int scalKeyIdx = CurrentKeyIndex<NodeAnimation::ScalingKey>(nodeAnim.m_Scalings, time);
        int nScalKeyIdx = scalKeyIdx + 1;

        NodeAnimation::ScalingKey scalKey = nodeAnim.m_Scalings[scalKeyIdx];
        NodeAnimation::ScalingKey nScalKey = nodeAnim.m_Scalings[nScalKeyIdx];

        t = (time - scalKey.m_Time) / (nScalKey.m_Time - scalKey.m_Time);
        interpScale = lerp(scalKey.m_Scaling.x, nScalKey.m_Scaling.x, t);
    }

    XMMATRIX scale = XMMatrixScaling(interpScale, interpScale, interpScale);

    XMMATRIX nodeTransform = scale * rotation * translation;
    XMMATRIX globalTransform = nodeTransform * parentTransform;

    animator.m_FinalTransforms[bone->m_Index] = bone->m_Offset * globalTransform * skeleton->m_GlobalInverse;
    bone->m_GlobalTransform = globalTransform;

    for (Bone* child : bone->m_Children)
    {
        UpdateBone(child, animator, globalTransform, time);
    }
}

