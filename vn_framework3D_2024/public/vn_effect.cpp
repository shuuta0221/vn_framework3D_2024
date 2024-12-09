//--------------------------------------------------------------//
//	"vn_effect.cpp"												//
//		�G�t�F�N�g(�p�[�e�B�N��)�N���X							//
//													2024/12/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/vn_environment.h"

#define vnPARTICLE_MAX (1024)

vnEmitter::vnEmitter()
{
	//�p�[�e�B�N���z��̃������m��
	pParticle = new vnParticle[vnPARTICLE_MAX];

	for (int i = 0; i < vnPARTICLE_MAX; i++) {
		pParticle[i].Life = 0.0f;
		pParticle[i].Pos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		pParticle[i].Vel = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	//�`����̏�����
	HRESULT hr = S_OK;

	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	texbuff = NULL;
	basicDescHeap = NULL;

	//�e�N�X�`���̊g���q
	WCHAR texture_file[] = L"";//L"data/image/particle/particle001.png";
	const WCHAR* ext = wcsrchr(texture_file, L'.');

	//�e�N�X�`���̓ǂݍ���
	if (ext != NULL)
	{
		if (wcscmp(ext, L".tga") == 0 || wcscmp(ext, L".TGA") == 0)
		{
			hr = LoadFromTGAFile(texture_file, TGA_FLAGS_NONE, &metadata, scratchImg);
		}
		else
		{
			hr = LoadFromWICFile(texture_file, WIC_FLAGS_NONE, &metadata, scratchImg);
		}
		if (hr == S_OK)
		{
			const Image* img = scratchImg.GetImage(0, 0, 0);

			D3D12_HEAP_PROPERTIES texHeapProp = {};
			texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
			texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
			texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
			texHeapProp.CreationNodeMask = 0;
			texHeapProp.VisibleNodeMask = 0;

			D3D12_RESOURCE_DESC resDesc = {};
			resDesc.Format = metadata.format;
			resDesc.Width = metadata.width;
			resDesc.Height = (UINT)metadata.height;
			resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
			resDesc.SampleDesc.Count = 1;
			resDesc.SampleDesc.Quality = 0;
			resDesc.MipLevels = (UINT16)metadata.mipLevels;
			resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
			resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			hr = vnDirect3D::getDevice()->CreateCommittedResource(
				&texHeapProp,
				D3D12_HEAP_FLAG_NONE,
				&resDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				nullptr,
				IID_PPV_ARGS(&texbuff)
			);
			texbuff->SetName(L"vnEffect::texbuff");

			hr = texbuff->WriteToSubresource(0,
				NULL,
				img->pixels,
				(UINT)img->rowPitch,
				(UINT)img->slicePitch
			);
		}
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 2;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		hr = vnDirect3D::getDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

		//�萔�o�b�t�@
		D3D12_HEAP_PROPERTIES constHeapProp = {};
		constHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		constHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		constHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		constHeapProp.CreationNodeMask = 1;
		constHeapProp.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC constDesc = {};
		constDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		constDesc.Width = ((sizeof(stConstantBuffer) + 0xff) & ~0xff);
		constDesc.Height = 1;
		constDesc.DepthOrArraySize = 1;
		constDesc.MipLevels = 1;
		constDesc.Format = DXGI_FORMAT_UNKNOWN;
		constDesc.SampleDesc.Count = 1;
		constDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		constDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		hr = vnDirect3D::getDevice()->CreateCommittedResource(
			&constHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&constDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff)
		);
		constBuff->SetName(L"vnEffect::constBuff");

		D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
		//�V�F�[�_���\�[�X�r���[�̍쐬
		if (texbuff != NULL)
		{
			//�e�N�X�`���r���[�쐬
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = metadata.format;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;

			vnDirect3D::getDevice()->CreateShaderResourceView(texbuff,
				&srvDesc,
				basicHeapHandle
			);
		}
		else
		{
			vnDirect3D::getDevice()->CreateShaderResourceView(vnDirect3D::getWhiteTexture(),
				vnDirect3D::getWhiteTextueViewDesc(),
				basicHeapHandle
			);
		}
		basicHeapHandle.ptr += vnDirect3D::getDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
		//�萔�o�b�t�@�r���[�̍쐬
		vnDirect3D::getDevice()->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

		hr = constBuff->Map(0, NULL, (void**)&pConstBuffer);
	}

	//���_�o�b�t�@
	const int vnum = vnPARTICLE_MAX * 4;

	const UINT vertexBufferSize = sizeof(vnVertex3D) * vnum;

	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = vertexBufferSize;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_UNKNOWN;
	resdesc.SampleDesc.Count = 1;
	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	hr = vnDirect3D::getDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL,
		IID_PPV_ARGS(&vertBuff)
	);
	vertBuff->SetName(L"vnEffect::vertBuff");

	hr = vertBuff->Map(0, NULL, reinterpret_cast<void**>(&vtx));

	vertexBufferView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(vnVertex3D);
	vertexBufferView.SizeInBytes = vertexBufferSize;

	//�C���f�b�N�X�o�b�t�@
	const int inum = vnPARTICLE_MAX * 6;
	{
		const UINT indexBufferSize = sizeof(WORD) * inum;

		D3D12_HEAP_PROPERTIES heapprop = {};
		heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resdesc = {};
		resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resdesc.Width = indexBufferSize;
		resdesc.Height = 1;
		resdesc.DepthOrArraySize = 1;
		resdesc.MipLevels = 1;
		resdesc.Format = DXGI_FORMAT_UNKNOWN;
		resdesc.SampleDesc.Count = 1;
		resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		hr = vnDirect3D::getDevice()->CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&pIndexBuffer)
		);
		pIndexBuffer->SetName(L"vnEffect::pIndexBuffer");
		pIndexBuffer->Map(0, nullptr, (void**)&idx);

		//�C���f�b�N�X�o�b�t�@�r���[���쐬
		indexBufferView.BufferLocation = pIndexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		indexBufferView.SizeInBytes = indexBufferSize;

		//�C���f�b�N�X�f�[�^�̏�����
		memset(idx, 0, sizeof(WORD) * inum);
	}

	//���_�f�[�^�̏�����
	for (int i = 0; i < vnPARTICLE_MAX; i++) {
		for (int j = 0; j < 4; j++) {
			vtx[i * 4 + j].x = 0.0f;
			vtx[i * 4 + j].y = 0.0f;
			vtx[i * 4 + j].z = 0.0f;
			vtx[i * 4 + j].nx = 0.0f;
			vtx[i * 4 + j].ny = 0.0f;
			vtx[i * 4 + j].nz = 0.0f;
			vtx[i * 4 + j].r = 1.0f;
			vtx[i * 4 + j].g = 1.0f;
			vtx[i * 4 + j].b = 1.0f;
			vtx[i * 4 + j].a = 1.0f;
		}
		vtx[i * 4 + 0].u = 0.0f; vtx[i * 4 + 0].v = 0.0f;
		vtx[i * 4 + 1].u = 1.0f; vtx[i * 4 + 1].v = 0.0f;
		vtx[i * 4 + 2].u = 0.0f; vtx[i * 4 + 2].v = 1.0f;
		vtx[i * 4 + 3].u = 1.0f; vtx[i * 4 + 3].v = 1.0f;
	}

	//�`�悳���C���f�b�N�X�̐�
	IndexNum = 0;

	//�}�e���A���֘A
	Diffuse = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	Ambient = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	Specular = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	setTransparent(true);

	setLighting(false);
}

