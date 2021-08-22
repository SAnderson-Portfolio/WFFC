#include "MousePicking.h"

bool MousePicking::PickDisplayObjects(DisplayObject& objectPicked, std::vector<DisplayObject>& objectVector, float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool returnOnFirstHit)
{
	float pickedDistance = 0;
	float closestDistance = INFINITE;
	//Setup near and far planes of frustum with mouse X and mouse Y pass down from ToolMain
	const XMVECTOR nearSource = XMVectorSet(mouseX, mouseY, 0.0f, 1.0f);
	const XMVECTOR farSource = XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);
	XMVECTOR pickingVector;
	XMVECTOR nearPoint;
	for (DisplayObject object : objectVector)
	{
		//Set the world matrix for selected object
		XMMATRIX local = world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, DAUtils::GetScale(object), g_XMZero, DAUtils::GetRotation(object), DAUtils::GetTranslation(object));

		//Unproject the points on the near and far plane with respect to the world matrix.
		nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth, projection, view, local);

		XMVECTOR farPoint = XMVector3Unproject(farSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth, projection, view, local);

		pickingVector = farPoint - nearPoint;
		pickingVector = XMVector3Normalize(pickingVector);

		for (int j = 0; j < object.m_model.get()->meshes.size(); j++)
		{
			if (object.m_model.get()->meshes[j]->boundingBox.Intersects(nearPoint, pickingVector, pickedDistance))
			{
				if (pickedDistance < closestDistance)
				{
					closestDistance = pickedDistance;
					objectPicked = object;
					if (returnOnFirstHit)
						return true;
				}
			}
		}
	}

	if (closestDistance != INFINITE)
		return true;

	return false;
}

float MousePicking::PickDisplayObjectDistance(DisplayObject& objectPicked, std::vector<DisplayObject>& objectVector, float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth)
{
	float pickedDistance = 0;
	float closestDistance = INFINITE;
	//Setup near and far planes of frustum with mouse X and mouse Y pass down from ToolMain
	const XMVECTOR nearSource = XMVectorSet(mouseX, mouseY, 0.0f, 1.0f);
	const XMVECTOR farSource = XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);
	XMVECTOR pickingVector = GetPickingVector(mouseX, mouseY, world, projection, view, screenDimension, minDepth, maxDepth);
	XMVECTOR nearPoint;
	for (DisplayObject object : objectVector)
	{
		//Set the world matrix for selected object
		XMMATRIX local = world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, DAUtils::GetScale(object), g_XMZero, DAUtils::GetRotation(object), DAUtils::GetTranslation(object));

		//Unproject the points on the near and far plane with respect to the world matrix.
		nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth, projection, view, local);

		XMVECTOR farPoint = XMVector3Unproject(farSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth, projection, view, local);

		pickingVector = farPoint - nearPoint;
		pickingVector = XMVector3Normalize(pickingVector);

		for (int j = 0; j < object.m_model.get()->meshes.size(); j++)
		{
			if (object.m_model.get()->meshes[j]->boundingBox.Intersects(nearPoint, pickingVector, pickedDistance))
			{
				if (pickedDistance < closestDistance)
				{
					closestDistance = pickedDistance;
					objectPicked = object;
				}
			}
		}
	}

	return closestDistance;
}

int MousePicking::PickObjectID(std::vector<DisplayObject>& objectVector, float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool returnOnFirstHit)
{
	POINT mPos;
	mPos.x = mouseX;
	mPos.y = mouseY;
	ClientToScreen(GetActiveWindow(), &mPos);

	int selectedID = -1;
	float pickedDistance = 0;
	float closestDistance = 0;
	//Setup near and far planes of frustum with mouse X and mouse Y pass down from ToolMain
	const XMVECTOR nearSource = XMVectorSet(mouseX, mouseY, 0.0f, 1.0f);
	const XMVECTOR farSource = XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);
	XMVECTOR pickingVector;
	XMVECTOR nearPoint;
	for (int i = 0; i < objectVector.size(); i++)
	{

		XMMATRIX local = world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, DAUtils::GetScale(objectVector[i]), g_XMZero, DAUtils::GetRotation(objectVector[i]), DAUtils::GetTranslation(objectVector[i]));

		//Unproject the points on the near and far plane with respect to the world matrix.
		nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth,
			projection, view, local);

		XMVECTOR farPoint = XMVector3Unproject(farSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth,
			projection, view, local);

		pickingVector = farPoint - nearPoint;
		pickingVector = XMVector3Normalize(pickingVector);
		
		for (int j = 0; j < objectVector[i].m_model.get()->meshes.size(); j++)
		{
			if (objectVector[i].m_model.get()->meshes[j]->boundingBox.Intersects(nearPoint, pickingVector, pickedDistance))
			{
				if (selectedID == -1)
				{
					closestDistance = pickedDistance;
					selectedID = i;
					if (returnOnFirstHit)
						return selectedID;
				}

				if (pickedDistance < closestDistance)
				{
					selectedID = i;
					closestDistance = pickedDistance;
				}
			}
		}

		
	}

	return selectedID;
}


