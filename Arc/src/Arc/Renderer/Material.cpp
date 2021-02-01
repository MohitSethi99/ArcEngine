#include "arcpch.h"
#include "Material.h"

#include "Shader.h"
#include "Texture.h"

namespace ArcEngine
{
	Ref<MaterialInstance> MaterialInstance::Create(MaterialInstance::Type type)
	{
		switch (type)
		{
			case (MaterialInstance::Type::PBR): return CreateRef<PbrMaterial>();
		}

		return nullptr;
	}

	PbrMaterial::PbrMaterial()
	{
		m_Shader = Shader::Create("assets/shaders/PBR.glsl");

		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		Ref<Texture2D> blackTexture = Texture2D::Create(1, 1);
		uint32_t blackTextureData = 0x00000000;
		whiteTexture->SetData(&blackTextureData, sizeof(uint32_t));

		AlbedoMap           = whiteTexture;
		MetallicMap         = blackTexture;
		NormalMap           = blackTexture;
		RoughnessMap        = blackTexture;
		AmbientOcclusionMap = blackTexture;
		EmissiveMap         = blackTexture;
	}

	void PbrMaterial::Bind() const
	{
		m_Shader->Bind();

		m_Shader->SetFloat4("u_Albedo", Color);
		m_Shader->SetFloat("u_Metallic", Metallic);
		m_Shader->SetFloat("u_Roughness", Roughness);
		m_Shader->SetFloat("u_AO", AO);
		m_Shader->SetFloat3("u_EmissionColor", EmissiveColor);
		m_Shader->SetFloat("u_EmissiveIntensity", EmissiveIntensity);

		m_Shader->SetBool("u_UseAlbedoMap", UseAlbedoMap);
		m_Shader->SetBool("u_UseMetallicMap", UseMetallicMap);
		m_Shader->SetBool("u_UseNormalMap", UseNormalMap);
		m_Shader->SetBool("u_UseRoughnessMap", UseRoughnessMap);
		m_Shader->SetBool("u_UseOcclusionMap", UseOcclusionMap);
		m_Shader->SetBool("u_UseEmissiveMap", UseEmissiveMap);

		m_Shader->SetInt("u_IrradianceMap", 0);
		
		m_Shader->SetInt("u_AlbedoMap", 1);
		m_Shader->SetInt("u_MetallicMap", 2);
		m_Shader->SetInt("u_NormalMap", 3);
		m_Shader->SetInt("u_RoughnessMap", 4);
		m_Shader->SetInt("u_AmbientOcclusionMap", 5);
		m_Shader->SetInt("u_EmissiveMap", 6);

		if (AlbedoMap)
			AlbedoMap->Bind(1);

		if (MetallicMap)
			MetallicMap->Bind(2);

		if (NormalMap)
			NormalMap->Bind(3);

		if (RoughnessMap)
			RoughnessMap->Bind(4);

		if (AmbientOcclusionMap)
			AmbientOcclusionMap->Bind(5);

		if (EmissiveMap)
			EmissiveMap->Bind(6);
	}
}
