#pragma once
#include "stdafx.h"

#include <map>
#include <queue>

using namespace DirectX;

class Bone;
class Animation;

class Skeleton
{
public:
    Bone* m_Root;
    std::vector<Animation> m_Animations;
    std::vector<Bone*> m_Bones;
    XMMATRIX m_RootTransform;
    XMMATRIX m_GlobalInverse;
    ~Skeleton();
    static const UINT NUM_BONES = 50;
};

class Bone
{
public:
    std::string m_Name;
    int m_Index;
    XMMATRIX m_GlobalTransform;
    XMMATRIX m_Offset;
    std::vector<Bone*> m_Children;

    Bone(const std::string& name, int index, const XMMATRIX& offset) :
        m_Name(name),
        m_Index(index),
        m_Offset(offset),
        m_GlobalTransform(XMMatrixIdentity())
    {
    }
};

struct NodeAnimation
{
    std::string m_Name;

    struct PositionKey
    {
        XMVECTOR m_Position;
        double m_Time;
    };
    std::vector<PositionKey> m_Positions;

    struct RotationKey
    {
        XMVECTOR m_Quaternion;
        double m_Time;
    };
    std::vector<RotationKey> m_Rotations;

    struct ScalingKey
    {
        XMFLOAT3 m_Scaling;
        double m_Time;
    };
    std::vector<ScalingKey> m_Scalings;
};

class Animation
{
public:
    std::string m_Name;
    std::map<std::string, NodeAnimation> m_NodeAnimations;
    double m_Duration;
    double m_TicksPerSecond;
};

