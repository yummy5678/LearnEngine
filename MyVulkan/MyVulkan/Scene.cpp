#include "Scene.h"

void Scene::Initialize()
{
	MeshManager& meshManager = MeshManager::getInstance();
	meshManager.Load("");
	m_Object.SetMesh(meshManager.GetMesh(""));
	

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
	m_Object.SetTransform(testMat);

	m_MainCamera.UpdateVBuffer();

}

std::vector<SceneObject> Scene::GetObjects()
{
	return { m_Object };
}

SceneCamera Scene::GetMainCamera()
{
	return m_Camera;
}

