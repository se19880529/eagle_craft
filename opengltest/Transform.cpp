#include "stdafx.h"
#include "Transform.h"


inline const ELVector& Transform::GetPosition() const
{
	return position;
}
inline const ELVector& Transform::GetRotation() const
{
	return rotation;
}
inline const ELVector& Transform::GetScale() const
{
	return scale;
}
inline const ELMatrix4x4 GetMatrix() const
{
	return localMatrix;
}

void Transform::_RefreshMatrix()
{
	localDirty = false;
	Quaternion q;
	q.MakeEulerRotation(rotation.x, rotation.y, rotation.z);
	localMatrix = q.MakeRotateMatrix();
	for(int j = 0; j < 3; j++)
		localMatrix.elems[j][0]*=scale.x;
	for(int j = 0; j < 3; j++)
		localMatrix.elems[j][0]*=scale.y;
	for(int j = 0; j < 3; j++)
		localMatrix.elems[j][0]*=scale.z;
	localMatrix.elems[0][3] = position.x;
	localMatrix.elems[1][3] = position.y;
	localMatrix.elems[2][3] = position.z;
}

void Transform::SetLocalMatrix(const ELMatrix4x4& matrix)
{
	_SetLocalDirty();
	localDirty = false;
	localMatrix = matrix;
	position.x = localMatrix.elems[0][3];
	position.y = localMatrix.elems[1][3];
	position.z = localMatrix.elems[2][3];
	scale.x = sqrt(localMatrix.elems[0][0]*localMatrix.elems[0][0] + localMatrix.elems[0][1]*localMatrix.elems[0][1] + localMatrix.elems[0][2]*localMatrix.elems[0][2]);
	scale.y = sqrt(localMatrix.elems[1][0]*localMatrix.elems[1][0] + localMatrix.elems[1][1]*localMatrix.elems[1][1] + localMatrix.elems[1][2]*localMatrix.elems[1][2]);
	scale.z = sqrt(localMatrix.elems[2][0]*localMatrix.elems[2][0] + localMatrix.elems[2][1]*localMatrix.elems[2][1] + localMatrix.elems[2][2]*localMatrix.elems[2][2]);
	
	if(abs(localMatrix.elems[0][0]) < MIN_POSITIVE)
		rotation.x = (localMatrix.elems[0][0] * localMatrix.elems[1][0]>0):PI/2:-PI/2;
	else
		rotation.x = atan( localMatrix.elems[1][0]/localMatrix.elems[0][0] );

	if(abs(localMatrix.elems[2][2]) < MIN_POSITIVE)
		rotation.y = (localMatrix.elems[2][1] * localMatrix.elems[2][2]>0):PI/2:-PI/2;
	else
		rotation.y = atan( localMatrix.elems[2][1]/localMatrix.elems[2][2] );
	if(abs(localMatrix.elems[2][2]) + abs(localMatrix.elems[2][1]) < MIN_POSITIVE)
		rotation.z = (localMatrix.elems[2][0]>0):-PI/2:PI/2;
	else
		rotation.z = atan( -localMatrix.elems[2][0]/ sqrt( localMatrix.elems[2][1]*localMatrix.elems[2][1]+localMatrix.elems[2][2]*localMatrix.elems[2][2]) );
}
const Matrix& Transform::GetLocalMatrix() const
{
	if(localDirty)
		_RefreshMatrix();
	return localMatrix;
}
const Matrix& Transform::GetWorldMatrix() const
{
	if(worldDirty)
	{
		worldDirty = false;
		worldMatrix = GetLocalMatrix();
		if(parent != NULL)
			worldMatrix = worldMatrix.RightMultiply(parent.GetWorldMatrix());
	}
	return worldMatrix;
}
void Transform::SetPosition(const ELVector& pos)
{
	_SetLocalDirty();
	position = pos;
}
void Transform::SetScale(const ELVector& _scale)
{
	_SetLocalDirty();
	scale = _scale;
}
void Transform::SetRotation(const ELVector& rot)
{
	_SetLocalDirty();
	rotation = rot;
}
inline const Transform& GetParent() const
{
	return *parent;
}
void SetParent(Transform& p)
{
	std::vector<Transform*>::iterator iter = p.childs.begin();
	while(iter != p.childs.end())
		if(*iter == this)
			return;
		else
			iter++;
	p.childs.push_back(this);
	parent = &p;
	_SetWorldDirty();
}
void Transform::_SetWorldDirty()
{
	worldDirty = true;
	if(attachedObject != NULL)
		attachedObject->OnTransformDirty(true);
	std::vector<Transform*>::iterator iter = childs.begin();
	while(iter != childs.end())
	{
		(*iter)->_SetWorldDirty();
		iter++;
	}
}
void Transform::_SetLocalDirty()
{
	localDirty = true;
	worldDirty = true;
	if(attachedObject != NULL)
		attachedObject->OnTransformDirty(false);
	
	std::vector<Transform*>::iterator iter = childs.begin();
	while(iter != childs.end())
	{
		(*iter)->_SetWorldDirty();
		iter++;
	}
}