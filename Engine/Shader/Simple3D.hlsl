// テクスチャとサンプラーデータのグローバル変数定義
Texture2D g_texture : register(t0);     // テクスチャー
SamplerState g_sampler : register(s0);  // サンプラー

// コンスタントバッファ
// DirectX側から送信されてくる、ポリゴン頂点以外の諸情報の定義
cbuffer global
{
    float4x4 matWVP;        // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matNormal;     // 法線行列
    float4 diffuseColor;    // ディフューズカラー( マテリアルの色 )
    bool isUseTexture;      // テクスチャを使用するならtrue
};

// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
struct VS_OUT
{
    float4 pos : SV_POSITION; // 位置
    float2 uv : TEXCOORD; // UV座標
    float4 color : COLOR; // 色( 明るさ )
};

// 頂点シェーダー
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
    // ピクセルシェーダーへ渡す情報
    VS_OUT outData;
    
    outData.pos = mul(pos, matWVP); // ローカル座標 → スクリーン座標
    outData.uv = uv.xy;

    normal = mul(normal, matNormal); // 法線ベクトルをワールド行列で変換
    normal = normalize(normal); // 法線ベクトルを正規化
    
    float4 light = float4(-1.0f, 0.5f, -0.7f, 0.0f);
    light = normalize(light);
    outData.color = clamp(dot(normal.xyz, light.xyz), 0.0f, 1.0f); 
    
    return outData;
}

// ピクセルシェーダー
float4 PS(VS_OUT inData) : SV_Target
{
    float4 color;
    if (isUseTexture == 1)
    {
        color = g_texture.Sample(g_sampler, inData.uv);
    }
    else
    {
        color = diffuseColor;
    }
    return color * inData.color;
}
