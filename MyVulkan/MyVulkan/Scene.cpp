#include "Scene.h"




void Scene::Initialize()
{
	MeshManager& meshManager = MeshManager::getInstance();
	meshManager.Load("");
	m_Object.mesh = meshManager.GetMesh("");
}

void Scene::Update()
{
	float now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	angle += 10.0f * deltaTime;
	if (angle > 360.0f) { angle -= 360.0f; }

	glm::mat4 testMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	testMat = glm::rotate(testMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_Object.transform = testMat;



}

void Scene::Render(RenderConfig& pipeline)
{
	pipeline.updateModel(m_Object.mesh, m_Object.transform);
}

