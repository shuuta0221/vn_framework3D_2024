///--------------------------------------------------------------//
//	"vn_effect.h"												//
//		エフェクト(パーティクル)クラス							//
//													2024/12/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//パーティクルクラス
class vnParticle
{
public:
	float Life;	//寿命

	XMVECTOR Pos;	//位置
	XMVECTOR Vel;	//速度
};

//エミッタークラス
class vnEmitter : public vnObject
{
private:
	//パーティクル配列
	vnParticle* pParticle;


	//頂点データ
	vnVertex3D* vtx;

	//インデックスデータ
	WORD* idx;

	//頂点バッファ
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//インデックスバッファ
	ID3D12Resource* pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	//テクスチャ
	ID3D12Resource* texbuff;
	//定数バッファ
	ID3D12Resource* constBuff;
	ID3D12DescriptorHeap* basicDescHeap;
	stConstantBuffer* pConstBuffer;

	//マテリアル関連
	XMVECTOR	Diffuse;	//拡散色
	XMVECTOR	Ambient;	//環境色
	XMVECTOR	Specular;	//鏡面反射色

	//頂点データへの各種情報の設定
	virtual void setVertexPosition();

public:
	vnEmitter();
	virtual ~vnEmitter();

	virtual void execute();

	virtual void render();
};