#pragma once

#include "VertexArray.h"
#include "Arc/Utils/AABB.h"

#include <assimp/scene.h>

namespace ArcEngine
{
	class MaterialInstance;
	class Timestep;
	class VertexArray;
	class Texture2D;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
		int ObjectID;
	};

	struct Submesh
	{
		Submesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<Texture2D>> textures)
			: Vertices(vertices), Indices(indices), Textures(textures)
		{
			SubmeshVertexArray = VertexArray::Create();
			SubmeshVertexArray->Bind();

			Ref<VertexBuffer> buffer = VertexBuffer::Create((float*)Vertices.data(), vertices.size() * sizeof(Vertex));
			buffer->Bind();

			buffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Int, "a_ObjectID" },
			});

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(Indices.data(), Indices.size());

			SubmeshVertexArray->AddVertexBuffer(buffer);
			SubmeshVertexArray->SetIndexBuffer(indexBuffer);
		}

		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;

		glm::mat4 Transform;

		std::string NodeName, MeshName;
		
		Ref<VertexArray> SubmeshVertexArray;
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<Ref<Texture2D>> Textures;
		AABB BoundingBox;
	};
	
	class Mesh
	{
	public:
		Mesh(int entityID, const std::string& filepath);
		~Mesh() = default;
		
		std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
		Ref<MaterialInstance>& GetMaterialInstance(uint32_t index) { return m_Materials.at(index); }
		uint32_t GetMaterialsCount() const { return m_Materials.size(); }
		std::string& GetFilePath() { return m_Filepath; }
		std::string& GetName() { return m_Name; }

	private:
		void LoadModel(int entityID, const std::string& path);
		void ProcessNode(int entityID, aiNode* node, const aiScene* scene);
		Submesh LoadSubmesh(int entityID, aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* material, aiTextureType type);
	
	private:
		std::vector<Submesh> m_Submeshes;
		std::string m_Filepath;
		std::string m_Name;
		std::string m_Directory;
		std::vector<Ref<MaterialInstance>> m_Materials;
	};
}

