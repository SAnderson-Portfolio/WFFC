//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "DisplayObject.h"
#include <string>


using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game()

{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
	m_displayList.clear();
	//initial Settings
	
	//modes
	m_grid = false;

	
}

Game::~Game()
{
	if (m_widgetStencil)
	{
		m_widgetStencil->Release();
		m_widgetStencil = nullptr;
	}
#ifdef DXTK_AUDIO
    if (m_audEngine)
    {
        m_audEngine->Suspend();
    }
#endif
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_gamePad = std::make_unique<GamePad>();

    m_keyboard = std::make_unique<Keyboard>();

    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    m_deviceResources->SetWindow(window, width, height);
	
	m_camera.SetWindowSizes(width, height);
    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();
	
	GetClientRect(window, &m_screenDimensions);
	m_winWidth = width;
	m_winHeight = height;
	m_RayIntersectPoint = DirectX::SimpleMath::Vector3::Zero;
	m_terrainUpdateRequired = true;
	m_transformStarted = false;
	m_deselectTriggered = false;
	m_changesStacked = false;
	m_stateChangeHappened = false;
	m_undoStackSize = 10;

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_deviceResources->GetD3DDevice()->CreateDepthStencilState(&dsDesc, &m_widgetStencil);

	m_currentPickingState = PickingState::PICKINGNONE;
#ifdef DXTK_AUDIO
    // Create DirectXTK for Audio objects
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif

    m_audEngine = std::make_unique<AudioEngine>(eflags);

    m_audioEvent = 0;
    m_audioTimerAcc = 10.f;
    m_retryDefault = false;

    m_waveBank = std::make_unique<WaveBank>(m_audEngine.get(), L"adpcmdroid.xwb");

    m_soundEffect = std::make_unique<SoundEffect>(m_audEngine.get(), L"MusicMono_adpcm.wav");
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);

    m_effect1->Play(true);
    m_effect2->Play();
#endif

	m_selectionID = -1;
}

void Game::SetGridState(bool state)
{
	m_grid = state;
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick(InputCommands *Input)
{
	//copy over the input commands so we have a local version to use elsewhere.
	m_InputCommands = *Input;
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });
#ifdef DXTK_AUDIO
    // Only update audio engine once per frame
    if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
#endif

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	//Update the camera
	m_camera.HandleInput(m_InputCommands);
	m_camera.Update();
	//Set the matrices
    m_batchEffect->SetView(m_camera.GetViewMatrix());
    m_batchEffect->SetWorld(Matrix::Identity);
	m_displayChunk.m_terrainEffect->SetView(m_camera.GetViewMatrix());
	m_displayChunk.m_terrainEffect->SetWorld(Matrix::Identity);

#ifdef DXTK_AUDIO
    m_audioTimerAcc -= (float)timer.GetElapsedSeconds();
    if (m_audioTimerAcc < 0)
    {
        if (m_retryDefault)
        {
            m_retryDefault = false;
            if (m_audEngine->Reset())
            {
                // Restart looping audio
                m_effect1->Play(true);
            }
        }
        else
        {
            m_audioTimerAcc = 4.f;

            m_waveBank->Play(m_audioEvent++);

            if (m_audioEvent >= 11)
                m_audioEvent = 0;
        }
    }
#endif
	
	if (VALID_POINTER_INDEX(m_selectionID, m_sceneGraph))
	{
		m_TransformWidgets.UpdateWidgets(m_sceneGraph->at(m_selectionID), m_camera.GetCameraPosition());

		if (m_currentPickingState == PickingState::TRANSFORM)
		{
			if (m_TransformWidgets.GetMode() != TransformModes::NONE && !m_transformStarted)
				m_TransformWidgets.MousePicking(m_InputCommands.mouseX, m_InputCommands.mouseY, m_screenDimensions, m_world, m_projection, m_camera.GetViewMatrix(),
					m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth);
		}
		
		if (m_InputCommands.escape)
		{
			m_deselectTriggered = true;
			m_InputCommands.escape = false;
		}

		if (m_InputCommands.del)
		{
			AddChanges();
			m_sceneGraph->erase(m_sceneGraph->begin() + m_selectionID);
			m_displayListUpdateRequired = true;
			m_deselectTriggered = true;
			m_InputCommands.del = false;
		}
	}

}

