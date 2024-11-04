//--------------------------------------------------------------//
//	"vn_object.cpp"												//
//		�I�u�W�F�N�g���N���X									//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/vn_environment.h"

//�ÓI���ʃf�[�^
//���_�v�f
D3D12_INPUT_ELEMENT_DESC vnObject::inputElementDescs[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};
//�p�C�v���C���X�e�[�g
ID3D12PipelineState* vnObject::pPipelineState = NULL;
ID3D12PipelineState* vnObject::pPipelineState_NL = NULL;

ID3D12PipelineState* vnObject::pPipelineState_Alpha = NULL;
ID3D12PipelineState* vnObject::pPipelineState_Alpha_NL = NULL;



//�ÓI���ʃf�[�^������
bool vnObject::initializeCommon()
{
	HRESULT hr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = vnDirect3D::getRootSignature();

	psoDesc.VS.pShaderBytecode = vnShader::getVShader(vnShader::eVertexShader::VS_3D)->getCode();
	psoDesc.VS.BytecodeLength = vnShader::getVShader(vnShader::eVertexShader::VS_3D)->getLength();
	psoDesc.PS.pShaderBytecode = vnShader::getPShader(vnShader::ePixelShader::PS_3D)->getCode();
	psoDesc.PS.BytecodeLength = vnShader::getPShader(vnShader::ePixelShader::PS_3D)->getLength();

	psoDesc.RasterizerState.MultisampleEnable = false;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.DepthClipEnable = true;
	psoDesc.RasterizerState.FrontCounterClockwise = false;
	psoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	psoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	psoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	psoDesc.RasterizerState.AntialiasedLineEnable = false;
	psoDesc.RasterizerState.ForcedSampleCount = 0;
	psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	psoDesc.BlendState.AlphaToCoverageEnable = false;
	psoDesc.BlendState.IndependentBlendEnable = false;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = false;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_DEST_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
	psoDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	psoDesc.DepthStencilState.DepthEnable = true;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psoDesc.DepthStencilState.StencilEnable = false;
	psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	//���C�e�B���O�L��&�s����
	hr = vnDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState));
	pPipelineState->SetName(L"vnObject::pPipelineState");

	//���C�e�B���O����&�s����
	psoDesc.VS.pShaderBytecode = vnShader::getVShader(vnShader::eVertexShader::VS_3D_Nolight)->getCode();
	psoDesc.VS.BytecodeLength = vnShader::getVShader(vnShader::eVertexShader::VS_3D_Nolight)->getLength();
	psoDesc.PS.pShaderBytecode = vnShader::getPShader(vnShader::ePixelShader::PS_3D_Nolight)->getCode();
	psoDesc.PS.BytecodeLength = vnShader::getPShader(vnShader::ePixelShader::PS_3D_Nolight)->getLength();

	hr = vnDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState_NL));
	pPipelineState_NL->SetName(L"vnObject::pPipelineState_NL");

	//�������`��̗L��
	psoDesc.BlendState.RenderTarget[0].BlendEnable = true;

	//�������L��&���C�e�B���O�L��
	hr = vnDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState_Alpha));
	pPipelineState_Alpha->SetName(L"vnObject::pPipelineState_Alpha");

	//�������L��&���C�e�B���O����
	psoDesc.VS.pShaderBytecode = vnShader::getVShader(vnShader::eVertexShader::VS_3D_Nolight)->getCode();
	psoDesc.VS.BytecodeLength = vnShader::getVShader(vnShader::eVertexShader::VS_3D_Nolight)->getLength();
	psoDesc.PS.pShaderBytecode = vnShader::getPShader(vnShader::ePixelShader::PS_3D_Nolight)->getCode();
	psoDesc.PS.BytecodeLength = vnShader::getPShader(vnShader::ePixelShader::PS_3D_Nolight)->getLength();

	hr = vnDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState_Alpha_NL));
	pPipelineState_Alpha_NL->SetName(L"vnObject::pPipelineState_Alpha_NL");

	return true;
}

//�ÓI���ʃf�[�^�폜
void vnObject::terminateCommon()
{
	SAFE_RELEASE(pPipelineState);
	SAFE_RELEASE(pPipelineState_NL);
	SAFE_RELEASE(pPipelineState_Alpha);
	SAFE_RELEASE(pPipelineState_Alpha_NL);
}


vnObject::vnObject()
{
	Position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	Rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	
	Scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	
	Local = XMMatrixIdentity();
	World = XMMatrixIdentity();

	executeEnable = true;
	renderEnable = true;

	lighting = true;

	transparent = false;
}

vnObject::~vnObject()
{
}

void vnObject::execute()
{
}

void vnObject::render()
{
}

//�}�g���N�X�̌v�Z
void vnObject::calculateLocalMatrix()
{
	XMMATRIX trans = XMMatrixTranslation(getPositionX(), getPositionY(), getPositionZ());
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(getRotationX(), getRotationY(), getRotationZ());
	XMMATRIX scale = XMMatrixScaling(getScaleX(), getScaleY(), getScaleZ());
	Local = scale * rotate * trans;
}

void vnObject::calculateWorldMatrix()
{
	if (pParent == NULL) {
		World = Local;
	}
	else {
		World = Local * (*pParent->getWorld());
	}
}

//�ړ��l�̐ݒ�
void vnObject::setPosition(float x, float y, float z)
{
	Position = XMVectorSet(x, y, z, 0.0f);
}

void vnObject::setPosition(const XMVECTOR *v)
{
	Position = *v;
}

void vnObject::setPositionX(float value)
{
	Position = XMVectorSetX(Position, value);
}

