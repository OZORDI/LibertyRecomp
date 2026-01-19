// Fullscreen Triangle Vertex Shader for Post-Processing
// Generates a fullscreen triangle from vertex ID (0, 1, 2)

struct VSOutput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

VSOutput shaderMain(uint vertexID : SV_VertexID)
{
    VSOutput output;
    
    // Generate fullscreen triangle vertices
    // VertexID 0: (-1, -1), VertexID 1: (3, -1), VertexID 2: (-1, 3)
    output.texCoord = float2((vertexID << 1) & 2, vertexID & 2);
    output.position = float4(output.texCoord * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);
    
    return output;
}