void Game::UpdateTerrain()
{
	m_displayChunk.GenerateHeightmap();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }
	
	Clear();

	auto context = m_deviceResources->GetD3DDeviceContext();
	m_deviceResources->PIXBeginEvent(L"Render");

	

	if (m_grid)
	{
		// Draw procedurally generated dynamic grid
		const XMVECTORF32 xaxis = { 512.f, 0.f, 0.f };
		const XMVECTORF32 yaxis = { 0.f, 0.f, 512.f };
		DrawGrid(xaxis, yaxis, g_XMZero, 512, 512, Colors::Gray);
	}
	
	//RENDER OBJECTS FROM SCENEGRAPH
	int numRenderObjects = m_displayList.size();
	for (int i = 0; i < numRenderObjects; i++)
	{
		if (!m_displayList[i].m_render)
			continue;

		m_deviceResources->PIXBeginEvent(L"Draw model");

		const XMVECTORF32 scale = { m_displayList[i].m_scale.x, m_displayList[i].m_scale.y, m_displayList[i].m_scale.z };
		const XMVECTORF32 translate = { m_displayList[i].m_position.x, m_displayList[i].m_position.y, m_displayList[i].m_position.z };

		//convert degrees into radians for rotation matrix
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y *3.1415 / 180,
			m_displayList[i].m_orientation.x *3.1415 / 180,
			m_displayList[i].m_orientation.z *3.1415 / 180);

		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

		m_displayList[i].m_model->Draw(context, *m_states, local, m_camera.GetViewMatrix(), m_projection, m_displayList[i].m_wireframe);	//last variable in draw,  make TRUE for wireframe
		if (i == m_selectionID)
		{
			XMMATRIX wireLocal = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale * 1.01, g_XMZero, rotate, translate);
			m_displayList[i].m_model->UpdateEffects([&](IEffect* effect)
			{

				auto fog = dynamic_cast<IEffectFog*>(effect);
				if (fog)
				{
					fog->SetFogEnabled(true);
					fog->SetFogStart(0);
					fog->SetFogEnd(1);
					fog->SetFogColor(Colors::Yellow);
				}
			});
			m_displayList[i].m_model->Draw(context, *m_states, wireLocal, m_camera.GetViewMatrix(), m_projection, true);	//last variable in draw,  make TRUE for wireframe
			m_displayList[i].m_model->UpdateEffects([&](IEffect* effect)
			{

				auto fog = dynamic_cast<IEffectFog*>(effect);
				if (fog)
				{
					fog->SetFogEnabled(false);
					fog->SetFogStart(0);
					fog->SetFogEnd(1);
					fog->SetFogColor(Colors::Yellow);
				}
			});

		}
		m_deviceResources->PIXEndEvent();
	}
	

	m_deviceResources->PIXEndEvent();

	//RENDER TERRAIN
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	context->RSSetState(m_states->CullNone());
	if(m_displayChunk.GetWireFrame())
		context->RSSetState(m_states->Wireframe());		//uncomment for wireframe

	//Render the batch,  This is handled in the Display chunk becuase it has the potential to get complex
	m_displayChunk.RenderBatch(m_deviceResources);
	
	
	//ComPtr<ID3D11Texture2D> backBuffer;
	//
	//HRESULT hr = m_deviceResources->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D),
	//	reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
	//if (SUCCEEDED(hr))
	//{
	//	hr = SaveDDSTextureToFile(context, backBuffer.Get(),
	//		L"SCREENSHOT.DDS");
	//	DX::ThrowIfFailed(hr);
	//}

	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->OMSetDepthStencilState(m_widgetStencil, 1u);
	if (VALID_POINTER_INDEX(m_selectionID, m_sceneGraph) && m_currentPickingState == PickingState::TRANSFORM)
	{
		m_deviceResources->PIXBeginEvent(L"Render Widgets");
		m_TransformWidgets.RenderWidgets(context, *m_states, m_world, m_camera.GetViewMatrix(), m_projection);
		m_deviceResources->PIXEndEvent();
	}

	//CAMERA POSITION ON HUD
	m_sprites->Begin();
	WCHAR   Buffer[256];
	std::wstring var = L"Cam X: " + std::to_wstring(m_camera.GetCameraPosition().x) + L"Cam Z: " + std::to_wstring(m_camera.GetCameraPosition().z);
	std::wstring cameraText;
	switch (m_camera.GetCurrentCameraMode())
	{
	case CameraMode::FOCUSED:
		cameraText = L"Camera: Focused";
		break;
	case CameraMode::FREE:
		cameraText = L"Camera: Free";
		break;
	case CameraMode::ROTATE:
		cameraText = L"Camera: Rotating";
		break;
	}
	//std::wstring ray = L"Ray X: " + std::to_wstring(m_RayIntersectPoint.x) + L"Ray Y: " + std::to_wstring(m_RayIntersectPoint.y) + L"Ray Z: " + std::to_wstring(m_RayIntersectPoint.z);
	m_font->DrawString(m_sprites.get(), var.c_str(), XMFLOAT2(100, 10), Colors::Yellow);
	m_font->DrawString(m_sprites.get(), cameraText.c_str(), XMFLOAT2(100, 30), Colors::Yellow);
	m_sprites->End();

	

	m_deviceResources->Present();


	
	
	//m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetBackBufferRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}