void vnObject::setPositionY(float value)
{
	Position = XMVectorSetY(Position, value);
}

void vnObject::setPositionZ(float value)
{
	Position = XMVectorSetZ(Position, value);
}

void vnObject::addPosition(float x, float y, float z)
{
	Position = XMVectorAdd(Position, XMVectorSet(x, y, z, 0.0f));
}

void vnObject::addPosition(const XMVECTOR *v)
{
	Position = XMVectorAdd(Position, *v);
}

void vnObject::addPositionX(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void vnObject::addPositionY(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void vnObject::addPositionZ(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

//�ړ��l�̎擾
XMVECTOR *vnObject::getPosition()
{
	return &Position;
}

float vnObject::getPositionX(void)
{
	return XMVectorGetX(Position);
}

float vnObject::getPositionY(void)
{
	return XMVectorGetY(Position);
}

float vnObject::getPositionZ(void)
{
	return XMVectorGetZ(Position);
}

//��]�l�̐ݒ�
void vnObject::setRotation(float x, float y, float z)
{
	Rotation = XMVectorSet(x, y, z, 0.0f);
}

void vnObject::setRotation(const XMVECTOR *v)
{
	Rotation = *v;
}

void vnObject::setRotationX(float radian)
{
	Rotation = XMVectorSetX(Rotation, radian);
}

void vnObject::setRotationY(float radian)
{
	Rotation = XMVectorSetY(Rotation, radian);
}

void vnObject::setRotationZ(float radian)
{
	Rotation = XMVectorSetZ(Rotation, radian);
}

void vnObject::addRotation(float x, float y, float z)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(x, y, z, 0.0f));
}

void vnObject::addRotation(const XMVECTOR *v)
{
	Rotation = XMVectorAdd(Rotation, *v);
}

void vnObject::addRotationX(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(radian, 0.0f, 0.0f, 0.0f));
}

void vnObject::addRotationY(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(0.0f, radian, 0.0f, 0.0f));
}

void vnObject::addRotationZ(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(0.0f, 0.0f, radian, 0.0f));
}

//��]�l�̎擾
XMVECTOR *vnObject::getRotation()
{
	return &Rotation;
}

float vnObject::getRotationX(void)
{
	return XMVectorGetX(Rotation);
}

float vnObject::getRotationY(void)
{
	return XMVectorGetY(Rotation);
}

float vnObject::getRotationZ(void)
{
	return XMVectorGetZ(Rotation);
}

//�g��l�̐ݒ�
void vnObject::setScale(float x, float y, float z)
{
	Scale = XMVectorSet(x, y, z, 0.0f);
}

void vnObject::setScale(const XMVECTOR *v)
{
	Scale = *v;
}

void vnObject::setScaleX(float value)
{
	Scale = XMVectorSetX(Scale, value);
}

void vnObject::setScaleY(float value)
{
	Scale = XMVectorSetY(Scale, value);
}

void vnObject::setScaleZ(float value)
{
	Scale = XMVectorSetZ(Scale, value);
}

void vnObject::addScale(float x, float y, float z)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(x, y, z, 0.0f));
}

void vnObject::addScale(const XMVECTOR *v)
{
	Scale = XMVectorAdd(Scale, *v);
}

void vnObject::addScaleX(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void vnObject::addScaleY(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void vnObject::addScaleZ(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

//�g��l�̎擾
XMVECTOR *vnObject::getScale()
{
	return &Scale;
}

float vnObject::getScaleX(void)
{
	return XMVectorGetX(Scale);
}

float vnObject::getScaleY(void)
{
	return XMVectorGetY(Scale);
}

float vnObject::getScaleZ(void)
{
	return XMVectorGetZ(Scale);
}

//���[���h�s��̎擾
XMMATRIX *vnObject::getWorld()
{
	return &World;
}

//���[���h���W�̎擾
void vnObject::getWorldPosition(XMVECTOR *v)
{
	*v = World.r[3];
}

float vnObject::getWorldPositionX()
{
	return XMVectorGetX(World.r[3]);
}

float vnObject::getWorldPositionY()
{
	return XMVectorGetY(World.r[3]);
}

float vnObject::getWorldPositionZ()
{
	return XMVectorGetZ(World.r[3]);
}

//���[���h�s��̐ݒ�
void vnObject::setWorld(XMMATRIX *m)
{
	if (m)
	{
		World = *m;
	}
}

//���s��Ԃ̐ݒ�
void vnObject::setExecuteEnable(bool flag)
{
	executeEnable =  flag;
}

//�`���Ԃ̐ݒ�
void vnObject::setRenderEnable(bool flag)
{
	renderEnable = flag;
}

//���s��Ԃ̎擾
bool vnObject::isExecuteEnable()
{
	return executeEnable;
}

//�`���Ԃ̎擾
bool vnObject::isRenderEnable()
{
	return renderEnable;
}

//���C�e�B���O�̗L��/�����̐ݒ�
void vnObject::setLighting(bool flag)
{
	lighting = flag;
}
//���C�e�B���O�̗L��/�����̎擾
bool vnObject::getLighting()
{
	return lighting;
}

//�������̗L��/�����̐ݒ�
void vnObject::setTransparent(bool flag)
{
	transparent = flag;
}
//�������̗L��/�����̎擾
bool vnObject::getTransparent()
{
	return transparent;
}

//�K�w�\���̐e�̐ݒ�
void vnObject::setParent(vnObject* p) {
	pParent = p;
}

//�K�w�\���̐e�̎擾
vnObject* vnObject::getParent(void) {
	return pParent;
}