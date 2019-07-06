#include "stdafx.h"
#include "Material.h"
#include "Graphics.h"
#include "Texture.h"

Material::~Material()
{
    SafeRelease(m_Buffer);
}

Material::Material(Material&& material)
{
    m_Buffer = material.m_Buffer;
    material.m_Buffer = NULL;

    m_Diffuse = material.m_Diffuse;
    m_Normal = material.m_Normal;
    m_Specular = material.m_Specular;

    m_MaterialInfo = material.m_MaterialInfo;
}

void Material::Use(ID3D11DeviceContext* deviceContext)
{
    if (m_Diffuse) m_Diffuse->Use(deviceContext, 0);
    if (m_Normal) m_Normal->Use(deviceContext, 1);
    if (m_Specular) m_Specular->Use(deviceContext, 2);
    deviceContext->PSSetConstantBuffers(0, 1, &m_Buffer);
}

void Material::Update(Graphics& graphics)
{
    if (m_Buffer == nullptr)
    {
        m_Buffer = graphics.CreateBuffer(sizeof(MaterialInfo), D3D11_BIND_CONSTANT_BUFFER, &m_MaterialInfo);
    }
    else
    {
        graphics.UpdateBuffer(m_Buffer, &m_MaterialInfo);
    }
}

Material& Material::SetEmissive(float r, float g, float b)
{
    m_MaterialInfo.m_Emissive.x = r;
    m_MaterialInfo.m_Emissive.y = g;
    m_MaterialInfo.m_Emissive.z = b;
    return *this;
}


Material& Material::SetAmbient(float r, float g, float b)
{
    m_MaterialInfo.m_Ambient.x = r;
    m_MaterialInfo.m_Ambient.y = g;
    m_MaterialInfo.m_Ambient.z = b;
    return *this;
}

Material& Material::SetDiffuse(float r, float g, float b)
{
    m_MaterialInfo.m_Diffuse.x = r;
    m_MaterialInfo.m_Diffuse.y = g;
    m_MaterialInfo.m_Diffuse.z = b;
    return *this;
}

Material& Material::SetSpecular(float r, float g, float b)
{
    m_MaterialInfo.m_Specular.x = r;
    m_MaterialInfo.m_Specular.y = g;
    m_MaterialInfo.m_Specular.z = b;
    return *this;
}

Material& Material::SetShininess(float n)
{
    m_MaterialInfo.m_Shininess = n;
    return *this;
}

Material& Material::UseDiffuseMap(Texture* diffuse)
{
    m_Diffuse = diffuse;
    m_MaterialInfo.m_UseDiffuse = 1;
    return *this;
}

Material& Material::UseNormalMap(Texture* normal)
{
    m_Normal = normal;
    m_MaterialInfo.m_UseNormal = 1;
    return *this;
}

Material& Material::UseSpecularMap(Texture* specular)
{
    m_Specular = specular;
    m_MaterialInfo.m_UseSpecular = 1;
    return *this;
}
