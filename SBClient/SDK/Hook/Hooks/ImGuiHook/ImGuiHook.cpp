#include "ImGuiHook.h"
#include <stdio.h>

HWND ImGuiHook::window = nullptr;
WNDPROC ImGuiHook::oWndProc = nullptr;

ImGuiHook::FrameContext* ImGuiHook::g_frameContext = nullptr;
int ImGuiHook::NUM_BACK_BUFFERS = 0;
bool ImGuiHook::g_ImGuiInitialized = false;

winrt::com_ptr<IDXGISwapChain3> ImGuiHook::g_pSwapChain;
winrt::com_ptr<ID3D12GraphicsCommandList> ImGuiHook::g_pd3dCommandList;
winrt::com_ptr<ID3D12Device> ImGuiHook::g_pd3dDevice;
winrt::com_ptr<ID3D12DescriptorHeap> ImGuiHook::g_pd3dSrvDescHeap;
winrt::com_ptr<ID3D12DescriptorHeap> ImGuiHook::g_pd3dRtvDescHeap;
winrt::com_ptr<ID3D12CommandQueue> ImGuiHook::g_pd3dCommandQueue;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
//dx11
static ID3D11RenderTargetView* mainRenderTargetView = nullptr;

typedef HRESULT(__stdcall* PresentFunc)(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
PresentFunc oPresent = nullptr;

typedef HRESULT(__stdcall* ResizeBuffersFunc12)(IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
ResizeBuffersFunc12 oResizeBuffers = nullptr;

typedef HRESULT(__stdcall* ResizeBuffersFunc11)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
ResizeBuffersFunc11 oResizeBuffers11 = nullptr;

typedef void(__stdcall* ExecuteCommandListsFunc)(ID3D12CommandQueue* pCommandQueue, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists);
ExecuteCommandListsFunc oExecuteCommandLists = nullptr;

typedef HRESULT(__stdcall* SignalFunc)(ID3D12CommandQueue* queue, ID3D12Fence* fence, UINT64 value);
SignalFunc oSignal = nullptr;

void ImGuiHook::CreateRenderTarget() {
	if (!g_pSwapChain || !g_pd3dDevice || !g_frameContext || NUM_BACK_BUFFERS <= 0)
		return;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	UINT rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


	for (int i = 0; i < NUM_BACK_BUFFERS; i++) {
		if (g_frameContext[i].RenderTargetResource) {
			g_frameContext[i].CommandAllocator = nullptr;
			g_frameContext[i].RenderTargetResource = nullptr;
		}
		
		g_frameContext[i].RenderTargetDescriptor = rtvHandle;
		
		ID3D12Resource* pBackBuffer = nullptr;

		HRESULT hr = g_pd3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			__uuidof(ID3D12CommandAllocator),
			reinterpret_cast<void**>(g_frameContext[i].CommandAllocator.put())
		);


		if (SUCCEEDED(g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer)))) {
			g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, rtvHandle);
			g_frameContext[i].RenderTargetResource.attach( pBackBuffer);
		}
		rtvHandle.ptr += rtvDescriptorSize;
	}
	
		g_pd3dCommandList = nullptr;

		HRESULT hr = g_pd3dDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			g_frameContext[0].CommandAllocator.get(),
			nullptr,
			__uuidof(ID3D12GraphicsCommandList),
			reinterpret_cast<void**>(g_pd3dCommandList.put())
		);
	if (SUCCEEDED(hr)) {
		
		g_pd3dCommandList->Close();
		
	}
	
}

void ImGuiHook::CleanupRenderTarget() {
	if (g_frameContext && NUM_BACK_BUFFERS > 0) {
		for (int i = 0; i < NUM_BACK_BUFFERS; i++) {
				g_frameContext[i].RenderTargetResource = nullptr;
				g_frameContext[i].CommandAllocator = nullptr;
		}
			g_pd3dCommandList = nullptr;

		
	}
	
}

void ImGuiHook::InitImGui() {
	if (g_ImGuiInitialized) return;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	window = FindWindowA(nullptr, "Minecraft");
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX12_Init(
		g_pd3dDevice.get(), NUM_BACK_BUFFERS,
		DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap.get(),
		g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
		g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart()
	);
	
	g_ImGuiInitialized = true;
}

void ImGuiHook::CleanupImGui() {
	if (!g_ImGuiInitialized) return;
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	g_ImGuiInitialized = false;
}

LRESULT APIENTRY ImGuiHook::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;
	
	return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
}

