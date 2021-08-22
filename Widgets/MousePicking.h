#pragma once
#include <vector>
#include "DisplayObject.h"
#include "DisplayObjectUtils.h"
#include "TranslateWidget.h"
#include "RotateWidget.h"
#include "ScaleWidget.h"
#include "DisplayChunk.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;
namespace MousePicking
{
	///<summary>Tests to see if you have picked any DisplayObjects in a DisplayObject vector.</summary>
	/// <param name="objectPicked">Out object for the DisplayObject picked.</param>
	/// <param name="objectVector">Vector of DisplayObjects you want to pick.</param>
	/// <param name="mouseX">The X position of the mouse.</param>
	/// <param name="mouseY">The Y position of the mouse.</param>
	/// <param name="world">World matrix.</param>
	/// <param name="projection">Projection matrix.</param>
	/// <param name="view">View matrix/</param>
	/// <param name="screenDimension">The dimensions of the screen.</param>
	/// <param name="minDepth">The near depth.</param>
	/// <param name="maxDepth">The far depth.</param>
	/// <param name="returnOnFirstHit">If the function should return when first object is hit, may result in false collisions.</param>
	/// <returns>True if DisplayObject has been picked, else returns false.</returns>
	bool PickDisplayObjects(DisplayObject& objectPicked, std::vector<DisplayObject>& objectVector, float mouseX, float mouseY,
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool returnOnFirstHit = false);

	///<summary>Tests to see if you have picked any DisplayObjects in a DisplayObject vector.</summary>
	/// <param name="objectPicked">Out object for the DisplayObject picked.</param>
	/// <param name="objectVector">Vector of DisplayObjects you want to pick.</param>
	/// <param name="mouseX">The X position of the mouse.</param>
	/// <param name="mouseY">The Y position of the mouse.</param>
	/// <param name="world">World matrix.</param>
	/// <param name="projection">Projection matrix.</param>
	/// <param name="view">View matrix/</param>
	/// <param name="screenDimension">The dimensions of the screen.</param>
	/// <param name="minDepth">The near depth.</param>
	/// <param name="maxDepth">The far depth.</param>
	/// <param name="returnOnFirstHit">If the function should return when first object is hit, may result in false collisions.</param>
	/// <returns>Distance to object if picked else INFINITE</returns>
	float PickDisplayObjectDistance(DisplayObject& objectPicked, std::vector<DisplayObject>& objectVector, float mouseX, float mouseY,
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth);

	///<summary>Attempt to pick a DisplayObject and get the ID.</summary>
	/// <param name="objectVector">DisplayObject vector to be picked.</param>
	/// <param name="mouseX">The X position of the mouse.</param>
	/// <param name="mouseY">The Y position of the mouse.</param>
	/// <param name="world">World matrix.</param>
	/// <param name="projection">Projection matrix.</param>
	/// <param name="view">View matrix/</param>
	/// <param name="screenDimension">The dimensions of the screen.</param>
	/// <param name="minDepth">The near depth.</param>
	/// <param name="maxDepth">The far depth.</param>
	/// <param name="returnOnFirstHit">If the function should return when first object is hit, may result in false collisions.</param>
	/// <returns>Integer of the picked object's ID.</returns>
	int PickObjectID(std::vector<DisplayObject>& objectVector, float mouseX, float mouseY,
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool returnOnFirstHit = false);

	///<summary>Attempt to pick a single DisplayObject.</summary>
	/// <param name="objectToPick">DisplayObject to check for picking.</param>
	/// <param name="mouseX">The X position of the mouse.</param>
	/// <param name="mouseY">The Y position of the mouse.</param>
	/// <param name="world">World matrix.</param>
	/// <param name="projection">Projection matrix.</param>
	/// <param name="view">View matrix/</param>
	/// <param name="screenDimension">The dimensions of the screen.</param>
	/// <param name="minDepth">The near depth.</param>
	/// <param name="maxDepth">The far depth.</param>
	/// <param name="distance">Out float for storing the distance of the pick.</param>
	/// <returns>True if object picked, else false.</returns>
	bool PickSingleObject(DisplayObject& objectToPick, float mouseX, float mouseY,
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, float& distance);
	
	///<summary>Checks if a ray intersects a triangle in a given DisplayChunk.</summary>
	/// <param name="displayChunk">DisplayChunk to be picked.</param>
	/// <param name="rayOrigin">Starting point of the ray.</param>
	/// <param name="rayVector">Direction of the ray.</param>
	/// <param name="outIntersectionPoint">Out vector for the point being intersected.</param>
	/// <param name="i">The X index position to be checked.</param>
	/// <param name="j">The Y index position to be checked.</param>
	bool RayIntersectsTriangle(DisplayChunk& displayChunk, Vector3 rayOrigin, Vector3 rayVector, Vector3& outIntersectionPoint, int i, int j);

	///<summary></summary>
	/// <param name=""></param>
	void CheckForTriangleIntersection(DisplayChunk& displayChunk, Vector3 rayOrigin, float mouseX, float mouseY, 
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, Vector3& outIntersectionPoint);
	 
	///<summary></summary>
	/// <param name=""></param>
	void CheckForTriangleIntersection(DisplayChunk& displayChunk, Vector3 rayOrigin, Vector3 pickingRay, POINT& intersectedPoint);

	///<summary></summary>
	/// <param name=""></param>
	XMVECTOR GetPickingVector(float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth);

	///<summary></summary>
	/// <param name=""></param>
	void SculptTerrain(DisplayChunk& displayChunk, Vector3 rayOrigin, float mouseX, float mouseY,
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool isLMB);

	///<summary></summary>
	/// <param name=""></param>
	void FlattenTerrain(DisplayChunk& displayChunk, Vector3 rayOrigin, float mouseX, float mouseY,
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth);

	///<summary></summary>
	/// <param name=""></param>
	void ScaleTerrain(DisplayChunk& displayChunk, Vector3 rayOrigin, float mouseX, float mouseY,
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool isLMB);

	void PaintTerrain(DisplayChunk& displayChunk, Vector3 rayOrigin, float mouseX, float mouseY,
		Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool isLMB);
}

