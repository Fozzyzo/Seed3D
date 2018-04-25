Texture2D texture2d;
SamplerState sample_type;

struct PixelData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texture_coord : TEXCOORD0;
};

float4 pixelMain(PixelData pixel_input) : SV_TARGET
{
	float4 texture_color;
	texture_color = texture2d.Sample(sample_type, pixel_input.texture_coord);
	
	pixel_input.color = pixel_input.color + texture_color;
	return pixel_input.color;
}