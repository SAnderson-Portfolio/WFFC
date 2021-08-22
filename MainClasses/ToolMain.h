#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include <vector>
#include "ObjectLoader.h"
class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	int&	getCurrentSelectionID();									//returns the selection number of currently selected object so that It can be displayed.
	void	setCurrentSelectionID(int newID) { m_selectedObjectID = newID; }
	void	onActionInitialise(HWND handle, HWND mainHandle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry
	afx_msg void	onFreeCamToggle();

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	void SetScreenCorner(POINT screenCorner) {	corner = screenCorner;	}
	std::vector<DisplayObject>& GetDisplayList() { return m_d3dRenderer.GetDisplayList(); }
	std::vector<SceneObject>& GetSceneGraph() { return m_sceneGraph; }
	TransformWidgets* GetTransformWidgets() { return m_d3dRenderer.GetTransformWidgets(); }
	bool InspectorRequiresUpdate() { return m_d3dRenderer.InspectorRequiresUpdate(); }
	//Duplicates the currently selected object.
	void DuplicateSelectedObject();
	//Rebuilds the display list in Game if required.
	void RebuildDisplayList();
	//Passthrough picking messages
	void SetTransformPicking() { m_d3dRenderer.SetTransformPicking(); }
	void SetFreeMovePicking() { m_d3dRenderer.SetFreeMovePicking(); }
	void SetTerrainSculptingPicking() { m_d3dRenderer.SetTerrainSculptingPicking(); }
	void SetTerrainFlattenPicking() { m_d3dRenderer.SetTerrainFlattenPicking(); }
	void SetTerrainScalingPicking() { m_d3dRenderer.SetTerrainScalingPicking(); }
	void SetTerrainPaintPicking() { m_d3dRenderer.SetTerrainPaintPicking(); }
	//Widget messages
	void SetWidgetsToTranslate() { m_d3dRenderer.SetWidgetsToTranslate(); }
	void SetWidgetsToRotate() { m_d3dRenderer.SetWidgetsToRotate(); }
	void SetWidgetsToScale() { m_d3dRenderer.SetWidgetsToScale(); }

	//Passthrough state changes.
	void RedoChanges() { m_d3dRenderer.RedoChanges(); }
	void UndoChanges() { m_d3dRenderer.UndoChanges(); }
	void AddChanges() { m_d3dRenderer.AddChanges(); }
	void ResetObjectTransform() { m_d3dRenderer.ResetObjectTransform(); }
	void StacksHaveElements(bool& undoStack, bool& redoStack) { m_d3dRenderer.StacksHaveElements(undoStack, redoStack); }

	ObjectLoader* GetObjectLoader() { return m_objectLoader; }
	Game* GetGameObject() { return &m_d3dRenderer; }
	DisplayChunk* GetDisplayChunk() { return m_d3dRenderer.GetDisplayChunk(); }
public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk

private:	//methods
	void	onContentAdded();
	void	InitInputValues();

		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	HWND	m_mainHandle;		//Handle to the  main window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	
	POINT corner;
	
	ObjectLoader*	m_objectLoader;
	int				m_selectedObjectID;						//ID of current Selection
};
