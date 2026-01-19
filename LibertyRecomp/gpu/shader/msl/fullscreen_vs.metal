// Fullscreen Triangle Vertex Shader for Post-Processing (Metal)
#include <metal_stdlib>
using namespace metal;

struct VSOutput {
    float4 position [[position]];
    float2 texCoord;
};

vertex VSOutput shaderMain(uint vertexID [[vertex_id]]) {
    VSOutput output;
    
    // Generate fullscreen triangle vertices
    // VertexID 0: (-1, -1), VertexID 1: (3, -1), VertexID 2: (-1, 3)
    output.texCoord = float2((vertexID << 1) & 2, vertexID & 2);
    output.position = float4(output.texCoord * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);
    
    return output;
}
