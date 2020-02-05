#pragma once
#include "stdafx.h"

#include <map>
#include "assimp/scene.h"
#include "AnimationLoader.h"
#include "Skeleton.h"

class SkeletonLoader
{
public:
    std::map<std::string, Bone*> m_BoneMap;
    std::vector<Bone*> m_Bones;

    SkeletonLoader(const aiScene* scene);
    ~SkeletonLoader();
    Skeleton* GenerateSkeleton(aiNode* root);
    int AddBone(aiBone* bone);
    void ProcessBones(aiNode* node, Bone* bone);
    void LoadAnimations(Skeleton* skeleton);
private:
    aiNode* FindBoneRoot(aiNode* root);
    AnimationLoader m_AnimationLoader;
    const aiScene* m_Scene;
    int m_BoneReached = 0;
};

SkeletonLoader::SkeletonLoader(const aiScene* scene) :
    m_Scene(scene)
{
}

int SkeletonLoader::AddBone(aiBone* aibone)
{
    std::string boneName(aibone->mName.data);
    if (m_BoneMap.find(boneName) == m_BoneMap.end())
    {
        Bone* bone = new Bone(boneName, m_Bones.size(), XMMatrixTranspose(XMMATRIX(&aibone->mOffsetMatrix.a1)));
        m_Bones.push_back(bone);
        m_BoneMap.emplace(boneName, bone);
    }
    return m_BoneMap[boneName]->m_Index;
}

aiNode* SkeletonLoader::FindBoneRoot(aiNode* nodeRoot)
{
    // BFS
    std::queue<aiNode*> queue;
    queue.push(nodeRoot);

    while (!queue.empty())
    {
        aiNode* node = queue.front();
        queue.pop();

        std::string nodeName(node->mName.data);
        if (m_BoneMap.find(nodeName) != m_BoneMap.end())
        {
            return node->mParent;
        }
        for (UINT i = 0; i < node->mNumChildren; ++i)
        {
            aiNode* childNode = node->mChildren[i];
            queue.push(childNode);
        }
    }
    return nullptr;
}

Skeleton* SkeletonLoader::GenerateSkeleton(aiNode* root)
{
    Skeleton* skeleton = new Skeleton();
    aiNode* boneRoot = FindBoneRoot(root);

    // Parent is not a bone recorded
    Bone* parent = new Bone(std::string(boneRoot->mName.data), m_Bones.size(), XMMatrixIdentity());
    m_Bones.push_back(parent);

    skeleton->m_Root = parent;
    ProcessBones(boneRoot, parent);
    skeleton->m_Bones = m_Bones;
    skeleton->m_RootTransform = XMMATRIX(&boneRoot->mTransformation.a1);

    m_Bones.clear();
    skeleton->m_GlobalInverse = XMMatrixInverse(NULL,XMMATRIX(&boneRoot->mTransformation.a1));

    LoadAnimations(skeleton);
    return skeleton;
}

void SkeletonLoader::ProcessBones(aiNode* node, Bone* bone)
{
    for (UINT i = 0; i < node->mNumChildren; ++i)
    {
        aiNode* child = node->mChildren[i];
        if (m_BoneMap.find(child->mName.data) != m_BoneMap.end())
        {
            Bone* childBone = m_BoneMap[child->mName.data];
            bone->m_Children.push_back(childBone);
            ProcessBones(child, childBone);
        }
    }
}

void SkeletonLoader::LoadAnimations(Skeleton* skeleton)
{
    for (UINT i = 0; i < m_Scene->mNumAnimations; ++i)
    {
        Animation anim = m_AnimationLoader.LoadAnimation(m_Scene->mAnimations[i]);
        skeleton->m_Animations.push_back(anim);
    }
}

SkeletonLoader::~SkeletonLoader()
{
    for (Bone* bone : m_Bones)
    {
        delete bone;
    }
    m_Bones.clear();
}
