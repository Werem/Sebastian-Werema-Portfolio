struct vsInput{
	float4 pos : POSITION;
    float3 uv : TEXCOORD;
};

struct psInput{
	float4 pos : SV_POSITION;
    float3 uv : TEXCOORD;
};

psInput VSMain(vsInput v){
	psInput o;
    o.pos = v.pos;
    o.uv = v.uv;
	return o;
}