HRESULT __stdcall ImGuiHook::hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags) {
	static bool init = false;
	if (!init) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)g_pd3dDevice.put()))) {
			g_pSwapChain.attach( pSwapChain);
			DXGI_SWAP_CHAIN_DESC sdesc;
			pSwapChain->GetDesc(&sdesc);
			window = FindWindowA(nullptr, "Minecraft");
			NUM_BACK_BUFFERS = sdesc.BufferCount;

			// sDesc1
			DXGI_SWAP_CHAIN_DESC1 sdesc1;
			pSwapChain->GetDesc1(&sdesc1);

			D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
			rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvDesc.NumDescriptors = NUM_BACK_BUFFERS;
			rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			rtvDesc.NodeMask = 0;
			g_pd3dDevice->CreateDescriptorHeap(
				&rtvDesc,
				__uuidof(ID3D12DescriptorHeap),
				reinterpret_cast<void**>(g_pd3dRtvDescHeap.put())
			);
			
			
			D3D12_DESCRIPTOR_HEAP_DESC srvDesc = {};
			srvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvDesc.NumDescriptors = 1;
			srvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			srvDesc.NodeMask = 0;
			g_pd3dDevice->CreateDescriptorHeap(
				&srvDesc,
				__uuidof(ID3D12DescriptorHeap),
				reinterpret_cast<void**>(g_pd3dSrvDescHeap.put())
			);
			
			
			g_frameContext = new FrameContext[NUM_BACK_BUFFERS]();
			


			CreateRenderTarget();

			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			
			InitImGui();

			// Add fullscreen mode support
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
			pSwapChain->GetDesc1(&swapChainDesc);

			// Enable Alt+Enter and resize support
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc = {};
			fullscreenDesc.Windowed = TRUE;
			fullscreenDesc.RefreshRate.Numerator = 60;
			fullscreenDesc.RefreshRate.Denominator = 1;
			fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			
		}
		init = true;
	}
	 if(g_ResizeWidth != 0 && g_ResizeHeight != 0)
	{
		//WaitForLastSubmittedFrame();
		CleanupRenderTarget();
		g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
		g_ResizeWidth = g_ResizeHeight = 0;
		CreateRenderTarget();
	}
	if (!g_pd3dCommandQueue)
	{
		printf("Failed to create command queue\n");
		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	// ImGui 
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool show_demo_window = true;
	if (GetAsyncKeyState(VK_INSERT) & 1)
		show_demo_window = !show_demo_window;
	if (show_demo_window)
		ImGui::ShowDemoWindow();

	ImGui::Render();


	int backBufferIdx = pSwapChain->GetCurrentBackBufferIndex();
	FrameContext& frameCtx = g_frameContext[backBufferIdx];
	frameCtx.CommandAllocator->Reset();
	
	g_pd3dCommandList->Reset(frameCtx.CommandAllocator.get(), nullptr);
	
	// Barrier: Present -> RenderTarget
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = frameCtx.RenderTargetResource.get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	g_pd3dCommandList->ResourceBarrier(1, &barrier);

	g_pd3dCommandList->OMSetRenderTargets(1, &frameCtx.RenderTargetDescriptor, FALSE, nullptr);
	ID3D12DescriptorHeap* heaps[] = { g_pd3dSrvDescHeap.get()};
	g_pd3dCommandList->SetDescriptorHeaps(1, heaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList.get());

	// Barrier: RenderTarget -> Present
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	g_pd3dCommandList->ResourceBarrier(1, &barrier);

	g_pd3dCommandList->Close();
		g_pd3dCommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&g_pd3dCommandList));
	
	return oPresent(pSwapChain, SyncInterval, Flags);
}

