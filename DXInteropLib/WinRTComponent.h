#pragma once
#include <d3d11.h>
#include "BasicMath.h"
#include "BasicCamera.h"
#include "matdata.h"
#ifndef WinRTComponent_H
#define WinRTComponent_H
using namespace Windows::Foundation;
namespace DXInteropLib
{
	public delegate void RenderLoopArgs();
	
	public ref class DirectContext sealed {
 public:
	 
	 DirectContext(void* sender, void* context, float width, float height);
	 void* CreateVertexShader(array<byte>^ vertfile, array<byte>^ pixfile);
	 static DirectContext^ getDefaultContext();
	 void ApplyVertexShader(void* input);
	 ///<summary>Registers a global loop handler for each frame</summary>
	 void RegisterLoopHandler(RenderLoopArgs^ args);
	 void CallLoopHandler();
	 void* CreateTexture2D(array<byte>^ texture, int width, int height);
	 void* CreateVertexBuffer(array<byte>^ data, int scan);
	 void BindVertexBuffer(void* vertbuffer, int stride);
	 void Draw(int vertcount);
	 void InitializeInputLayout(array<byte>^ shaderbytecode);
	 void ApplyTexture(void* texture);
	 ///<summary>Creates a constant buffer with the specified data and size</summary>
	 void* CreateConstantBuffer(void* data, UINT size);
	 void* CreateMatrix(bool withcamera);
	 void UpdateMatrixCamera(void* matrixptr,array<float>^ position, array<float>^ lookat);
	 
 private:
	float _width;
	float _height;
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
 class Matrix  {
public:
	void Initialize();
	//Creates a default camera defining the view and projection matrices
	static Matrix CreateDefaultCamera(float width, float height);
	///<summary>Uploads this matrix to the GPU</summary>
	void* UploadMatrix(DirectContext^ context);
	void SetCameraProperties(array<float>^ cameraposition, array<float>^ lookat);
	
private:
	
	///A pointer to the underlying buffer on the GPU
	void* gpubuffer;
	bool hasModelViewProjection;
	///A single matrix. Used only if hasModelViewProjection is set to FALSE.
	float4x4 singlematrix;
	BasicCamera^ underlyingcamera;
	GPUMatrixData underlyingmatrices;
};
// WinRTComponent.h
}
#endif