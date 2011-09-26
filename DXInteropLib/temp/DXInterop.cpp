// WinRTComponent.cpp

#include "pch.h"
#include "DXInterop.h"
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <d3dcommon.h>
using namespace DXInteropLib;
namespace DXInteropLib {
	//Context
	DirectContext::DirectContext(void* sender, void* context) {
        //Device initialization
		
		auto currentinstance = this;
		
		ID3D11Device** device = (ID3D11Device**)sender;
		ID3D11DeviceContext** con = (ID3D11DeviceContext**)context;
		underlyingDevicePtr = *device;
		underlyingcontext = *con;
		
		currentcontext = this;
		//End device initialization
		
	};
	void tester() {
		
	};
	DirectContext^ DirectContext::getDefaultContext() {
		return currentcontext;
	};
	//Shader
	char* W2A(wchar_t* origstr, int len) {
	char* fstring = new char[len*12];

		wcstombs(fstring,origstr,len*12);
		return fstring;
	};
	DXShader::DXShader(Microsoft::WRL::ComPtr<ID3D11Device> device, array<byte>^ vertshader, array<byte>^ filenamepixel) {
		SIZE_T len = vertshader->Length;
		byte* data = vertshader->Data;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vshader;
		
		HRESULT dbgresult = device->CreateVertexShader(data,len,NULL,&vshader);
		
		shader = vshader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pshader;
		data = filenamepixel->Data;
		len = filenamepixel->Length;
		dbgresult = device->CreatePixelShader(data,len,NULL,&pshader);
		pixelshader = pshader;
		
	};
	void* DirectContext::CreateVertexShader(array<byte>^ vertshader, array<byte>^ pixshader) {
		DXShader* mshader = new DXShader(underlyingDevicePtr,vertshader,pixshader);
		
		if(mshader->pixelshader !=NULL & mshader->shader !=NULL) {
		return mshader;
		}else {
		return NULL;
		}
	};
	void DirectContext::ApplyVertexShader(void* shadypointer) {
		DXShader* mshader = (DXShader*)shadypointer;
		ID3D11VertexShader* realshader = mshader->shader.Get();
		underlyingcontext->VSSetShader(mshader->shader.Get(),NULL,0);
		underlyingcontext->PSSetShader(mshader->pixelshader.Get(),NULL,0);
	};
	void DirectContext::RegisterLoopHandler(RenderLoopArgs^ args) {
		loophandler = args;
	};
	void DirectContext::CallLoopHandler() {
		if(loophandler !=nullptr) {
			loophandler();
		}
	};
	void* DirectContext::CreateTexture2D(array<byte>^ data, int width, int height) {
		D3D11_TEXTURE2D_DESC description;
		//Zero memory
		memset(&description,0,sizeof(description));
		description.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		description.Width = (UINT)width;
		description.Height = (UINT)height;
		description.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_TYPELESS;
		description.ArraySize = 1;
		description.MipLevels = 1;
		D3D11_SUBRESOURCE_DATA mdat;
		memset(&mdat,0,sizeof(mdat));
		description.SampleDesc.Count = 1;
		mdat.SysMemPitch = 4*width;
		byte* rdata = data->Data;
		mdat.pSysMem = rdata;
	
		const D3D11_SUBRESOURCE_DATA tdat[1] = {mdat};
		ID3D11Texture2D* texture;
		HRESULT mresult = underlyingDevicePtr->CreateTexture2D(&description,tdat,&texture);
		return texture;
	};
}