#include "OpenGLMesh.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLDevice.h"
#include "OpenGLPrimitiveDrawer.h"

#include "AssetSystem/AssetManager.h"
#include "AssetSystem/MeshAsset.h"

#include "Engine/Engine.h"
#include "Camera.h"

#include <GL/gl3w.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace Solus
{

	OpenGLMeshData::~OpenGLMeshData()
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
		if (texCoordBuffer >= 0)
		{
			glDeleteBuffers(1, &texCoordBuffer);
		}
	}

	void OpenGLMeshData::GenerateBuffers(const MeshAsset& asset)
	{
		const MeshData& meshData = asset.GetFirstMesh();

		indicesCount = meshData.indices.size();

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * meshData.vertices.size(), &meshData.vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * meshData.normals.size(), &meshData.normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &texCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2f) * meshData.texCoords.size(), &meshData.texCoords[0], GL_STATIC_DRAW);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * meshData.indices.size(), &meshData.indices[0], GL_STATIC_DRAW);

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

		textures.push_back(gEngine->GetRenderDevice()->GetDefaultTexture());
	}

	OpenGLMesh::~OpenGLMesh()
	{
		glDeleteVertexArrays(1, &VAO);
		for (RenderTexture* texture : textures)
		{
			gEngine->GetRenderDevice()->DestroyTexture(texture);
		}
	}

	void OpenGLMesh::Render(const Entity* owner)
	{
		glUseProgram(GetOpenGLShader()->GetShaderProgram());
		Mat4f projection = *gEngine->GetMainCamera()->GetProjectionMatrix();
		Mat4f view = gEngine->GetMainCamera()->GetTransform();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), owner->GetPosition());
		model = glm::scale(model, owner->GetScale());
		const Vec3f rot = owner->GetRotation();
		model = glm::rotate(model, glm::radians(rot.x), Vec3f(1.0f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(rot.y), Vec3f(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(rot.z), Vec3f(0.f, 0.f, 1.f));
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

		glBindBuffer(GL_ARRAY_BUFFER, meshData.vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, meshData.normalBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, meshData.texCoordBuffer);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.indexBuffer);

		glDrawElements(GL_TRIANGLES, (GLsizei)meshData.indicesCount, GL_UNSIGNED_INT, nullptr);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		CHECK_OPENGL_ERROR();

		//auto* primDrawer = (OpenGLPrimitiveDrawer*)gEngine->GetRenderDevice()->GetPrimitiveDrawer();
		//Vec2f center = { 1280.f * 0.5f, 720.f * 0.5f };
		//Vec2f extents = { 100, 100 };
		//primDrawer->RenderRectangle(center, extents, Vec4f(1, 0, 0, 1));
		//
		//center = { 1280.f * 0.2f, 720.f * 0.4f };
		//extents = { 100, 50 };
		//primDrawer->RenderRectangle(center, extents, Vec4f(1, 0, 0, 1));
	}

	bool OpenGLMesh::Load(MeshAsset& meshAsset)
	{
		meshAsset.Load();
		meshData.GenerateBuffers(meshAsset);


		return true;
	}

	OpenGLShader* OpenGLMesh::GetOpenGLShader() const
	{
		return reinterpret_cast<OpenGLShader*>(shader);
	}
}
