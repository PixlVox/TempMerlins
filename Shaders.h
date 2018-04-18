#ifndef SHADERS_H
#define SHADERS_h
#include <DirectXMath.h>
#include <memory>
#include <CommonStates.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Microsoft>
class Shaders
{
private:
	enum SHADER { Basic, Glowing }; //Till exempel
	DirectX::XMMATRIX wMatrix;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout; //Eller en egen input layout

	ID3D11Buffer* vBuffer;
	ID3D11Buffer* iBuffer;

public:	
	Shaders(DirectX::XMMATRIX in_wMatrix, SHADER whatShader); //Inkluderar 
	~Shaders();

	void setShader(SHADER);
};

#endif
