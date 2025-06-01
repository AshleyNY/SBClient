#include <winrt/base.h>
using namespace winrt;
#include "../../Libs/Imgui/imgui.h"
#include "../../Libs/Imgui/imgui_impl_win32.h"
#include "../../Libs/Imgui/imgui_impl_dx12.h"
#include "../../Libs/Imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <cstdint>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dwrite.lib")
#include "../../../../Libs/Kiero/kiero.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
class ImGuiHook {
protected:
//dx11Hook
	typedef HRESULT(__stdcall* PresentD3D11)(IDXGISwapChain*, UINT, UINT);
	static inline PresentD3D11 oPresent11;
	static inline ID3D11Device* d3d11Device = nullptr;

	static inline enum Device {
		INVALID_DEVICE_TYPE,
		D3D11,
		D3D12
	} deviceType = INVALID_DEVICE_TYPE;;

	//dx12
	struct FrameContext {
		winrt::com_ptr<ID3D12CommandAllocator> CommandAllocator;
		 winrt::com_ptr<ID3D12Resource> RenderTargetResource;
		 D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetDescriptor = {};
	};

	static HWND window;
	static WNDPROC oWndProc;

	//init smth
	static winrt::com_ptr<IDXGISwapChain3> g_pSwapChain;
	static winrt::com_ptr<ID3D12Device> g_pd3dDevice;
	static winrt::com_ptr<ID3D12DescriptorHeap> g_pd3dRtvDescHeap;
	static winrt::com_ptr<ID3D12DescriptorHeap> g_pd3dSrvDescHeap;
	static winrt::com_ptr<ID3D12CommandQueue> g_pd3dCommandQueue;
	static winrt::com_ptr<ID3D12GraphicsCommandList> g_pd3dCommandList;

	static FrameContext* g_frameContext;
	static int NUM_BACK_BUFFERS;
	static bool g_ImGuiInitialized;


	static inline bool initContext = false;
public:
	
	 static  HRESULT hookPresentD3D11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	 static void hkExecuteCommandLists(ID3D12CommandQueue* pCommandQueue, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists);

	/* static HRESULT hkSignal(ID3D12CommandQueue* queue, ID3D12Fence* fence, UINT64 value);*/


	
	 static bool Init();
	 static void Shutdown();

	 // DX12 Present/ResizeBuffers Hook
	 static void CreateRenderTarget();
	 static void CleanupRenderTarget();
	 static void InitImGui();
	 static void CleanupImGui();

	 static LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	 static void HookDX12();
	 static HRESULT __stdcall hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
	 static HRESULT __stdcall hkResizeBuffers(IDXGISwapChain3* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	 // 在 ImGuiHook.h 里加上声明
	 static HRESULT __stdcall hkResizeBuffers11(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
};

