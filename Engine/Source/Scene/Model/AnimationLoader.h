#pragma once
#include "stdafx.h"

#include <map>
#include "assimp/scene.h"
#include "Skeleton.h"

class AnimationLoader
{
public:
    Animation LoadAnimation(aiAnimation* animation);
    NodeAnimation LoadNodeAnimation(aiNodeAnim* aiNodeAnim);
};

Animation AnimationLoader::LoadAnimation(aiAnimation* aianimation)
{
    Animation animation;
    animation.m_Name = aianimation->mName.data;
    animation.m_Duration = aianimation->mDuration;
    animation.m_TicksPerSecond = aianimation->mTicksPerSecond;

    for (UINT i = 0; i < aianimation->mNumChannels; ++i)
    {
        aiNodeAnim* ainodeAnim = aianimation->mChannels[i];
        NodeAnimation nodeAnim = LoadNodeAnimation(ainodeAnim);
        animation.m_NodeAnimations.emplace(ainodeAnim->mNodeName.data, nodeAnim);
    }

    return animation;
}

NodeAnimation AnimationLoader::LoadNodeAnimation(aiNodeAnim* ainodeAnim)
{
    NodeAnimation nodeAnim;
    nodeAnim.m_Name = ainodeAnim->mNodeName.data;

    for (UINT i = 0; i < ainodeAnim->mNumPositionKeys; ++i)
    {
        aiVectorKey& aiPosKey = ainodeAnim->mPositionKeys[i];
        NodeAnimation::PositionKey posKey;
        posKey.m_Position = XMVectorSet(aiPosKey.mValue.x, aiPosKey.mValue.y, aiPosKey.mValue.z, 1);
        posKey.m_Time = aiPosKey.mTime;
        nodeAnim.m_Positions.push_back(posKey);
    }

    for (UINT i = 0; i < ainodeAnim->mNumRotationKeys; ++i)
    {
        aiQuatKey& aiRotKey = ainodeAnim->mRotationKeys[i];
        NodeAnimation::RotationKey rotKey;
        aiQuaternion& aiQuat = aiRotKey.mValue;
        rotKey.m_Quaternion = XMVectorSet(aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w);
        rotKey.m_Time = aiRotKey.mTime;
        nodeAnim.m_Rotations.push_back(rotKey);
    }

    for (UINT i = 0; i < ainodeAnim->mNumScalingKeys; ++i)
    {
        aiVectorKey& aiScalKeys = ainodeAnim->mScalingKeys[i];
        NodeAnimation::ScalingKey scalKey;
        scalKey.m_Scaling.x = aiScalKeys.mValue.x;
        scalKey.m_Scaling.y = aiScalKeys.mValue.y;
        scalKey.m_Scaling.z = aiScalKeys.mValue.z;
        scalKey.m_Time = aiScalKeys.mTime;
        nodeAnim.m_Scalings.push_back(scalKey);
    }

    return nodeAnim;
}
