#define MAX_NUM_OF_DIRECTIONAL_LIGHTS 4

struct BaseLight
{
    float3 color;
    float strength;
};

struct DirectionalLight
{
    BaseLight base;
    float3 direction;
};