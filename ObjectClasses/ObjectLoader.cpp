#include "ObjectLoader.h"

ObjectLoader::ObjectLoader(std::vector<SceneObject>& sceneGraph, std::vector<DisplayObject>& displayList, ID3D11Device* device, DirectX::EffectFactory* fxFactory)
{
	m_sceneGraph = &sceneGraph;
	m_displayList = &displayList;
	m_device = device;
	m_fxFactory = fxFactory;
	m_displayListBuildRequired = false;
}

ObjectLoader::~ObjectLoader()
{
}

int ObjectLoader::AddObjectToDisplayList(const char* modelPath, const char* objectName, const char* texturePath, bool addToSceneGraph, Vector3 position, Vector3 rotation, Vector3 scale)
{
	SceneObject scnObject = *GetSceneObject(modelPath, objectName, texturePath, position, rotation, scale);
	DisplayObject objectToPush = *GetDisplayObject(&scnObject);
	if (addToSceneGraph)
	{
		m_sceneGraph->push_back(scnObject);
		m_displayListBuildRequired = true;
	}
	else
	{
		//m_displayList->push_back(objectToPush);
		AddObjectToDisplayList(&objectToPush);
	}
	

	return objectToPush.m_ID;
}

HRESULT ObjectLoader::AddObjectToDisplayList(DisplayObject* objectToAdd)
{
	if (m_displayList)
	{
		m_displayList->push_back(*objectToAdd);
		return S_OK;
	}
	return E_POINTER;
}

HRESULT ObjectLoader::AddObjectToSceneGraph(const char* modelPath, const char* objectName, const char* texturePath, bool addToSceneGraph, Vector3 position, Vector3 rotation, Vector3 scale)
{
	if (m_sceneGraph)
	{
		SceneObject objectToPush = *GetSceneObject(modelPath, objectName, texturePath, position, rotation, scale);

		m_sceneGraph->push_back(objectToPush);
		m_displayListBuildRequired = true;
		return S_OK;
	}

	return E_POINTER;
}

HRESULT ObjectLoader::AddObjectToSceneGraph(SceneObject* objectToAdd)
{
	if (m_sceneGraph)
	{
		m_sceneGraph->push_back(*objectToAdd);
		m_displayListBuildRequired = true;
		return S_OK;
	}

	return E_POINTER;
}

HRESULT ObjectLoader::DuplicateDisplayObject(DisplayObject* objectToDuplicate, bool addToSceneGraph)
{
	if (m_displayList)
	{
		DisplayObject newObject = *objectToDuplicate;
		newObject.m_position.x -= 1.0f;
		AddObjectToDisplayList(&newObject);
		return S_OK;
	}
	return E_POINTER;
}

HRESULT ObjectLoader::DuplicateSceneObject(int objectID)
{
	if (m_displayList)
	{
		if (objectID < 0 || objectID >= m_sceneGraph->size())
			return E_ABORT;

		SceneObject newObject = m_sceneGraph->at(objectID);
		newObject.posX -= 1.0f;
		AddObjectToSceneGraph(&newObject);
		return S_OK;
	}
	return E_POINTER;
}

HRESULT ObjectLoader::DuplicateSceneObject(SceneObject* objectToDuplicate)
{
	if (m_sceneGraph)
	{
		SceneObject newObject = *objectToDuplicate;
		newObject.posY += 1.0f;
		AddObjectToSceneGraph(&newObject);
		//m_sceneGraph->push_back(newObject);
		m_displayListBuildRequired = true;
		return S_OK;
	}

	return E_POINTER;
}

void ObjectLoader::CreateDisplayObject(DisplayObject& objectToFill, const char * modelPath, const char * objectName, const char * texturePath, bool addToSceneGraph, Vector3 position, Vector3 rotation, Vector3 scale)
{
	SceneObject scnObject = *GetSceneObject(modelPath, objectName, texturePath, position, rotation, scale);
	if (addToSceneGraph)
	{
		if (m_sceneGraph)
			m_sceneGraph->push_back(scnObject);
		m_displayListBuildRequired = true;
	}
	objectToFill = *GetDisplayObject(&scnObject);
}

void ObjectLoader::CreateDisplayObjectAndAddToSceneGraph(const char * modelPath, const char * objectName, const char * texturePath, bool addToSceneGraph, Vector3 position, Vector3 rotation, Vector3 scale)
{
	AddObjectToSceneGraph(GetSceneObject(modelPath, objectName, texturePath, position, rotation, scale));
	m_displayListBuildRequired = true;
}