vnEmitter::~vnEmitter()
{
	delete pParticle;

	SAFE_RELEASE(basicDescHeap);
	SAFE_RELEASE(texbuff);
	SAFE_RELEASE(constBuff);
	SAFE_RELEASE(vertBuff);
	SAFE_RELEASE(pIndexBuffer);
}

void vnEmitter::execute()
{
	//�p�[�e�B�N���̕��o
	for (int i = 0; i < vnPARTICLE_MAX; i++) {
		if (pParticle[i].Life > 0.0f)continue;

		pParticle[i].Life = 60.0f;
		pParticle[i].Pos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		pParticle[i].Vel = XMVectorSet(
			(float)(rand() % 2000) / 1000.0f - 1.0f,	//-1~+1
			(float)(rand() % 1000) / 1000.0f,			//+0~+1
			(float)(rand() % 2000) / 1000.0f - 1.0f,	//-1~+1
			0.0f);
		pParticle[i].Vel *= 0.1f;
		break;
	}

	//�p�[�e�B�N���̓���
	for (int i = 0; i < vnPARTICLE_MAX; i++) {
		if (pParticle[i].Life <= 0.0f)continue;

		pParticle[i].Life -= 1.0f;

		pParticle[i].Pos += pParticle[i].Vel;
	}
}