// --- ResizeBuffers Hook ---
HRESULT __stdcall ImGuiHook::hkResizeBuffers(IDXGISwapChain3* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {

	
		ImGui_ImplDX12_InvalidateDeviceObjects();
	

	CleanupRenderTarget();
	if (g_frameContext) { delete[] g_frameContext; g_frameContext = nullptr; }
	if (g_pd3dRtvDescHeap) { g_pd3dRtvDescHeap = nullptr; }

	HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	if (SUCCEEDED(hr)) {
		DXGI_SWAP_CHAIN_DESC sdesc;
		pSwapChain->GetDesc(&sdesc);
		NUM_BACK_BUFFERS = sdesc.BufferCount;
		g_frameContext = new FrameContext[NUM_BACK_BUFFERS]();

		
		D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
		rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDesc.NumDescriptors = NUM_BACK_BUFFERS;
		rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvDesc.NodeMask = 0;
		g_pd3dDevice->CreateDescriptorHeap(
			&rtvDesc,
			__uuidof(ID3D12DescriptorHeap),
			reinterpret_cast<void**>(g_pd3dRtvDescHeap.put())
		);

		CreateRenderTarget();
		if (!g_pd3dCommandQueue) {
			OutputDebugStringA("CommandQueue Unfound\n");
			return hr;
		}

			ImGui_ImplDX12_CreateDeviceObjects();	}
	return hr;
}


HRESULT ImGuiHook::hookPresentD3D11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	static HWND hwnd = nullptr;
	if (!hwnd) {
		DXGI_SWAP_CHAIN_DESC desc;
		pSwapChain->GetDesc(&desc);
		hwnd = desc.OutputWindow;
	}

	if (!d3d11Device) {
		if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&d3d11Device)))
			return oPresent11(pSwapChain, SyncInterval, Flags);
	}
	ID3D11DeviceContext* pContext = nullptr;
	d3d11Device->GetImmediateContext(&pContext);
	if (g_pd3dCommandQueue == nullptr) {
		OutputDebugStringA("CommandQueue Unfound\n");
		return oPresent11(pSwapChain, SyncInterval, Flags);
	}
	if (!initContext) {
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(d3d11Device, pContext);
		initContext = true;
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Ê¾Àý´°¿Ú
	static bool show_demo_window = true;
	if (GetAsyncKeyState(VK_INSERT) & 1)
		show_demo_window = !show_demo_window;
	if (show_demo_window)
		ImGui::ShowDemoWindow();
	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	pContext->Release();
	return oPresent11(pSwapChain, SyncInterval, Flags);
}

HRESULT __stdcall ImGuiHook::hkResizeBuffers11(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{

	ImGui_ImplDX11_InvalidateDeviceObjects();
	
	if (mainRenderTargetView) { mainRenderTargetView->Release(); mainRenderTargetView = nullptr; }

	HRESULT hr = oResizeBuffers11(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	if (SUCCEEDED(hr)) {
		ID3D11Texture2D* pBackBuffer = nullptr;

		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
			if (d3d11Device)
				d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
		}
		if (g_pd3dCommandQueue == nullptr) {
			OutputDebugStringA("CommandQueue Unfound \n");
			return hr;
		}

		ImGui_ImplDX11_CreateDeviceObjects();
	}
	return hr;
}

void ImGuiHook::hkExecuteCommandLists(ID3D12CommandQueue* pCommandQueue, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists)
{
	if(!g_pd3dCommandQueue)
		g_pd3dCommandQueue.attach( pCommandQueue);
	oExecuteCommandLists(pCommandQueue, NumCommandLists, ppCommandLists);
}


bool ImGuiHook::Init() {

	bool isDx11 = false;
	if (kiero::init(kiero::RenderType::D3D12) != kiero::Status::Success) {
		if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
			//logF("Failed to initialize kiero!");
			return false;
		}
		isDx11 = true;
	}

	if (!isDx11) {
		OutputDebugStringA("working D3D12\n");
		kiero::bind(54, (void**)&oExecuteCommandLists, hkExecuteCommandLists);
		kiero::bind(140, (void**)&oPresent, hkPresent);
		kiero::bind(145, (void**)&oResizeBuffers, hkResizeBuffers); // Index 145 for DXGI 1.4/D3D12 ResizeBuffers
		OutputDebugStringA("bind success\n");
	}
	else {
		OutputDebugStringA("working D3D11\n");
		kiero::bind(8, (void**)&oPresent11, hookPresentD3D11);
		kiero::bind(13, (void**)&oResizeBuffers11, hkResizeBuffers11); // Index 13 for DXGI/D3D11 ResizeBuffers
		OutputDebugStringA("bind success\n");
	}
	return true;
}

void ImGuiHook::Shutdown() {

CleanupImGui();
CleanupRenderTarget();
if (g_frameContext) { delete[] g_frameContext; g_frameContext = nullptr; }
g_pd3dRtvDescHeap = nullptr;
g_pd3dSrvDescHeap = nullptr;
g_pd3dDevice = nullptr;
g_pd3dCommandList = nullptr;
g_pd3dCommandQueue = nullptr;
if (oWndProc && window) {
    SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oWndProc);
    oWndProc = nullptr;
}
window = nullptr;
g_pSwapChain = nullptr;
NUM_BACK_BUFFERS = 0;
g_ImGuiInitialized = false;
}