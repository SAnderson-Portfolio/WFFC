//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SceneObject.h"
#include "DisplayObject.h"
#include "DisplayChunk.h"
#include "ChunkObject.h"
#include "InputCommands.h"
#include <vector>
#include <deque>
#include "Camera.h"
#include "CameraModes.h"
#include "TransformWidgets.h"
#include "TransformModes.h"
#include "ObjectLoader.h"
#include "MousePicking.h"
#include "MousePickingStates.h"
#include <ScreenGrab.h>
// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

	Game();
	~Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);
	void SetGridState(bool state);

	// Basic game loop
	void Tick(InputCommands * Input);
	void Render();

	// Rendering helpers
	void Clear();

	// IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	//tool specific
	void BuildDisplayList(std::vector<SceneObject> * SceneGraph); //note vector passed by reference
	void BuildDisplayChunk(ChunkObject *SceneChunk);
	void SetTransformEnd();
	const bool GetTransformStarted() { return m_transformStarted; }
	const bool InspectorRequiresUpdate() { return m_transformStarted; }
	const bool DeselectRequired() { return m_deselectTriggered; }
	void DeselectPerformed() { m_deselectTriggered = false; }
	DisplayChunk* GetDisplayChunk() { return &m_displayChunk; }
	std::vector<DisplayObject>& GetDisplayList() { return m_displayList; }
	DirectX::EffectFactory& GetEffectFactory() { return *m_fxFactory; }
	TransformWidgets* GetTransformWidgets() { return &m_TransformWidgets; }
	ID3D11Device& GetDevice() { return *m_deviceResources->GetD3DDevice(); }
	void SaveDisplayChunk(ChunkObject *SceneChunk);	//saves geometry et al
	void ChangeCameraMode();
	void ClearDisplayList();

	void SetSelectionID(int selected);
	int GetSelectionID() { return m_selectionID; }

	int MousePicking(bool isLMB = false);

	//Mouse picking mode
	void SetTransformPicking() { m_currentPickingState = PickingState::TRANSFORM; }
	void SetTerrainSculptingPicking() { m_currentPickingState = PickingState::TERRAINHEIGHT; }
	void SetTerrainFlattenPicking() { m_currentPickingState = PickingState::TERRAINFLATTEN; }
	void SetTerrainScalingPicking() { m_currentPickingState = PickingState::TERRAINSCALE; }
	void SetTerrainPaintPicking() { m_currentPickingState = PickingState::TERRAINPAINT; }
	void SetFreeMovePicking() { m_currentPickingState = PickingState::FREEMOVE; }
	//Change widget type
	//Set widgets to translate.
	void SetWidgetsToTranslate() { m_TransformWidgets.SetMode(TransformModes::TRANSLATION); SetTransformPicking();	}
	//Set widgets to rotate.
	void SetWidgetsToRotate() { m_TransformWidgets.SetMode(TransformModes::ROTATION);  SetTransformPicking(); }
	//Set widgets to scale.
	void SetWidgetsToScale() { m_TransformWidgets.SetMode(TransformModes::SCALING);  SetTransformPicking();	}
	//Load any additional objects required.
	void LoadObjects(ObjectLoader* objectLoader);
	//Undo changes if any exist.
	void UndoChanges();
	//Redo changes if any exist.
	void RedoChanges();
	//Add a state to the stack before any changes are made.
	void AddChanges();
	void StacksHaveElements(bool& undoStack, bool& redoStack);
	PickingState GetCurrentPicking() { return m_currentPickingState; }
	bool DisplayListUpdateRequired() { return m_displayListUpdateRequired; }
	void SetDisplayListUpdateRequired(bool newState) { m_displayListUpdateRequired = newState; }

	void ResetObjectTransform();
#ifdef DXTK_AUDIO
	void NewAudioDevice();
#endif

private:

	void Update(DX::StepTimer const& timer);
	void UpdateTerrain();
	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void XM_CALLCONV DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);

	//tool specific
	std::vector<DisplayObject>			m_displayList;
	std::vector<SceneObject>*			m_sceneGraph;
	std::deque<std::vector<SceneObject>> m_redoStackDisplayList;
	std::deque<std::vector<SceneObject>> m_undoStackDisplayList;
	DisplayChunk						m_displayChunk;
	InputCommands						m_InputCommands;

	//control variables
	bool m_grid;							//grid rendering on / off
	// Device resources.
    std::shared_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

    // Input devices.
    std::unique_ptr<DirectX::GamePad>       m_gamePad;
    std::unique_ptr<DirectX::Keyboard>      m_keyboard;
    std::unique_ptr<DirectX::Mouse>         m_mouse;

    // DirectXTK objects.
    std::unique_ptr<DirectX::CommonStates>                                  m_states;
    std::unique_ptr<DirectX::BasicEffect>                                   m_batchEffect;
    std::unique_ptr<DirectX::EffectFactory>                                 m_fxFactory;
    std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape;
    std::unique_ptr<DirectX::Model>                                         m_model;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
    std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
    std::unique_ptr<DirectX::SpriteFont>                                    m_font;

#ifdef DXTK_AUDIO
    std::unique_ptr<DirectX::AudioEngine>                                   m_audEngine;
    std::unique_ptr<DirectX::WaveBank>                                      m_waveBank;
    std::unique_ptr<DirectX::SoundEffect>                                   m_soundEffect;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect1;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect2;
#endif

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture1;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture2;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;

#ifdef DXTK_AUDIO
    uint32_t                                                                m_audioEvent;
    float                                                                   m_audioTimerAcc;

    bool                                                                    m_retryDefault;
#endif

    DirectX::SimpleMath::Matrix                                             m_world;
	DirectX::SimpleMath::Matrix                                             m_projection;

	//Camera object
	Camera																	m_camera;
	int																		m_selectionID;

	//Window rect
	RECT																	m_screenDimensions;

	int																		m_winWidth;
	int																		m_winHeight;
	//The point that a ray intersection has occured.
	DirectX::SimpleMath::Vector3											m_RayIntersectPoint;
	//Widgets used for transforming obejcts.
	TransformWidgets														m_TransformWidgets;
	//Terrain requires an update.
	bool																	m_terrainUpdateRequired;
	//A transform action has begun.
	bool																    m_transformStarted;
	//The selected object should be reset.
	bool																	m_deselectTriggered;
	//Control changes being stacked.
	bool																	m_changesStacked;
	//Where the mouse picking should be performed.
	PickingState															m_currentPickingState;
	ID3D11DepthStencilState*												m_widgetStencil;
	//If the display list requires and update
	bool																	m_displayListUpdateRequired;
	//How many undo steps there can be,
	int																		m_undoStackSize;
	bool																	m_stateChangeHappened;
};

std::wstring StringToWCHART(std::string s);