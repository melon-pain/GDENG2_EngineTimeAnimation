
struct PS_INPUT
{
	float4 position: SV_POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};

cbuffer constant: register(b0)
{
	//float m_angle;
    float m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(lerp(input.color, input.color1, (sin(m_time * 1.57f) + 1.0f) / 2.0f), 1.0f);
}