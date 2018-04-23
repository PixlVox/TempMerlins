#ifndef SHADERS_H
#define SHADERS_h
#include <DirectXMath.h>
#include <memory>
#include <CommonStates.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <iostream>
#include <wrl\client.h>
#include <vector>
#include <SimpleMath.h>

enum SHADER {
	VertexColor,
	VertexNormal,
	VertexNormalTexture
};

class Shaders
{
private:
	
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDeviceContext;

	std::unique_ptr<DirectX::BasicEffect> m_effect;	
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout; 	

	ID3D11Buffer* vBuffer;
	ID3D11Buffer* iBuffer;

	void createVertexPositionColorShader();
	void createVertexPositionNormalShader();
	void createVertexPositionNormalTextureShader();

public:	
	Shaders(); 
	~Shaders();

	void init(ID3D11Device* in_gDevice, ID3D11DeviceContext* in_gDeviceContext);
	void createShader(SHADER whatShader);
	void setBuffers(ID3D11Buffer* in_vBuffer, ID3D11Buffer* in_iBuffer);
	void setShaderParams(DirectX::XMMATRIX in_WMatrix, DirectX::SimpleMath::Vector3 in_SpecColor, DirectX::SimpleMath::Vector3 in_DiffuseColor,
		DirectX::SimpleMath::Vector3 in_Emissive, float in_SpecExponent, float in_Opacity);
	void setCameraParams(DirectX::XMMATRIX in_Proj, DirectX::XMMATRIX in_View);
	void setShaderLightParams(DirectX::SimpleMath::Vector3 in_Ambient, int in_WhichLight, DirectX::XMVECTOR in_LightDirection,
		DirectX::XMVECTOR in_LightDiffuseColor, DirectX::XMVECTOR in_LightSpecularColor);
	void setShaderFogParams(DirectX::XMVECTOR in_FogColor);
	void setTexture(ID3D11ShaderResourceView*  in_Tex);
	//void disableSpecular();
	void setAlpha(float in_Alpha);
	void setLightingEnabled(bool in_Value);
	void setSpecificLightEnabled(int in_WhichLight, bool in_Value);
	void setLightDirection(int in_WhichLight, DirectX::XMVECTOR in_Direction);

};

#endif