bool MousePicking::PickSingleObject(DisplayObject& objectToPick, float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, float& distance)
{
	XMMATRIX local = world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, DAUtils::GetScale(objectToPick), g_XMZero, DAUtils::GetRotation(objectToPick), DAUtils::GetTranslation(objectToPick));
	XMVECTOR pickingVector = GetPickingVector(mouseX, mouseY, local, projection, view, screenDimension, minDepth, maxDepth);
	

	float pickedDistance = 0;
	//Setup near and far planes of frustum with mouse X and mouse Y pass down from ToolMain
	const XMVECTOR nearSource = XMVectorSet(mouseX, mouseY, 0.0f, 1.0f);
	XMVECTOR nearPoint;

	//Unproject the points on the near and far plane with respect to the world matrix.
	nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth,
		projection, view, local);


	for (int j = 0; j < objectToPick.m_model.get()->meshes.size(); j++)
	{
		if (objectToPick.m_model.get()->meshes[j]->boundingBox.Intersects(nearPoint, pickingVector, pickedDistance))
		{
			if (pickedDistance < distance)
			{
				distance = pickedDistance;
				return true;
			}
		}
	}

	return false;
}

bool MousePicking::RayIntersectsTriangle(DisplayChunk& displayChunk, DirectX::SimpleMath::Vector3 rayOrigin, DirectX::SimpleMath::Vector3 rayVector, DirectX::SimpleMath::Vector3 & outIntersectionPoint, int i, int j)
{
	//Möller–Trumbore intersection algorithm
	const float EPSILON = 1e-5f;
	bool vertexFound = true;
	DirectX::SimpleMath::Vector3 vertex0 = displayChunk.GetTerrainGeometryAtIndex(i, j, vertexFound).position;
	if (!vertexFound)//Check if the vertex actually exists before continuing.
		return false;

	DirectX::SimpleMath::Vector3 vertex1 = displayChunk.GetTerrainGeometryAtIndex(i, j + 1, vertexFound).position;
	if (!vertexFound)
		return false;

	DirectX::SimpleMath::Vector3 vertex2 = displayChunk.GetTerrainGeometryAtIndex(i + 1, j, vertexFound).position;
	if (!vertexFound)
		return false;

	DirectX::SimpleMath::Vector3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = rayVector.Cross(edge2);

	a = edge1.Dot(h);

	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.

	f = 1.0f / a;
	s = rayOrigin - vertex0;
	u = f * s.Dot(h);
	if (u < 0.0f || u > 1.0f)
		return false;
	q = s.Cross(edge1);
	v = f * rayVector.Dot(q);
	if (v < 0.0f || u + v > 1.0f)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * edge2.Dot(q);
	if (t > EPSILON) // ray intersection
	{
		outIntersectionPoint = rayOrigin + rayVector * t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
}

void MousePicking::CheckForTriangleIntersection(DisplayChunk& displayChunk, DirectX::SimpleMath::Vector3 rayOrigin, float mouseX, float mouseY,
	Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, DirectX::SimpleMath::Vector3& outIntersectionPoint)
{
	DirectX::SimpleMath::Vector3 closestPoint = DirectX::SimpleMath::Vector3::Zero;
	XMVECTOR pickingVector = GetPickingVector(mouseX, mouseY, world, projection, view, screenDimension, minDepth, maxDepth);
	float closestDistance = INFINITE;
	int xPoint = 0, yPoint = 0;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			if (RayIntersectsTriangle(displayChunk, rayOrigin, pickingVector, outIntersectionPoint, i, j))
			{
				float currentDist = DirectX::SimpleMath::Vector3::Distance(outIntersectionPoint, rayOrigin);
				if (currentDist < closestDistance)
				{
					closestDistance = currentDist;
					closestPoint = outIntersectionPoint;
					xPoint = i;
					yPoint = j;
				}
			}
		}
	}

}

