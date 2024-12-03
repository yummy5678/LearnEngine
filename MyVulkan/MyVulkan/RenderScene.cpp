#include "RenderScene.h"

RenderScene::RenderScene()
{
}

RenderScene::~RenderScene()
{
}

void RenderScene::Initialize(VmaAllocator* allocator)
{
	m_Allocator = allocator;

	MeshLoder meshLoader;	
	std::shared_ptr<VMeshObject> meshResult = meshLoader.Load(allocator, "");
	m_Object.SetMesh(allocator, meshResult);
	

}

void RenderScene::Update()
{
	float now = (float)glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	angle += 10.0f * deltaTime;
	if (angle > 360.0f) { angle -= 360.0f; }

	glm::mat4 testMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	testMat = glm::rotate(testMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_Object.SetTransform(testMat);

	//m_Camera.UpdateBuffer(m_Allocator);

}

std::vector<RenderObject> RenderScene::GetObjects()
{
	return { m_Object };
}

SceneCamera RenderScene::GetMainCamera()
{
	return m_Camera;
}


