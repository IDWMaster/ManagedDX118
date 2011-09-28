// WinRTComponent.cpp

#include "pch.h"
#include "WinRTComponent.h"
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
	void* DirectContext::CreateVertexBuffer(array<byte>^ data, int scan) {
		D3D11_BUFFER_DESC description;
		memset(&description,0,sizeof(description));
		description.ByteWidth = data->Length;
		description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		description.MiscFlags = 0;
		description.Usage = D3D11_USAGE_DEFAULT;
		description.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA mdat;
		mdat.pSysMem = data->Data;
		mdat.SysMemPitch = 0;
		mdat.SysMemSlicePitch = 0;
		ID3D11Buffer* output;
		
		HRESULT dbg = underlyingDevicePtr->CreateBuffer(&description,&mdat,&output);
		return output;
		
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
		description.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		description.ArraySize = 1;
		description.MipLevels = 1;
		description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		D3D11_SUBRESOURCE_DATA mdat;
		memset(&mdat,0,sizeof(mdat));
		description.SampleDesc.Count = 1;
		//4 bytes per pixel
		mdat.SysMemPitch = 4*width;
		byte* rdata = data->Data;
		mdat.pSysMem = rdata;
		
		const D3D11_SUBRESOURCE_DATA tdat[1] = {mdat};
		ID3D11Texture2D* texture;
		HRESULT mresult = underlyingDevicePtr->CreateTexture2D(&description,tdat,&texture);
		return texture;
	};
	void DirectContext::ApplyTexture(void* sender) {
	D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDescription;
	memset(&textureViewDescription,0,sizeof(textureViewDescription));
	textureViewDescription.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	textureViewDescription.Texture2D.MipLevels = 1;
	textureViewDescription.Texture2D.MostDetailedMip = 0;
	textureViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> realview;
	ID3D11Texture2D* texture = (ID3D11Texture2D*)sender;
	underlyingDevicePtr->CreateShaderResourceView(texture,&textureViewDescription,&realview);
	underlyingcontext->PSSetShaderResources(0,1,realview.GetAddressOf());
	};
	void DirectContext::InitializeInputLayout(array<byte>^ shader) {
	D3D11_INPUT_ELEMENT_DESC* VertexPositionNormalTexture = new D3D11_INPUT_ELEMENT_DESC[3];
	D3D11_INPUT_ELEMENT_DESC elem0;
	elem0.SemanticName = "POSITION";
	elem0.SemanticIndex = 0;
	//This MIGHT work
	elem0.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elem0.InputSlot = 0;
	elem0.AlignedByteOffset = 0;
	elem0.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elem0.InstanceDataStepRate = 0;
	VertexPositionNormalTexture[0] = elem0;
	D3D11_INPUT_ELEMENT_DESC elem1;
	elem1.SemanticName = "NORMAL";
	elem1.SemanticIndex = 0;
	//This MIGHT work
	elem1.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elem1.InputSlot = 0;
	//Should eqaul 12
	elem1.AlignedByteOffset = sizeof(float)*3;
	elem1.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elem1.InstanceDataStepRate = 0;
	VertexPositionNormalTexture[1] = elem1;
	D3D11_INPUT_ELEMENT_DESC elem2;
	elem2.SemanticName = "TEXCOORD";
	elem2.SemanticIndex = 0;
	//This MIGHT work
	elem2.Format = DXGI_FORMAT_R32G32_FLOAT;
	elem2.InputSlot = 0;
	//Should eqaul 24
	elem2.AlignedByteOffset = (sizeof(float)*3)+(sizeof(float)*3);
	elem2.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elem2.InstanceDataStepRate = 0;
	VertexPositionNormalTexture[2] = elem2;

	ID3D11InputLayout* layout;
	underlyingDevicePtr->CreateInputLayout(VertexPositionNormalTexture,3,shader->Data,shader->Length,&layout);
	if(layout == NULL) {
	throw "Error creating layout!";
	}
	underlyingcontext->IASetInputLayout(layout);
	//SHOULD: Set sampler states. Might not work if pixel shader hasn't been set yet though.....









	D3D11_SAMPLER_DESC samplerDescription;
	memset(&samplerDescription,0,sizeof(samplerDescription));
	samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescription.MaxAnisotropy = 0;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescription.BorderColor[0] = 0.0f;
    samplerDescription.BorderColor[1] = 0.0f;
    samplerDescription.BorderColor[2] = 0.0f;
    samplerDescription.BorderColor[3] = 0.0f;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	underlyingDevicePtr->CreateSamplerState(&samplerDescription,&sampler);
	underlyingcontext->PSSetSamplers(0,1,sampler.GetAddressOf());
	};
	void DirectContext::Draw(int vertcount) {
		underlyingcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		underlyingcontext->Draw(vertcount,0);
	};
	void DirectContext::BindVertexBuffer(void* input, int stride) {
	ID3D11Buffer* buffer = (ID3D11Buffer*)input;
	ID3D11Buffer** abuffer = new ID3D11Buffer*[1];
	abuffer[0] = buffer;
	
	UINT* offsets = new UINT[1];
	offsets[0] = 0;
	
	UINT* strides = new UINT[1];
	strides[0] = stride;
	
	underlyingcontext->IASetVertexBuffers(0,1,abuffer,strides,offsets);
	delete strides;
	delete offsets;
	};
}