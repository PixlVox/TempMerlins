cbuffer EXAMPLE_BUFFER : register(b0) {

	Matrix world;
	Matrix view;
	Matrix projection;

};

struct GS_IN {

	float4 pos : POSITION;
	float3 norm : NORMAL;

};

struct GS_OUT {

	float4 pos : SV_POSITION;
	float4 posWorld : POSITION;
	float3 norm : NORMAL;

};

[maxvertexcount(3)]
void GS_main(triangle GS_IN input[3], inout TriangleStream<GS_OUT> outputStream){

	GS_OUT output = (GS_OUT)0;

	//Store values sent from VS, apply  and pass them to the stream
	for (int i = 0; i < 3; i++){

		//Apply transformations
		input[i].pos = mul(world, input[i].pos);
		output.posWorld = input[i].pos;
		input[i].pos = mul(view, input[i].pos);
		input[i].pos = mul(projection, input[i].pos);

		//Copy over values
		output.pos = input[i].pos;
		output.norm = input[i].norm;

		//Send values to stream
		outputStream.Append(output);

	}

	outputStream.RestartStrip();

}