void XM_CALLCONV Game::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
    m_deviceResources->PIXBeginEvent(L"Draw grid");

    auto context = m_deviceResources->GetD3DDeviceContext();
    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_states->DepthNone(), 0);
    context->RSSetState(m_states->CullCounterClockwise());

    m_batchEffect->Apply(context);

    context->IASetInputLayout(m_batchInputLayout.Get());

    m_batch->Begin();

    xdivs = std::max<size_t>(1, xdivs);
    ydivs = std::max<size_t>(1, ydivs);

    for (size_t i = 0; i <= xdivs; ++i)
    {
        float fPercent = float(i) / float(xdivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    for (size_t i = 0; i <= ydivs; i++)
    {
        float fPercent = float(i) / float(ydivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    m_batch->End();

    m_deviceResources->PIXEndEvent();
}


#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
#ifdef DXTK_AUDIO
    m_audEngine->Suspend();
#endif
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

#ifdef DXTK_AUDIO
    m_audEngine->Resume();
#endif
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();
}


void Game::BuildDisplayList(std::vector<SceneObject> * SceneGraph)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();


	m_sceneGraph = SceneGraph;
	if (!m_displayList.empty())		//is the vector empty
	{
		m_displayList.clear();		//if not, empty it
	}

	//for every item in the scenegraph
	int numObjects = SceneGraph->size();
	for (int i = 0; i < numObjects; i++)
	{
		
		//create a temp display object that we will populate then append to the display list.
		DisplayObject newDisplayObject;
		
		//load model
		std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);							//convect string to Wchar
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

		//Load Texture
		std::wstring texturewstr = StringToWCHART(SceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
		HRESULT rs;
		rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

		//if texture fails.  load error default
		if (rs)
		{
			CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
		}

		//apply new texture to models effect
		newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		{	
			auto lights = dynamic_cast<BasicEffect*>(effect);
			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);			
			}
		});
		newDisplayObject.m_ID = SceneGraph->at(i).ID;
		//set position
		newDisplayObject.m_position.x = SceneGraph->at(i).posX;
		newDisplayObject.m_position.y = SceneGraph->at(i).posY;
		newDisplayObject.m_position.z = SceneGraph->at(i).posZ;
		
		//setorientation
		newDisplayObject.m_orientation.x = SceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = SceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = SceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = SceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = SceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = SceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render		= SceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe	= SceneGraph->at(i).editor_wireframe;

		newDisplayObject.m_light_type		= SceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r	= SceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g	= SceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b	= SceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_specular_r = SceneGraph->at(i).light_specular_r;
		newDisplayObject.m_light_specular_g = SceneGraph->at(i).light_specular_g;
		newDisplayObject.m_light_specular_b = SceneGraph->at(i).light_specular_b;
		newDisplayObject.m_light_spot_cutoff = SceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant	= SceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear		= SceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic	= SceneGraph->at(i).light_quadratic;
		
		m_displayList.push_back(newDisplayObject);
		
	}
}

