// テクスチャとサンプラーデータのグローバル変数定義
Texture2D g_texture : register(t0);     // テクスチャー
SamplerState g_sampler : register(s0);  // サンプラー

// コンスタントバッファ
// DirectX側から送信されてくる、ポリゴン頂点以外の諸情報の定義
cbuffer global
{
    float4x4 g_uvTranslate; // テクスチャの座標変換行列
    float4 g_color;         // テクスチャとの合成色
    float2 g_position;      // スクリーン上の描画座標( ピクセル単位 )
    float2 g_size;          // 画像を描画する幅と高さ( ピクセル単位 )
    float2 g_imageSize;     // 画像の幅と高さ
    float2 g_screenSize;    // 画面の大きさ
    float g_rotation;	    // スクリーンに描画するときの回転量
};

// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
struct VS_OUT
{
    float4 pos : SV_POSITION; // 位置
    float2 uv : TEXCOORD; // UV座標
};

// 頂点シェーダー
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
    // ピクセルシェーダーへ渡す情報
    VS_OUT outData;
    
    float2 localPosition = pos.xy - float2(0.5f, 0.5f); // 回転を考慮するために頂点座標を(-0.5f～0.5f)に変換する
    localPosition *= g_size; // 拡縮する

    // 回転量を計算する
    float2 rotatePosition;
    float sinA = sin(g_rotation);
    float cosA = cos(g_rotation);
    rotatePosition.x = localPosition.x * cosA - localPosition.y * sinA;
    rotatePosition.y = localPosition.x * sinA + localPosition.y * cosA;

    float2 pixelPosition = rotatePosition + g_position + (g_size / 2); // 指定された中心座標へ平行移動する
    
    // ピクセル座標からDirectXのスクリーン座標に変換
    outData.pos.x = (pixelPosition.x/ g_screenSize.x) * 2.0f - 1.0f;
    outData.pos.y = 1.0f - (pixelPosition.y / g_screenSize.y) * 2.0f;
    outData.pos.z = 0.0f;
    outData.pos.w = 1.0f;

    outData.uv = mul(uv, g_uvTranslate).xy;

    return outData;
}

// ピクセルシェーダー
float4 PS(VS_OUT inData) : SV_Target
{
    return g_color * g_texture.Sample(g_sampler, inData.uv);
}