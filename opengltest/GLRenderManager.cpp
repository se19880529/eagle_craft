#include "GLRenderManager.h"
using namespace std;
void GLRenderManager::AddCamera(GLCamera* cam)
{
	if (cam == NULL)
		return;
	if (cameras.find(cam->GetID()) == cameras.end())
	{
		cameras.insert(map<GLObjectID, GLCamera*>::value_type(cam->GetID(), cam));
	}
}
void GLRenderManager::DelCamera(GLCamera* cam)
{
	if (cam != NULL)
	{
		cameras.erase(cam->GetID());
	}
}
void GLRenderManager::AddRenderObject(GLObject* obj)
{
	if (obj == NULL)
		return;
	if (objects.find(obj->GetID()) == objects.end())
	{
		objects.insert(map<GLObjectID, GLObject*>::value_type(obj->GetID(), obj));
	}
}
void GLRenderManager::DelRenderObject(GLObject* obj)
{
	if (obj == NULL)
		return;
	objects.erase(obj->GetID());
}
void GLRenderManager::Render()
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(-1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto elem : cameras)
	{
		elem.second->SetCurrent();
		for (auto obj : objects)
		{
			obj.second->Render();
		}
	}
}