SceneObject* ObjectLoader::GetSceneObject(const char* modelPath, const char* objectName, const char* texturePath, Vector3 position, Vector3 rotation, Vector3 scale)
{
	SceneObject* newSceneObject = new SceneObject();
	newSceneObject->ID = m_sceneGraph->size();
	newSceneObject->chunk_ID = 0;
	newSceneObject->model_path = modelPath;
	newSceneObject->tex_diffuse_path = texturePath;
	newSceneObject->posX = position.x;
	newSceneObject->posY = position.y;
	newSceneObject->posZ = position.z;
	newSceneObject->rotX = rotation.x;
	newSceneObject->rotY = rotation.y;
	newSceneObject->rotZ = rotation.z;
	newSceneObject->scaX = scale.x;
	newSceneObject->scaY = scale.y;
	newSceneObject->scaZ = scale.z;
	newSceneObject->render = 0;
	newSceneObject->collision = 0;
	newSceneObject->collision_mesh = reinterpret_cast<const char*>("");
	newSceneObject->collectable = 0;
	newSceneObject->destructable = 0;
	newSceneObject->health_amount = 0;
	newSceneObject->editor_render = 1;
	newSceneObject->editor_texture_vis = 1;
	newSceneObject->editor_normals_vis = 0;
	newSceneObject->editor_collision_vis = 0;
	newSceneObject->editor_pivot_vis = 0;
	newSceneObject->pivotX = 0;
	newSceneObject->pivotY = 0;
	newSceneObject->pivotZ = 0;
	newSceneObject->snapToGround = 0;
	newSceneObject->AINode = 0;
	newSceneObject->audio_path = reinterpret_cast<const char*>("");
	newSceneObject->volume = 0;
	newSceneObject->pitch = 0;
	newSceneObject->pan = 0;
	newSceneObject->one_shot = 0;
	newSceneObject->play_on_init = 0;
	newSceneObject->play_in_editor = 0;
	newSceneObject->min_dist = 0;
	newSceneObject->max_dist = 0;
	newSceneObject->camera = 0;
	newSceneObject->path_node = 0;
	newSceneObject->path_node_start = 0;
	newSceneObject->path_node_end = 0;
	newSceneObject->parent_id = 0;
	newSceneObject->editor_wireframe = 0;
	newSceneObject->name = objectName;
				  
	newSceneObject->light_type = 1;
	newSceneObject->light_diffuse_r = 2;
	newSceneObject->light_diffuse_g = 3;
	newSceneObject->light_diffuse_b = 4;
	newSceneObject->light_specular_r = 5;
	newSceneObject->light_specular_g = 6;
	newSceneObject->light_specular_b = 7;
	newSceneObject->light_spot_cutoff = 8;
	newSceneObject->light_constant = 9;
	newSceneObject->light_linear = 0;
	newSceneObject->light_quadratic = 1;

	return newSceneObject;
}

DisplayObject* ObjectLoader::GetDisplayObject(SceneObject* newObject)
{
	//create a temp display object that we will populate then append to the display list.
	DisplayObject* newDisplayObject = new DisplayObject();

	//load model
	std::wstring modelwstr = StringToWCHART(newObject->model_path);							//convect string to Wchar
	newDisplayObject->m_model = Model::CreateFromCMO(m_device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

	//Load Texture
	std::wstring texturewstr = StringToWCHART(newObject->tex_diffuse_path);								//convect string to Wchar
	HRESULT rs;
	rs = CreateDDSTextureFromFile(m_device, texturewstr.c_str(), nullptr, &newDisplayObject->m_texture_diffuse);	//load tex into Shader resource

	//if texture fails.  load error default
	if (rs)
	{
		CreateDDSTextureFromFile(m_device, L"database/data/Error.dds", nullptr, &newDisplayObject->m_texture_diffuse);	//load tex into Shader resource
	}

	//apply new texture to models effect
	newDisplayObject->m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
	{
		auto lights = dynamic_cast<BasicEffect*>(effect);
		if (lights)
		{
			lights->SetTexture(newDisplayObject->m_texture_diffuse);
		}
	});
	newDisplayObject->m_ID = newObject->ID;
	//set position
	newDisplayObject->m_position.x = newObject->posX;
	newDisplayObject->m_position.y = newObject->posY;
	newDisplayObject->m_position.z = newObject->posZ;

	//setorientation
	newDisplayObject->m_orientation.x = newObject->rotX;
	newDisplayObject->m_orientation.y = newObject->rotY;
	newDisplayObject->m_orientation.z = newObject->rotZ;

	//set scale
	newDisplayObject->m_scale.x = newObject->scaX;
	newDisplayObject->m_scale.y = newObject->scaY;
	newDisplayObject->m_scale.z = newObject->scaZ;

	//set wireframe / render flags
	newDisplayObject->m_render = newObject->editor_render;
	newDisplayObject->m_wireframe = newObject->editor_wireframe;

	newDisplayObject->m_light_type = newObject->light_type;
	newDisplayObject->m_light_diffuse_r = newObject->light_diffuse_r;
	newDisplayObject->m_light_diffuse_g = newObject->light_diffuse_g;
	newDisplayObject->m_light_diffuse_b = newObject->light_diffuse_b;
	newDisplayObject->m_light_specular_r = newObject->light_specular_r;
	newDisplayObject->m_light_specular_g = newObject->light_specular_g;
	newDisplayObject->m_light_specular_b = newObject->light_specular_b;
	newDisplayObject->m_light_spot_cutoff = newObject->light_spot_cutoff;
	newDisplayObject->m_light_constant = newObject->light_constant;
	newDisplayObject->m_light_linear = newObject->light_linear;
	newDisplayObject->m_light_quadratic = newObject->light_quadratic;

	return newDisplayObject;
	
}

std::wstring ObjectLoader::StringToWCHART(std::string s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
