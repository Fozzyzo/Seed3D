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
};

struct PixelData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelData vertexMain(VertexData vertex_input)
{
	PixelData pixel_output;

	vertex_input.position.w = 1.0f;
	pixel_output.position = mul(vertex_input.position, world_matrix);
	pixel_output.position = mul(vertex_input.position, view_matrix);
	pixel_output.position = mul(vertex_input.position, projection_matrix);

	pixel_output.color = vertex_input.color;

	return pixel_output;
}