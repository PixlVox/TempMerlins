
cbuffer LIGHT_BUFFER : register(b0) {

	float4 lightPos_Intensity;
	float4 lightColor;

};

struct PS_IN {

	float4 pos : SV_POSITION;
	float4 posWorld : POSITION;
	float3 col : COLOR;
	float3 normal : NORMAL;

};

struct PS_OUT{

	float4 col : COLOR;

};

PS_OUT PS_main(PS_IN input) : SV_Target{
	
	PS_OUT output = (PS_OUT)0;

	//Final Color
	output.col = float4(input.col, 1.0f);

	//Ambient
	float4 ambient = float4(0.15f, 0.15f, 0.15f, 1.0f);
	output.col *= ambient;

	//Diffuse factor
	float diffuse = max((dot(input.normal, normalize(lightPos_Intensity.xyz - input.posWorld.xyz))), 0.0f);

	//Diffuse: objectColor * lightColor * Diffuse * lightIntensity * (1/distancePointToLight)
	float4 color = float4(output.col.xyz * lightColor * diffuse * lightPos_Intensity.w * (lightPos_Intensity.w / distance(input.posWorld.xyz, lightPos_Intensity.xyz)), 1.0f);
	output.col += color;
	output.col = min(output.col, float4(1.0f, 1.0f, 1.0f, 1.0f));

	return output;

};