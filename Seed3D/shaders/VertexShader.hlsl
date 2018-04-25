cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

struct VertexData
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 texture_coord : TEXCOORD0;
};

struct PixelData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texture_coord : TEXCOORD0;
};

PixelData vertexMain(VertexData vertex_input)
{
	PixelData pixel_output;

	vertex_input.position.w = 1.0f;
	pixel_output.position = mul(vertex_input.position, world_matrix);
	pixel_output.position = mul(pixel_output.position, view_matrix);
	pixel_output.position = mul(pixel_output.position, projection_matrix);

	pixel_output.color = vertex_input.color;
	pixel_output.texture_coord = vertex_input.texture_coord;

	return pixel_output;
}