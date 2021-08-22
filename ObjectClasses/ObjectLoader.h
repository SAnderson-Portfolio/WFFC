#pragma once
#include "SceneObject.h"
#include "DisplayObject.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

using Microsoft::WRL::ComPtr;
class ObjectLoader
{
public:
	ObjectLoader(std::vector<SceneObject>& sceneGraph, std::vector<DisplayObject>& displayList, ID3D11Device* device, DirectX::EffectFactory* fxFactory);
	~ObjectLoader();
	/// <summary>
	/// Adds a new object to the display list. Note this will not be saved.
	/// </summary>
	/// <param name="modelPath">The path to the model for the object to have.</param>
	/// <param name="objectName">The name of the object.</param>
	/// <param name="texturePath">The path to the texutre for the object.</param>
	/// <param name="position">The world position for the object to go.</param>
	/// <param name="rotation">Therotation for the object to have.</param>
	/// <param name="scale">The scale for the object to have.</param>
	/// <returns>The ID of the object that has been created.</returns>
	int AddObjectToDisplayList(const char* modelPath, const char* objectName, const char* texturePath, bool addToSceneGraph = true, Vector3 position = Vector3::Zero, Vector3 rotation = Vector3::Zero, Vector3 scale = Vector3::One);
	/// <summary>
	/// Adds a new object to the display list. Note this will not be saved.
	/// </summary>
	/// <param name="objectToAdd">Pointer to the SceneObject you wish to add.</param>
	HRESULT AddObjectToDisplayList(DisplayObject* objectToAdd);
	//Add a completly new obejct to the scene graph.
	HRESULT AddObjectToSceneGraph(const char* modelPath, const char* objectName, const char* texturePath, bool addToSceneGraph = true, Vector3 position = Vector3::Zero, Vector3 rotation = Vector3::Zero, Vector3 scale = Vector3::One);
	//Add an already created scene object to the scene graph.
	HRESULT AddObjectToSceneGraph(SceneObject* objectToAdd);
	//Duplicate a display object with the option of adding it to the scene graph to be saved.
	HRESULT DuplicateDisplayObject(DisplayObject* objectToDuplicate, bool addToSceneGraph = true);
	//Duplicate a scene object based off the ID.
	HRESULT DuplicateSceneObject(int objectID);
	//Duplicate a scene object based off an already created object.
	HRESULT DuplicateSceneObject(SceneObject* objectToDuplicate);
	//Get the display list from the object loader.
	std::vector<DisplayObject>* GetDisplayList() { return m_displayList; }
	//Create a brand new display object and fill an object with the created object.
	void CreateDisplayObject(DisplayObject& objectToFill, const char* modelPath, const char* objectName, const char* texturePath, bool addToSceneGraph = true, Vector3 position = Vector3::Zero, Vector3 rotation = Vector3::Zero, Vector3 scale = Vector3::One);
	//Create a brand new display object and add it to the scene graph.
	void CreateDisplayObjectAndAddToSceneGraph(const char* modelPath, const char* objectName, const char* texturePath, bool addToSceneGraph = true, Vector3 position = Vector3::Zero, Vector3 rotation = Vector3::Zero, Vector3 scale = Vector3::One);
	//Signal an update of the scene graph is required.
	bool UpdateRequired() { return m_displayListBuildRequired; }
	//Set the update required variable.
	void SetUpdateRequired(bool newUpdate) { m_displayListBuildRequired = newUpdate; }
private:
	//Pointer to the scene graph.
	std::vector<SceneObject>*    m_sceneGraph;
	//Pointer of the display list.
	std::vector<DisplayObject>*  m_displayList; 
	//Effect factor pointer.
	DirectX::EffectFactory* m_fxFactory;
	//Pointer to the D3D device.
	ID3D11Device* m_device;
private:
	SceneObject* GetSceneObject(const char* modelPath, const char* objectName, const char* texturePath, Vector3 position, Vector3 rotation, Vector3 scale);
	DisplayObject* GetDisplayObject(SceneObject* newObject);
	std::wstring StringToWCHART(std::string s);
	bool m_displayListBuildRequired;
};