void Game::BuildDisplayChunk(ChunkObject * SceneChunk)
{
	//populate our local DISPLAYCHUNK with all the chunk info we need from the object stored in toolmain
	//which, to be honest, is almost all of it. Its mostly rendering related info so...
	m_displayChunk.PopulateChunkData(SceneChunk);		//migrate chunk data
	m_displayChunk.LoadHeightMap(m_deviceResources);
	m_displayChunk.m_terrainEffect->SetProjection(m_projection);
	m_displayChunk.InitialiseBatch();
}

void Game::SetTransformEnd()
{
	m_transformStarted = false;
	m_changesStacked = false;
}

void Game::SaveDisplayChunk(ChunkObject * SceneChunk)
{
	m_displayChunk.SaveHeightMap();			//save heightmap to file.
}

void Game::ChangeCameraMode()
{
	m_camera.SetCameraMode(CameraMode::FREE);
}

void Game::SetSelectionID(int selected)
{
	m_selectionID = selected;

	if (VALID_INDEX(m_selectionID, m_displayList))
	{
		m_camera.SetFocus(m_displayList[m_selectionID]);
	}
	else
	{
		m_camera.RemoveFocus();
	}
}

int Game::MousePicking(bool isLMB)
{
	FLOAT minDepth = m_deviceResources->GetScreenViewport().MinDepth;
	FLOAT maxDepth = m_deviceResources->GetScreenViewport().MaxDepth;

	if (m_selectionID == -1 && isLMB)
	{
		int newSelectedID = MousePicking::PickObjectID(m_displayList, m_InputCommands.mouseX, m_InputCommands.mouseY, m_world, m_projection, m_camera.GetViewMatrix(), m_screenDimensions, minDepth, maxDepth);

		if (newSelectedID != -1)
		{
			m_selectionID = newSelectedID;
			m_camera.SetFocus(m_displayList[m_selectionID]);
		}
	}

	if (!isLMB && m_selectionID != -1)
	{
		int newSelectedID = MousePicking::PickObjectID(m_displayList, m_InputCommands.mouseX, m_InputCommands.mouseY, m_world, m_projection, m_camera.GetViewMatrix(), m_screenDimensions, minDepth, maxDepth);
		if (newSelectedID == m_selectionID)
		{
			m_deselectTriggered = true;
		}

	}
	
	
	switch (m_currentPickingState)
	{
	case PickingState::TERRAINHEIGHT:
		MousePicking::SculptTerrain(m_displayChunk, m_camera.GetCameraPosition(), m_InputCommands.mouseX, m_InputCommands.mouseY, m_world, m_projection, m_camera.GetViewMatrix(),
			m_screenDimensions, minDepth, maxDepth, isLMB);
		break;

	case PickingState::TERRAINSCALE:
		MousePicking::ScaleTerrain(m_displayChunk, m_camera.GetCameraPosition(), m_InputCommands.mouseX, m_InputCommands.mouseY, m_world, m_projection, m_camera.GetViewMatrix(),
			m_screenDimensions, minDepth, maxDepth, isLMB);
		break;
	case PickingState::TERRAINFLATTEN:
		MousePicking::FlattenTerrain(m_displayChunk, m_camera.GetCameraPosition(), m_InputCommands.mouseX, m_InputCommands.mouseY, m_world, m_projection, m_camera.GetViewMatrix(),
			m_screenDimensions, minDepth, maxDepth);
		break;
	case PickingState::TRANSFORM:
		if (m_TransformWidgets.GetMode() != TransformModes::NONE)
		{
			if (!m_transformStarted)
			{
				m_TransformWidgets.MousePicking(m_InputCommands.mouseX, m_InputCommands.mouseY, m_screenDimensions, m_world, m_projection, m_camera.GetViewMatrix(),
					minDepth, maxDepth);
				m_TransformWidgets.SetTransformMouseStart(m_InputCommands.mouseX, m_InputCommands.mouseY);
				m_transformStarted = true;
				if (!m_changesStacked)
				{
					m_changesStacked = true;
					AddChanges();
				}
				m_displayListUpdateRequired = true;
			}
			else
			{
				if (VALID_POINTER_INDEX(m_selectionID, m_sceneGraph))
				{
					m_TransformWidgets.UpdateMousePosition(m_InputCommands.mouseX, m_InputCommands.mouseY, m_sceneGraph->at(m_selectionID));
					m_displayListUpdateRequired = true;
				}
			}
		}
		break;
	case PickingState::TERRAINPAINT:
		MousePicking::PaintTerrain(m_displayChunk, m_camera.GetCameraPosition(), m_InputCommands.mouseX, m_InputCommands.mouseY, m_world, m_projection, m_camera.GetViewMatrix(),
			m_screenDimensions, minDepth, maxDepth, isLMB);
		break;
	case PickingState::FREEMOVE:
		if (VALID_POINTER_INDEX(m_selectionID, m_sceneGraph))
		{
			Vector3 intersectionPoint = Vector3::Zero;
			if (m_InputCommands.shift)
			{
				DisplayObject tempObject;
				float pickedDistance = MousePicking::PickDisplayObjectDistance(tempObject, m_displayList, m_InputCommands.mouseX, m_InputCommands.mouseY, m_world, m_projection, m_camera.GetViewMatrix(), m_screenDimensions,
					minDepth, maxDepth);
				if (pickedDistance != INFINITE && tempObject.m_ID != m_displayList[m_selectionID].m_ID)
				{
					Vector3 tempCamera = m_camera.GetCameraPosition();
					tempCamera.y = tempObject.m_position.y;
					Vector3 dist = tempObject.m_position - tempCamera;
					dist.Normalize();
					dist *= tempObject.m_model->meshes[0]->boundingBox.Extents * 2.0f;

					intersectionPoint = tempObject.m_position - dist;

					m_sceneGraph->at(m_selectionID).rotX = tempObject.m_orientation.x;
					m_sceneGraph->at(m_selectionID).rotY = tempObject.m_orientation.y;
					m_sceneGraph->at(m_selectionID).rotZ = tempObject.m_orientation.z;
				}
				
			}
			else
			{

				MousePicking::CheckForTriangleIntersection(m_displayChunk, m_camera.GetCameraPosition(), m_InputCommands.mouseX, m_InputCommands.mouseY, m_world, m_projection, m_camera.GetViewMatrix(), m_screenDimensions,
					minDepth, maxDepth, intersectionPoint);
			}
			if (intersectionPoint != Vector3::Zero)
			{
				if (!m_changesStacked && !m_transformStarted)
				{
					m_changesStacked = true;
					m_transformStarted = true;
					AddChanges();
				}

				m_sceneGraph->at(m_selectionID).posX = intersectionPoint.x;
				m_sceneGraph->at(m_selectionID).posY = intersectionPoint.y;
				m_sceneGraph->at(m_selectionID).posZ = intersectionPoint.z;
				m_displayListUpdateRequired = true;
			}
		}
		break;
	default:
		break;
	}

	return m_selectionID;
}

