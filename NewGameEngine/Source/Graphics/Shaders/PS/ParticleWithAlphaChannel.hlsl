cbuffer CBuf
{
    float4 color;
};

float4 main() : SV_Target
{
    //float4 newColor = {color.x,color.y,color.z,1.0f };
    return normalize(color);
}
