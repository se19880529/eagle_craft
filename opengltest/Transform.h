#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "GLTBase.h"
#include "GLObject.h"
#include <vector>

class Transform
{
public:
	Transform(GLObject* obj=NULL):attachedObject(obj), scale(1,1,1),parent(NULL),dirty(true),parentDirty(true), localMatrix(1){}

	const ELVector& GetPosition() const;
	const ELVector& GetRotation() const;
	const ELVector& GetScale() const;
	const Matrix& GetMatrix() const;
	const Matrix& GetLocalMatrix() const;
	const Matrix& GetWorldMatrix() const;
	const Transform& GetParent() const;

	void SetParent(Transform& parent) const;
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