//LoadObjects
void Game::LoadObjects(ObjectLoader* objectLoader)
{
	m_TransformWidgets.AddWidgets(objectLoader);
}

void Game::UndoChanges()
{
	if (m_undoStackDisplayList.empty())
		return;

	m_deselectTriggered = true;
	m_redoStackDisplayList.push_back(*m_sceneGraph);

	std::vector<SceneObject> tempList;
	tempList = m_undoStackDisplayList.back();

	m_undoStackDisplayList.pop_back();

	*m_sceneGraph = tempList;
	m_displayListUpdateRequired = true;
}

void Game::RedoChanges()
{
	if (m_redoStackDisplayList.empty())
		return;

	m_deselectTriggered = true; 
	m_undoStackDisplayList.push_back(*m_sceneGraph);

	std::vector<SceneObject> tempList;
	tempList = m_redoStackDisplayList.back();

	m_redoStackDisplayList.pop_back();

	*m_sceneGraph = tempList;
	m_displayListUpdateRequired = true;
}

void Game::AddChanges()
{
	if (m_undoStackDisplayList.size() + 1 > m_undoStackSize)
	{
		m_undoStackDisplayList.pop_front();
	}
	m_undoStackDisplayList.push_back(*m_sceneGraph);
	m_redoStackDisplayList.clear();
}

