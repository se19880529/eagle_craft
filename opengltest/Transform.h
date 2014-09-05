#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "GLTBase.h"
#include <vector>

class Transform
{
public:
	Transform():scale(1,1,1),parent(NULL),dirty(false), localMatrix(1){}

	const ELVector& GetPosition() const;
	const ELVector& GetRotation() const;
	const ELVector& GetScale() const;
	const ELMatrix4x4 GetMatrix() const;
	const Matrix& GetLocalMatrix() const;

	void SetLocalMatrix(const ELMatrix4x4& matrix);
	void SetPosition(const ELVector& pos);
	void SetScale(const ELVector& scale);
	void SetRotation(const ELVector& rot);
protected:
	ELVector position;
	ELVector rotation;
	ELVector scale;
	Matrix localMatrix;
	Transform* parent;
	std::vector<Transform*> childs;
	bool dirty;
private:
	void _RefreshMatrix();
	void _SetDirty();
};
#endif