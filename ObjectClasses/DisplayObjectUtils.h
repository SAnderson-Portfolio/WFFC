#pragma once
#include "DisplayObject.h"
//Checks if i is a valid index for list.
#define VALID_INDEX(i, list) (i >= 0 && i < list.size())
#define VALID_POINTER_INDEX(i, list) (i >= 0 && i < list->size())
namespace DAUtils
{
	//Get the rotation of the DisplayObject passed in.
	const DirectX::XMVECTOR GetRotation(const DisplayObject& object);
	
	//Get the scale of the DisplayObject passed in.
	const DirectX::XMVECTORF32 GetScale(const DisplayObject& object);

	//Get the translation of the DisplayObject passed in.
	const DirectX::XMVECTORF32 GetTranslation(const DisplayObject& object);

	//Get the local matrix of a display object.
	const DirectX::XMMATRIX GetLocalMatrix(DirectX::SimpleMath::Matrix world, const DisplayObject& object);
}