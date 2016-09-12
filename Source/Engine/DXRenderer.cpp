#include "stdafx.h"
#include "DXRenderer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "RenderTexture.h"
#include "TextureDebugger.h"
#include "DepthBuffer.h"

DXRenderer::DXRenderer()
{
	mySwapchain = nullptr;
	myDevice = nullptr;
	myDeviceContext = nullptr;
	myBackbuffer = nullptr;
	mySamplerState = nullptr;
	myRasterState = nullptr;
	myAlphaBlendingState = nullptr;
	myDepthStencilState = nullptr;

	myWidth = 16.f;
	myHeight = 9.f;
}

DXRenderer::~DXRenderer()
{
	SAFE_RELEASE(mySwapchain);
	SAFE_RELEASE(myDevice);
	SAFE_RELEASE(myDeviceContext);
	SAFE_RELEASE(myBackbuffer);
	SAFE_RELEASE(mySamplerState);
	SAFE_RELEASE(myRasterState);
	SAFE_RELEASE(myAlphaBlendingState);
	SAFE_RELEASE(myDepthStencilState);
}

void DXRenderer::Initialize(void *aOutputTarget, int aWidth, int aHeight, bool aFullscreen)
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	// If the project is in a debug build, enable debugging via SDK Layers with this flag.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.Flags = 0;
	swapChainDesc.BufferDesc.Width = aWidth;
	swapChainDesc.BufferDesc.Height = aHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferCount = 2;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = reinterpret_cast<HWND>(aOutputTarget);
	swapChainDesc.Windowed = !aFullscreen;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL featureLevel;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	CheckDXError(
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags,
			&featureLevel, 1,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&mySwapchain,
			&myDevice,
			nullptr,
			&myDeviceContext)
	);

#ifdef _DEBUG
	ID3D11Debug *myD3dDebug;
	if (SUCCEEDED(myDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&myD3dDebug)))
	{
		ID3D11InfoQueue *d3dInfoQueue = nullptr;
		if (SUCCEEDED(myD3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
		{
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, false);

			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS
			};

			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	SetupBufferViews(aWidth, aHeight);

	D3D11_RASTERIZER_DESC rasterDesc;
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;

	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// CreateInputLayout the rasterizer state from the description we just filled out.
	CheckDXError(
		myDevice->CreateRasterizerState(&rasterDesc, &myRasterState)
	);

	myDeviceContext->RSSetState(myRasterState);

	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	// CreateInputLayout an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA; // D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	CheckDXError(
		myDevice->CreateBlendState(&blendStateDescription, &myAlphaBlendingState)
	);
	
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	myDeviceContext->OMSetBlendState(myAlphaBlendingState, blendFactor, 0xffffffff);

	// Create sampler
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0.f;

	CheckDXError(
		myDevice->CreateSamplerState(&samplerDesc, &mySamplerState)
	);

	myDeviceContext->PSSetSamplers(0, 1, &mySamplerState);

	myTextureDebugger = std::make_unique<TextureDebugger>();
}

void DXRenderer::ClearFrame()
{
	ID3D11RenderTargetView * renderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
	renderTargets[0] = myBackbuffer;

	myDeviceContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, renderTargets, nullptr);

	myDepthBuffer->Bind();

	float color[4];
	color[0] = 100.f / 255.f;
	color[1] = 149.f / 255.f;
	color[2] = 237.f / 255.f;
	color[3] = 1.0f;

	SetViewport(Vector2f::Zero, Vector2f(myWidth, myHeight));

	myDeviceContext->ClearRenderTargetView(myBackbuffer, color);

	ClearDepthBuffer();
}

void DXRenderer::ClearDepthBuffer()
{
	myDepthBuffer->Clear(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.f, 0);
}

void DXRenderer::Present()
{
	myTextureDebugger->Render();

	mySwapchain->Present(1, 0);
}

void DXRenderer::Resize(int aNewWidth, int aNewHeight)
{
	myWidth = static_cast<float>(aNewWidth);
	myHeight = static_cast<float>(aNewHeight);
	myDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	SAFE_RELEASE(myBackbuffer);

	CheckDXError(
		mySwapchain->ResizeBuffers(0, aNewWidth, aNewHeight, DXGI_FORMAT_UNKNOWN, 0);
	);

	SetupBufferViews(aNewWidth, aNewHeight);
}

std::shared_ptr<RenderTexture> DXRenderer::GetBackBuffer()
{
	return myBackbufferWrapper;
}

std::shared_ptr<DepthBuffer> DXRenderer::GetDepthBuffer()
{
	return myDepthBuffer;
}

TextureDebugger& DXRenderer::GetTextureDebugger()
{
	return *myTextureDebugger;
}

float DXRenderer::GetAspectRatio() const
{
	return myWidth / myHeight;
}

float DXRenderer::GetWidth() const
{
	return myWidth;
}

float DXRenderer::GetHeight() const
{
	return myHeight;
}


void DXRenderer::SetupBufferViews(int aWidth, int aHeight)
{
	SAFE_RELEASE(myBackbuffer);
	SAFE_RELEASE(myDepthStencilState);

	ID3D11Texture2D *backBuffer = nullptr;
	CheckDXError(
		mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)
	);

	CheckDXError(
		myDevice->CreateRenderTargetView(backBuffer, NULL, &myBackbuffer)
	);

	myBackbufferWrapper = std::make_shared<RenderTexture>(myBackbuffer);

	backBuffer->Release();
	backBuffer = nullptr;
	
	myDepthBuffer = std::make_shared<DepthBuffer>(aWidth, aHeight);
	
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// CreateInputLayout the depth stencil state.
	CheckDXError(
		myDevice->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilState)
	);

	myDeviceContext->OMSetDepthStencilState(myDepthStencilState, 1);

	myDepthBuffer->Bind();

	ResetViewport();
}

void DXRenderer::SetViewport(const Vector2f & aTopLeft, const Vector2f & aSize)
{
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = aTopLeft.x;
	viewport.TopLeftY = aTopLeft.y;
	viewport.Width = aSize.x;
	viewport.Height = aSize.y;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	myDeviceContext->RSSetViewports(1, &viewport);
}

void DXRenderer::ResetViewport()
{
	SetViewport(Vector2f::Zero, Vector2f(myWidth, myHeight));
}