void Game::StacksHaveElements(bool& undoStack, bool& redoStack)
{
	if (!m_undoStackDisplayList.empty())
		undoStack = true;
	else
		undoStack = false;

	if (!m_redoStackDisplayList.empty())
		redoStack = true;
	else
		redoStack = false;
}

void Game::ResetObjectTransform()
{
	AddChanges();
	SceneObject* tempObject = &m_sceneGraph->at(m_selectionID);
	tempObject->posX = 0;
	tempObject->posY = 0;
	tempObject->posZ = 0;

	tempObject->rotX = 0;
	tempObject->rotY = 0;
	tempObject->rotZ = 0;

	tempObject->scaX = 1;
	tempObject->scaY = 1;
	tempObject->scaZ = 1;
	m_displayListUpdateRequired = true;
}

#ifdef DXTK_AUDIO
void Game::NewAudioDevice()
{
    if (m_audEngine && !m_audEngine->IsAudioDevicePresent())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
}
#endif


#pragma endregion


#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_states = std::make_unique<CommonStates>(device);

    m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"database/data/"); //fx Factory will look in the database directory
	m_fxFactory->SetSharing(false);	//we must set this to false otherwise it will share effects based on the initial tex loaded (When the model loads) rather than what we will change them to.

    m_sprites = std::make_unique<SpriteBatch>(context);

    m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

    m_batchEffect = std::make_unique<BasicEffect>(device);
    m_batchEffect->SetVertexColorEnabled(true);

    {
        void const* shaderByteCode;
        size_t byteCodeLength;

        m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

        DX::ThrowIfFailed(
            device->CreateInputLayout(VertexPositionColor::InputElements,
                VertexPositionColor::InputElementCount,
                shaderByteCode, byteCodeLength,
                m_batchInputLayout.ReleaseAndGetAddressOf())
        );
    }

    m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

//    m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

    // SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
    m_model = Model::CreateFromSDKMESH(device, L"tiny.sdkmesh", *m_fxFactory);
	

    // Load textures
    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf())
    );

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
    );

}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(size.right) / float(size.bottom);
    float fovAngleY = 70.0f * XM_PI / 180.0f;

    // This is a simple example of change that can be made when the app is in
    // portrait or snapped view.
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    m_projection = Matrix::CreatePerspectiveFieldOfView(
        fovAngleY,
        aspectRatio,
        0.01f,
        1000.0f
    );

    m_batchEffect->SetProjection(m_projection);
	
}

void Game::OnDeviceLost()
{
    m_states.reset();
    m_fxFactory.reset();
    m_sprites.reset();
    m_batch.reset();
    m_batchEffect.reset();
    m_font.reset();
    m_shape.reset();
    m_model.reset();
    m_texture1.Reset();
    m_texture2.Reset();
    m_batchInputLayout.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

std::wstring StringToWCHART(std::string s)
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
