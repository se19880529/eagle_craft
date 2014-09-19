#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "GLTBase.h"
#include <vector>
class GLObject;
class Transform
{
public:
	Transform(GLObject* obj = NULL) :attachedObject(obj), scale(1, 1, 1), parent(NULL), localDirty(true), worldDirty(true), localMatrix(1){}

	const ELVector& GetPosition() const;
	const ELVector& GetRotation() const;
	const ELVector& GetScale() const;
	const Matrix& GetMatrix();
	const Matrix& GetLocalMatrix();
	const Matrix& GetWorldMatrix();
	const Transform& GetParent() const;

	void SetParent(Transform& parent);
	void SetLocalMatrix(const Matrix& matrix);
	void SetPosition(const ELVector& pos);
	void SetScale(const ELVector& scale);
	void SetRotation(const ELVector& rot);
	GLObject* GetGLObject(){ return attachedObject; };
protected:
	ELVector position;
	ELVector rotation;
	ELVector scale;
	Matrix localMatrix;
	Matrix worldMatrix;
	Transform* parent;
	std::vector<Transform*> childs;
	bool localDirty;
	bool worldDirty;
private:
	void _RefreshMatrix();
	void _SetWorldDirty();
	void _SetLocalDirty();
	GLObject* attachedObject;
};
#endif
