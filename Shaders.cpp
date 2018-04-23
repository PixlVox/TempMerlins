#include "Shaders.h"

void Shaders::createVertexPositionColorShader()
{
	UINT32 vertexSize = sizeof(float) * DirectX::VertexPositionColor::InputElementCount;
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->vBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetIndexBuffer(this->iBuffer, DXGI_FORMAT_R32_UINT, offset);

	 //vet inte om det funkar
	
	this->m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	this->m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	HRESULT hr = 0;

	hr = this->gDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.ReleaseAndGetAddressOf());
	
	if (FAILED(hr))
	{
		throw std::runtime_error("ERROR: Could not create input layout for VertexPositionColor shader.");
	}

	this->m_effect->Apply(this->gDeviceContext);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->IASetInputLayout(m_inputLayout.Get());
	
}

void Shaders::createVertexPositionNormalShader()
{
	UINT32 vertexSize = sizeof(float) * DirectX::VertexPositionNormal::InputElementCount;
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->vBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetIndexBuffer(this->iBuffer, DXGI_FORMAT_R32_UINT, offset);

	this->m_effect->SetVertexColorEnabled(true); //kanske ska vara avstängd här, förstår inte riktigt vad den gör.

	void const* shaderByteCode;
	size_t byteCodeLength;

	this->m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	HRESULT hr = 0;

	hr = this->gDevice->CreateInputLayout(DirectX::VertexPositionNormal::InputElements, DirectX::VertexPositionNormal::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.ReleaseAndGetAddressOf());

	if (FAILED(hr))
	{
		throw std::runtime_error("ERROR: Could not create input layout for VertexPositionNormal shader.");
	}

	this->m_effect->Apply(this->gDeviceContext);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->IASetInputLayout(m_inputLayout.Get());

}

void Shaders::createVertexPositionNormalTextureShader()
{
	UINT32 vertexSize = sizeof(float) * DirectX::VertexPositionNormalTexture::InputElementCount;
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->vBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetIndexBuffer(this->iBuffer, DXGI_FORMAT_R32_UINT, offset);

	//this->m_effect->SetVertexColorEnabled(true); //kanske ska vara avstängd här, förstår inte riktigt vad den gör.

	void const* shaderByteCode;
	size_t byteCodeLength;

	this->m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	HRESULT hr = 0;

	hr = this->gDevice->CreateInputLayout(DirectX::VertexPositionNormalTexture::InputElements, DirectX::VertexPositionNormalTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.ReleaseAndGetAddressOf());

	if (FAILED(hr))
	{
		throw std::runtime_error("ERROR: Could not create input layout for VertexPostionNormalTexture shader.");
	}

	this->m_effect->Apply(this->gDeviceContext);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->IASetInputLayout(m_inputLayout.Get());
}

Shaders::Shaders()
{
	
}

Shaders::~Shaders()
{	
}

void Shaders::init(ID3D11Device* in_gDevice, ID3D11DeviceContext* in_gDeviceContext) {

	this->gDevice = in_gDevice;
	this->gDeviceContext = in_gDeviceContext;
	this->m_effect = std::make_unique<DirectX::BasicEffect>(this->gDevice);

}

void Shaders::createShader(SHADER whatShader)
{
	switch (whatShader)
	{
	case VertexColor:
		try
		{
			this->createVertexPositionColorShader();
		}
		catch (std::exception& e)
		{
			//OutputDebugString(e.what());
		}
		break;
	
	case VertexNormal:
		try
		{
			this->createVertexPositionNormalShader();
		}
		catch (std::exception& e)
		{
			//OutputDebugString(e.what());
		}
		break;

	case VertexNormalTexture:
		try
		{
			this->createVertexPositionNormalTextureShader();
		}
		catch (std::exception& e)
		{
			//OutputDebugString(e.what());
		}
		break;
	}
}

void Shaders::setBuffers(ID3D11Buffer* in_vBuffer, ID3D11Buffer* in_iBuffer)
{
	this->vBuffer = in_vBuffer;
	this->iBuffer = in_iBuffer;
}
void Shaders::setShaderParams(DirectX::XMMATRIX in_WMatrix, DirectX::SimpleMath::Vector3 in_SpecColor, DirectX::SimpleMath::Vector3 in_DiffuseColor,
	DirectX::SimpleMath::Vector3 in_Emissive, float in_SpecExponent, float in_Opacity)
{
	this->m_effect->SetWorld(in_WMatrix);	
	this->m_effect->SetSpecularColor(in_SpecColor);
	this->m_effect->SetDiffuseColor(in_DiffuseColor);	
	this->m_effect->SetEmissiveColor(in_Emissive);

	if (in_SpecExponent == 0)
	{
		this->m_effect->SetSpecularPower(1);
	}
	else
	{
		this->m_effect->SetSpecularPower(in_SpecExponent);
	}	
	//Opacity?
}

void Shaders::setCameraParams(DirectX::XMMATRIX in_Proj, DirectX::XMMATRIX in_View)
{
	this->m_effect->SetProjection(in_Proj);
	this->m_effect->SetView(in_View);	
}

void Shaders::setShaderLightParams(DirectX::SimpleMath::Vector3 in_Ambient, int in_WhichLight, DirectX::XMVECTOR in_LightDirection,
	DirectX::XMVECTOR in_LightDiffuseColor, DirectX::XMVECTOR in_LightSpecularColor)
{
	this->m_effect->SetAmbientLightColor(in_Ambient);
	this->m_effect->SetLightDirection(in_WhichLight, in_LightDirection);
	this->m_effect->SetLightDiffuseColor(in_WhichLight, in_LightDiffuseColor);
	this->m_effect->SetLightSpecularColor(in_WhichLight, in_LightSpecularColor);
}

void Shaders::setShaderFogParams(DirectX::XMVECTOR in_FogColor)
{
	this->m_effect->SetFogEnabled(true);
	this->m_effect->SetFogColor(in_FogColor);	
}

void Shaders::setTexture(ID3D11ShaderResourceView* in_Tex)
{
	this->m_effect->SetTextureEnabled(true);
	this->m_effect->SetTexture(in_Tex);
}

void Shaders::setAlpha(float in_Alpha)
{
	this->m_effect->SetAlpha(in_Alpha);
}

void Shaders::setLightingEnabled(bool in_Value)
{
	this->m_effect->SetLightingEnabled(in_Value);
}

void Shaders::setSpecificLightEnabled(int in_WhichLight, bool in_Value)
{
	this->m_effect->SetLightEnabled(in_WhichLight, in_Value);
}

void Shaders::setLightDirection(int in_WhichLight, DirectX::XMVECTOR in_Direction)
{
	this->m_effect->SetLightDirection(in_WhichLight, in_Direction);
}


