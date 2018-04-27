Texture2D texture2d;
SamplerState sample_type;

cbuffer LightBuffer
{
	float4 ambient_light;
	float4 diffuse_color;
	float3 light_direction;
	float specular_strength;
	float4 specular_color;
};

struct PixelData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texture_coord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 view_direction : TEXCOORD1;
};

float4 pixelMain(PixelData pixel_input) : SV_TARGET
{
	float4 texture_color;
	float3 light_dir;
	float light_intensity;
	float3 reflection;
	float4 specular;

	texture_color = texture2d.Sample(sample_type, pixel_input.texture_coord);
	
	pixel_input.color += ambient_light;

	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	light_dir = -light_direction;
	light_intensity = saturate(dot(pixel_input.normal, light_direction));
	
	if (light_intensity > 0.0f)
	{
		pixel_input.color += (diffuse_color * light_intensity);
		pixel_input.color = saturate(pixel_input.color);
		
		reflection = normalize(2 * light_intensity * pixel_input.normal - light_direction);

		specular = pow(saturate(dot(reflection, pixel_input.view_direction)), specular_strength);
	}

	pixel_input.color = pixel_input.color * texture_color;
	pixel_input.color = saturate(pixel_input.color + specular);

	return pixel_input.color;
}