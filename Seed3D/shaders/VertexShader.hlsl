cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

cbuffer CameraBuffer
{
	float3 camera_position;
	float padding;
};

struct VertexData
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 texture_coord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texture_coord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 view_direction : TEXCOORD1;
};

PixelData vertexMain(VertexData vertex_input)
{
	PixelData pixel_output;
	float4 vertex_position;

	vertex_input.position.w = 1.0f;
	pixel_output.position = mul(vertex_input.position, world_matrix);
	pixel_output.position = mul(pixel_output.position, view_matrix);
	pixel_output.position = mul(pixel_output.position, projection_matrix);

	pixel_output.color = vertex_input.color;
	pixel_output.texture_coord = vertex_input.texture_coord;

	pixel_output.normal = mul(vertex_input.normal, (float3x3)world_matrix);
	pixel_output.normal = normalize(pixel_output.normal);

	vertex_position = mul(vertex_input.position, world_matrix);
	pixel_output.view_direction = camera_position.xyz - vertex_position.xyz;
	pixel_output.view_direction = normalize(pixel_output.view_direction);

	return pixel_output;
}