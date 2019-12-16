#include "OpenGLMesh.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "Engine.h"
#include "Camera.h"
#include "OpenGLDevice.h"
#include "MeshAsset.h"

#include <GL/gl3w.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>



namespace Solus
{

	OpenGLMeshEntry::OpenGLMeshEntry()
	{
		MaterialIndex = -1;
		normalBuffer = -1;
		uvBuffer = -1;
		vertexBuffer = -1;
		indexBuffer = -1;
	}

	OpenGLMeshEntry::~OpenGLMeshEntry()
	{
		if (vertexBuffer >= 0)
		{
			glDeleteBuffers(1, &vertexBuffer);
		}
		if (indexBuffer >= 0)
		{
			glDeleteBuffers(1, &indexBuffer);
		}
		if (normalBuffer >= 0)
		{
			glDeleteBuffers(1, &normalBuffer);
		}
		if (uvBuffer >= 0)
		{
			glDeleteBuffers(1, &uvBuffer);
		}
	}

	void OpenGLMeshEntry::GenerateBuffers()
	{
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * normals.size(), &normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2f) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	}

	OpenGLMesh::OpenGLMesh()
	{
		mvpUniformLoc = glGetUniformLocation(shader->GetShaderProgram(), "mvpMatrix");
		assert(mvpUniformLoc != 0xFFFFFFFF);
		viewMatrixUniformLoc = glGetUniformLocation(shader->GetShaderProgram(), "viewMatrix");
		//assert(viewMatrixUniformLoc != 0xFFFFFFFF);
		modelMatrixUniformLoc = glGetUniformLocation(shader->GetShaderProgram(), "modelMatrix");
		//assert(modelMatrixUniformLoc != 0xFFFFFFFF);
		wsLightPositionUniformLoc = glGetUniformLocation(shader->GetShaderProgram(), "wsLightPosition");
		//assert(wsLightPositionUniformLoc != 0xFFFFFFFF);

		//modelViewMatrixUniformLoc = glGetUniformLocation(shader->GetShaderProgram(), "modelViewMatrix");
		//assert(modelViewMatrixUniformLoc != 0xFFFFFFFF);
		textureSamplerLoc = glGetUniformLocation(shader->GetShaderProgram(), "textureSampler");
		//assert(textureSamplerLoc != 0xFFFFFFFF);

		glGenVertexArrays(1, &VAO);

		CHECK_OPENGL_ERROR();
	}

	OpenGLMesh::~OpenGLMesh()
	{
		glDeleteVertexArrays(1, &VAO);
	}

	void OpenGLMesh::Render()
	{
		glUseProgram(GetOpenGLShader()->GetShaderProgram());
		Mat4f projection = *gEngine->GetMainCamera()->GetProjectionMatrix();
		Mat4f view = gEngine->GetMainCamera()->GetTransform();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), owner->GetPosition());
		glm::mat4 mvp = projection * view * model;

		glUniformMatrix4fv(mvpUniformLoc, 1, GL_FALSE, &mvp[0][0]);

		glUniformMatrix4fv(viewMatrixUniformLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(modelMatrixUniformLoc, 1, GL_FALSE, &model[0][0]);
		Vec3f lightPosition = Vec3f(4, 4, 4);
		glUniform3f(wsLightPositionUniformLoc, lightPosition.x, lightPosition.y, lightPosition.z);


		for (int i = 0; i < 1/*textures.size()*/; i++)
		{
			textures[i]->Bind(i);
			glUniform1i(textureSamplerLoc, i);
		}

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, entries[0].vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, entries[0].normalBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, entries[0].uvBuffer);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[0].indexBuffer);

		glDrawElements(GL_TRIANGLES, entries[0].indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		CHECK_OPENGL_ERROR();
	}

	bool OpenGLMesh::Load(MeshAsset* meshAsset)
	{
		if (isLoaded)
		{
			return false;
		}
		Assimp::Importer importer;
		const char* path = "";
		const aiScene* scene = importer.ReadFile(meshAsset->GetFilePath().string(), aiProcess_FixInfacingNormals
												 | aiProcess_JoinIdenticalVertices
												 | aiProcess_GenSmoothNormals
												 | aiProcess_FindInvalidData
												 | aiProcess_GenUVCoords
												 | aiProcess_Triangulate);
		if (!scene)
		{
			gEngine->Log(LogError, "Could not load OBJ model file: %s", importer.GetErrorString());
			return false;
		}
		isLoaded = true;

		entries.resize(scene->mNumMeshes);
		textures.resize(scene->mNumTextures);

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			LoadMesh(i, scene->mMeshes[i]);
		}

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];
			bool foundMaterial = false;
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString path;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
				{
					std::string fullPath = ".";
					fullPath += path.C_Str();
					auto* texture = (TextureAsset*)gEngine->GetAssetManager()->GetAsset(fullPath.c_str());
					textures[i] = gEngine->GetRenderDevice()->CreateTexture(texture);
					bool foundMaterial = textures[i] != nullptr;
				}
			}
			if (!foundMaterial)
			{
				textures.push_back(gEngine->GetRenderDevice()->GetDefaultTexture());
			}
		}
		return true;
	}

	void OpenGLMesh::LoadMesh(int index, aiMesh* mesh)
	{
		entries[index].MaterialIndex = mesh->mMaterialIndex;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D& vertex = mesh->mVertices[i];
			aiVector3D& normal = mesh->mNormals[i];
			aiVector3D texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D{ 0.0f, 0.0f, 0.0f };

			entries[index].vertices.push_back(Vec3f(vertex.x, vertex.y, vertex.z));
			entries[index].normals.push_back(Vec3f(normal.x, normal.y, normal.z));
			entries[index].texCoords.push_back(Vec2f(texCoord.x, 1.0f - texCoord.y));
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			assert(face.mNumIndices == 3);
			for (unsigned int f = 0; f < face.mNumIndices; f++)
			{
				entries[index].indices.push_back(face.mIndices[f]);
			}
		}
		entries[index].GenerateBuffers();
	}

	OpenGLShader* OpenGLMesh::GetOpenGLShader() const
	{
		return reinterpret_cast<OpenGLShader*>(shader);
	}
}