void MousePicking::CheckForTriangleIntersection(DisplayChunk& displayChunk, Vector3 rayOrigin, Vector3 pickingRay, POINT& intersectedPoint)
{
	Vector3 closestPoint = Vector3::Zero;
	float closestDistance = INFINITE;
	bool intersected = false;
	int xPoint = 0, yPoint = 0;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			if (RayIntersectsTriangle(displayChunk, rayOrigin, pickingRay, closestPoint, i, j))
			{
				float currentDist = Vector3::Distance(closestPoint, rayOrigin);
				if (currentDist < closestDistance)
				{
					closestDistance = currentDist;
					intersectedPoint.x = i;
					intersectedPoint.y = j;
				}
			}
		}
	}
}

XMVECTOR MousePicking::GetPickingVector(float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth)
{
	POINT mPos;
	mPos.x = mouseX;
	mPos.y = mouseY;
	ClientToScreen(GetActiveWindow(), &mPos);

	//Setup near and far planes of frustum with mouse X and mouse Y pass down from ToolMain
	const XMVECTOR nearSource = XMVectorSet(mouseX, mouseY, 0.0f, 1.0f);
	const XMVECTOR farSource = XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);

	//Unproject the points on the near and far plane with respect to the world matrix.
	XMVECTOR nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth,
		projection, view, world);

	XMVECTOR farPoint = XMVector3Unproject(farSource, 0.0f, 0.0f, screenDimension.right, screenDimension.bottom, minDepth, maxDepth,
		projection, view, world);

	return XMVector3Normalize(farPoint - nearPoint);
}

void MousePicking::SculptTerrain(DisplayChunk & displayChunk, Vector3 rayOrigin, float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool isLMB)
{
	POINT terrainPoint{ -1, -1 };
	XMVECTOR pickingVector = GetPickingVector(mouseX, mouseY, world, projection, view, screenDimension, minDepth, maxDepth);
	CheckForTriangleIntersection(displayChunk, rayOrigin, pickingVector, terrainPoint);
	if (terrainPoint.x != -1 && terrainPoint.y != -1)
	{
		if(isLMB)
			displayChunk.SculptTerrain(terrainPoint.x, terrainPoint.y);
		else
			displayChunk.SculptTerrain(terrainPoint.x, terrainPoint.y, -1.0f);
	}
}

void MousePicking::FlattenTerrain(DisplayChunk & displayChunk, Vector3 rayOrigin, float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth)
{
	POINT terrainPoint{ -1, -1 };
	XMVECTOR pickingVector = GetPickingVector(mouseX, mouseY, world, projection, view, screenDimension, minDepth, maxDepth);
	CheckForTriangleIntersection(displayChunk, rayOrigin, pickingVector, terrainPoint);
	if (terrainPoint.x != -1 && terrainPoint.y != -1)
	{
		displayChunk.FlattenHeightmap(terrainPoint.x, terrainPoint.y);
	}
}

void MousePicking::ScaleTerrain(DisplayChunk & displayChunk, Vector3 rayOrigin, float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool isLMB)
{
	POINT terrainPoint{ -1, -1 };
	XMVECTOR pickingVector = GetPickingVector(mouseX, mouseY, world, projection, view, screenDimension, minDepth, maxDepth);
	CheckForTriangleIntersection(displayChunk, rayOrigin, pickingVector, terrainPoint);
	if (terrainPoint.x != -1 && terrainPoint.y != -1)
	{
		if(isLMB)
			displayChunk.ScaleTerrain(terrainPoint.x, terrainPoint.y);
		else
			displayChunk.ScaleTerrain(terrainPoint.x, terrainPoint.y, -1.0f);
	}
}

void MousePicking::PaintTerrain(DisplayChunk & displayChunk, Vector3 rayOrigin, float mouseX, float mouseY, Matrix world, Matrix projection, Matrix view, RECT screenDimension, FLOAT minDepth, FLOAT maxDepth, bool isLMB)
{
	POINT terrainPoint{ -1, -1 };
	XMVECTOR pickingVector = GetPickingVector(mouseX, mouseY, world, projection, view, screenDimension, minDepth, maxDepth);
	CheckForTriangleIntersection(displayChunk, rayOrigin, pickingVector, terrainPoint);
	if (terrainPoint.x != -1 && terrainPoint.y != -1)
	{
		if (isLMB)
			displayChunk.PaintTexture(terrainPoint.x, terrainPoint.y, true);
		else
			displayChunk.PaintTexture(terrainPoint.x, terrainPoint.y, false);
	}
}
