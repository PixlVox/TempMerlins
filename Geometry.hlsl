cbuffer EXAMPLE_BUFFER : register(b0) {

	Matrix world;
	Matrix view;
	Matrix projection;

};

struct GS_IN {

	float4 pos : POSITION;
	float3 col : COLOR;

};

struct GS_OUT {

	float4 pos : SV_POSITION;
	float4 posWorld : POSITION;
	float2 col : COLOR;
	float3 normal : NORMAL;

};

[maxvertexcount(3)]
void GS_main(triangle GS_IN input[3], inout TriangleStream<GS_OUT> outputStream){

	GS_OUT output = (GS_OUT)0;

	//Calculate normal
	float3 normal = float3(0.0f, 0.0f, 0.0f);
	float4 v1 = input[1].pos - input[0].pos;
	float4 v2 = input[2].pos - input[0].pos;
	normal = normalize(mul(world, cross(v1.xyz, v2.xyz)));
	output.normal = normal;

	//Store values sent from VS, apply  and pass them to the stream
	for (int i = 0; i < 3; i++){

		//Apply transformations
		input[i].pos = mul(world, input[i].pos);
		output.posWorld = input[i].pos;
		input[i].pos = mul(view, input[i].pos);
		input[i].pos = mul(projection, input[i].pos);

		//Copy over values
		output.pos = input[i].pos;
		output.col = input[i].col;

		//Send values to stream
		outputStream.Append(output);

	}

	outputStream.RestartStrip();

}