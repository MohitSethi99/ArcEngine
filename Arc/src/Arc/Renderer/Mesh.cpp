#include "arcpch.h"
#include "Mesh.h"

#include "Arc/Renderer/Texture.h"
#include "Arc/Renderer/Material.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace ArcEngine
{
	std::vector<Ref<Texture2D>> s_TextureCache;

	Mesh::Mesh(int entityID, const std::string& filepath)
	{
		LoadModel(entityID, filepath);
	}

	void Mesh::LoadModel(int entityID, const std::string& path)
	{
		Assimp::Importer importer;

		const uint32_t meshImportFlags = aiProcess_Triangulate
	        | aiProcess_GenUVCoords
	        | aiProcess_GenNormals
	        | aiProcess_OptimizeMeshes
	        | aiProcess_ValidateDataStructure;

		const aiScene* scene = importer.ReadFile(path, meshImportFlags);

		if (!scene || !scene->HasMeshes())
		{
			ARC_CORE_ERROR("Assimp::{0}", importer.GetErrorString());
			return;
		}

		m_Directory = path.substr(0, path.find_last_of("\\"));
		m_Name = path.substr(path.find_last_of("\\") + 1);
		ProcessNode(entityID, scene->mRootNode, scene);
	}

	void Mesh::ProcessNode(int entityID, aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Submeshes.push_back(LoadSubmesh(entityID, mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
			ProcessNode(entityID, node->mChildren[i], scene);
	}

	Submesh Mesh::LoadSubmesh(int entityID, aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Ref<Texture2D>> textures;
		AABB aabb;
		aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
		aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;

			glm::vec3 tempVector;
			tempVector.x = mesh->mVertices[i].x;
			tempVector.y = mesh->mVertices[i].y;
			tempVector.z = mesh->mVertices[i].z;
			vertex.Position = tempVector;

			aabb.Min.x = glm::min(vertex.Position.x, aabb.Min.x);
			aabb.Min.y = glm::min(vertex.Position.y, aabb.Min.y);
			aabb.Min.z = glm::min(vertex.Position.z, aabb.Min.z);
			aabb.Max.x = glm::max(vertex.Position.x, aabb.Max.x);
			aabb.Max.y = glm::max(vertex.Position.y, aabb.Max.y);
			aabb.Max.z = glm::max(vertex.Position.z, aabb.Max.z);

			tempVector.x = mesh->mNormals[i].x;
			tempVector.y = mesh->mNormals[i].y;
			tempVector.z = mesh->mNormals[i].z;
			vertex.Normal = tempVector;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 tex;
				tex.x = mesh->mTextureCoords[0][i].x;
				tex.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = tex;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertex.ObjectID = entityID;

			vertices.push_back(vertex);
		}

		for (size_t i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// TODO default to pbr as the standard material
		Ref<MaterialInstance> material = MaterialInstance::Create(MaterialInstance::Type::PBR);
		material->Name = mesh->mName.C_Str();
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* meshMaterial = scene->mMaterials[mesh->mMaterialIndex];

			// TODO check what type of material it is, to assign the proper material instance
			// Default to use pbr material for now;
			auto mat = std::dynamic_pointer_cast<PbrMaterial>(material);

			{ // Diffuse
				std::vector<Ref<Texture2D>> textures =
				LoadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE);
				if (textures.size() > 0)
				{
					mat->AlbedoMap = textures.at(0);
					mat->UseAlbedoMap = true;
				}
			}

			{ // Emmissive
				std::vector<Ref<Texture2D>> textures =
				LoadMaterialTextures(meshMaterial, aiTextureType_EMISSIVE);
				if (textures.size() > 0)
				{
					mat->EmissiveMap = textures.at(0);
					mat->UseEmissiveMap = true;
				}
			}

			{ // Lightmap (Ambient Occlusion)
				std::vector<Ref<Texture2D>> textures =
				LoadMaterialTextures(meshMaterial, aiTextureType_LIGHTMAP);
				if (textures.size() > 0)
				{
					mat->AmbientOcclusionMap = textures.at(0);
					mat->UseOcclusionMap = true;
				}
			}

			{ // Normal
				std::vector<Ref<Texture2D>> textures =
				LoadMaterialTextures(meshMaterial, aiTextureType_NORMALS);
				if (textures.size() > 0)
				{
					mat->NormalMap = textures.at(0);
					mat->UseNormalMap = true;
				}
			}

			{ // Reflection
				std::vector<Ref<Texture2D>> textures =
				LoadMaterialTextures(meshMaterial, aiTextureType_REFLECTION);
				if (textures.size() > 0)
				{
				}
			}

			{ // Specular
				std::vector<Ref<Texture2D>> textures =
				LoadMaterialTextures(meshMaterial, aiTextureType_SPECULAR);
			}
		}
		else
		{
			ARC_CORE_WARN("No Textures associated with {0}", m_Name);
		}

		m_Materials.emplace_back(material);
		Submesh submesh(vertices, indices, textures);
		submesh.BoundingBox = aabb;

		return submesh;
	}

	std::vector<Ref<Texture2D>> Mesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
	{
		std::vector<Ref<Texture2D>> textures;

		Ref<Texture2D> texture;
		for (size_t i = 0; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string path(m_Directory + "\\" + str.C_Str());

			bool inCache = false;
			for (auto tex : s_TextureCache)
			{
				if (path.compare(tex->GetName()) == 0)
				{
					inCache = true;
					texture = tex;
					break;
				}
			}
			if (!inCache)
			{
				ARC_CORE_INFO("Loading Texture from {0}", path);
				texture = Texture2D::Create(path);
				s_TextureCache.push_back(texture);
			}

			textures.push_back(texture);
		}

		return textures;
	}

}
