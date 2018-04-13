struct VS_IN{

	float3 pos : POSITION;
	float3 norm : NORMAL;

};

struct VS_OUT{

	float4 pos : POSITION;
	float3 norm : NORMAL;

};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------

VS_OUT VS_main(VS_IN input){

	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.0f);
	output.norm = input.norm;

	return output;

}