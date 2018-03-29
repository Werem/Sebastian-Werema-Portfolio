cbuffer MyBuffer{
	float4x4 mvp;
};

struct vsInput{
	float4 pos : POSITION;
	float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct psInput{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
    float2 uv : TEXCOORD;
};

psInput VSMain(vsInput v){
	psInput o;
    o.pos = mul(v.pos, mvp);
	o.color = v.color;
    o.uv = v.uv;
	return o;
}