#version 450 core

/*layout(std430, binding = 0) restrict readonly buffer VertexBlock {
    float vertexData[];
};

struct Instance
{
    mat4    modelMatrix_T;
    vec3    bbox_min;
    uint    meshID;
    vec3    bbox_max;
    uint    materialID;
};

layout(std430, binding = INSTANCE_BINDING) restrict readonly buffer InstanceBlock {
    int         numInstances;
    Instance    instances[];
};

out VertexData
{
    vec3 wpos;
    vec3 viewdir;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
    flat uint materialID;
} outData;*/

uniform vec3[3] vertices;

void main() {
    /*const uint instanceID = gl_BaseInstanceARB + gl_InstanceID;
    const mat4 modelMatrix_T = instances[instanceID].modelMatrix_T;

    uint idx = meshes[meshID].first + gl_VertexID * meshes[meshID].stride;

    vec3 value;
    value.x = vertexData[idx++];
    value.y = vertexData[idx++];
    value.z = vertexData[idx++];

    const vec4 worldPos = vec4(value, 1.0) * modelMatrix_T;
    gl_Position =  worldPos * cam.ProjViewMatrix_T;*/
    gl_Position = vec4(vertices[gl_VertexID], 1.0);
}