void vnEmitter::setVertexPosition()
{
	XMMATRIX mBillboard = *vnCamera::getView();
	//�ړ��������Ȃ���(w������1)
	mBillboard.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//�t�s����v�Z(�A�t�B���ϊ��݂̂̂��ߓ]���s��͓���)
	mBillboard = XMMatrixTranspose(mBillboard);

	//�p�[�e�B�N���̃f�o�b�N�`��
	for (int i = 0; i < vnPARTICLE_MAX; i++) {
		if (pParticle[i].Life <= 0.0f)continue;

		XMVECTOR v[4];
		float size = 0.5f;
		v[0] = XMVectorSet(-size, +size, 0.0f, 0.0f);
		v[1] = XMVectorSet(+size, +size, 0.0f, 0.0f);
		v[2] = XMVectorSet(-size, -size, 0.0f, 0.0f);
		v[3] = XMVectorSet(+size, -size, 0.0f, 0.0f);

		/*�r���{�[�h�ɂȂ�悤�Ɍv�Z*/
		for (int j = 0; j < 4; j++) {
			v[j] = XMVector3TransformNormal(v[j], mBillboard);
			v[j] += pParticle[i].Pos;

			vtx[j].x = XMVectorGetX(v[j]);
			vtx[j].y = XMVectorGetY(v[j]);
			vtx[j].z = XMVectorGetZ(v[j]);
		}
		//�C���f�b�N�X�f�[�^
		idx[0] = 0; idx[1] = 1; idx[2] = 2;
		idx[3] = 1; idx[4] = 3; idx[5] = 2;
		IndexNum = 6;

		vnDebugDraw::Line(&v[0], &v[1]);
		vnDebugDraw::Line(&v[1], &v[3]);
		vnDebugDraw::Line(&v[3], &v[2]);
		vnDebugDraw::Line(&v[2], &v[0]);

		vnDebugDraw::Line(&Position, &pParticle[i].Pos);
	}
}

void vnEmitter::render()
{
	setVertexPosition();
#if 1
	calculateLocalMatrix();
	calculateWorldMatrix();

	//�r���[�|�[�g�ϊ��}�g���N�X�̌v�Z
	XMMATRIX WVP = XMMatrixMultiply(XMMatrixIdentity(), *vnCamera::getScreen());

	//���_���W�̐ݒ�
	setVertexPosition();

	//�R���X�^���g�o�b�t�@�ɏ���ݒ�
	XMStoreFloat4x4(&pConstBuffer->WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&pConstBuffer->World, XMMatrixTranspose(World));
	XMStoreFloat4(&pConstBuffer->LightDir, *vnLight::getILightDir());
	XMStoreFloat4(&pConstBuffer->LightCol, *vnLight::getLightColor());
	XMStoreFloat4(&pConstBuffer->LightAmb, *vnLight::getAmbient());
	XMStoreFloat4(&pConstBuffer->CameraPos, *vnCamera::getPosition());
	XMStoreFloat4(&pConstBuffer->Diffuse, Diffuse);
	XMStoreFloat4(&pConstBuffer->Ambient, Ambient);
	XMStoreFloat4(&pConstBuffer->Specular, Specular);

	if (lighting == true)
	{	//���C�e�B���O�L��
		if (transparent == true)
		{
			//�������L��
			vnDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha);
		}
		else
		{
			vnDirect3D::getCommandList()->SetPipelineState(pPipelineState);
		}
	}
	else
	{	//���C�e�B���O����
		if (transparent == true)
		{
			//�������L��
			vnDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha_NL);
		}
		else
		{
			vnDirect3D::getCommandList()->SetPipelineState(pPipelineState_NL);
		}
	}

	vnDirect3D::getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	vnDirect3D::getCommandList()->SetDescriptorHeaps(1, &basicDescHeap);
	vnDirect3D::getCommandList()->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());
	vnDirect3D::getCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	vnDirect3D::getCommandList()->IASetIndexBuffer(&indexBufferView);
	
	vnDirect3D::getCommandList()->DrawIndexedInstanced(IndexNum, 1, 0, 0, 0);
#endif
}
