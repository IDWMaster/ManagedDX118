#pragma once
#include <d3d11.h>
using namespace Windows::Foundation;
namespace DXInteropLib
{
	public delegate void RenderLoopArgs();
 public ref class DirectContext {
 public:
	 
	 DirectContext(void* sender, void* context);
	 void* CreateVertexShader(array<byte>^ vertfile, array<byte>^ pixfile);
	 static DirectContext^ getDefaultContext();
	 void ApplyVertexShader(void* input);
	 ///<summary>Registers a global loop handler for each frame</summary>
	 void RegisterLoopHandler(RenderLoopArgs^ args);
	 void CallLoopHandler();
	 void* CreateTexture2D(array<byte>^ texture, int width, int height);
 private:
	 RenderLoopArgs^ loophandler;
	 Microsoft::WRL::ComPtr<ID3D11DeviceContext> underlyingcontext;
	 Microsoft::WRL::ComPtr<ID3D11Device> underlyingDevicePtr;
	static DirectContext^ currentcontext;

	};
 class DXShader {
	public:
		DXShader(Microsoft::WRL::ComPtr<ID3D11Device> device, array<byte>^ vertfile, array<byte>^ pixfile);
	 Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	 Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelshader;
	};
 
// WinRTComponent.h
}