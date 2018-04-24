struct PixelData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 pixelMain(PixelData pixel_input) : SV_TARGET
{
	return pixel_input.color;
}