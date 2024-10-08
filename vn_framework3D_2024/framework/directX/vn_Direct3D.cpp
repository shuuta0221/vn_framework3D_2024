//--------------------------------------------------------------//
//	"vn_Direct3D.cpp"											//
//		Direct3D�Ǘ�											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../vn_environment.h"

extern HWND hWnd;	// �E�B���h�E�̃n���h��

//#define USE_GPU (1)	//�f�B�X�N���[�gGPU���������Ďg�p����

UINT vnDirect3D::rtvIndex = 0;

IDXGIFactory4* vnDirect3D::pFactory = NULL;	//DXGI�t�@�N�g��
IDXGIAdapter3* vnDirect3D::pAdapter = NULL;	//�A�_�v�^�[
ID3D12Device* vnDirect3D::pDevice = NULL;	//D3D12�f�o�C�X
IDXGISwapChain3* vnDirect3D::pSwapChain = NULL;	//�X���b�v�`�F�C��
ID3D12CommandQueue* vnDirect3D::pCmdQueue = NULL;	//�R�}���h�L���[
HANDLE						vnDirect3D::hFenceEvent = NULL;	//�t�F���X�C�x���g
ID3D12Fence* vnDirect3D::pQueueFence = NULL;	//�R�}���h�L���[�p�̃t�F���X
ID3D12DescriptorHeap* vnDirect3D::pDH_RTV = NULL;	//�f�B�X�N���v�^�q�[�v(RenderTargetView)
ID3D12DescriptorHeap* vnDirect3D::pDH_DSV = NULL;	//�f�B�X�N���v�^�q�[�v(DepthStencilView)
D3D12_CPU_DESCRIPTOR_HANDLE	vnDirect3D::hRTV[frameCount];	//�f�B�X�N���v�^�n���h��(RenderTargetView)
D3D12_CPU_DESCRIPTOR_HANDLE	vnDirect3D::hDSV;				//�f�B�X�N���v�^�n���h��(DepthStencilView)
ID3D12Resource* vnDirect3D::pRenderTarget[frameCount];	//�����_�[�^�[�Q�b�g
ID3D12Resource* vnDirect3D::pDepthBuffer = NULL;
ID3D12CommandAllocator* vnDirect3D::pCmdAllocator = NULL;	//�R�}���h�A���P�[�^
ID3D12GraphicsCommandList* vnDirect3D::pCmdList = NULL;	//�R�}���h���X�g
ID3D12RootSignature* vnDirect3D::pRootSignature = NULL;	//���[�g�V�O�l�`��

UINT vnDirect3D::fenceValue = 0;

ID3D12Resource* vnDirect3D::pWhiteTex = NULL;
D3D12_SHADER_RESOURCE_VIEW_DESC vnDirect3D::srvDesc = {};


int vnDirect3D::initialize()
{
	HRESULT hr = S_OK;

	createFactory();

	createDevice();

	createCommandQueue();

	createSwapChain();

	createRenderTargetView();

	createDepthStencilBuffer();

	createCommandList();

	createRootSignature();

	createWhiteTexture();

	hr = pCmdList->Close();

	return 1;
}

void vnDirect3D::createFactory()
{
	HRESULT hr = S_OK;
	UINT dxgiFactoryFlags = 0;

	//�f�o�b�O���[�h�̏ꍇ�̓f�o�b�O���C���[��L���ɂ���
#if defined(_DEBUG)
	{
		ID3D12Debug* debug = NULL;
		hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
		assert(hr == S_OK);
		debug->EnableDebugLayer();

		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

		debug->Release();
	}
#endif

	//�t�@�N�g���̍쐬
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&pFactory));
	assert(hr == S_OK);

	//�t���X�N���[���ւ̑J�ڂ��T�|�[�g���Ȃ�
//	hr = pFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
}

void vnDirect3D::createDevice()
{
	HRESULT hr = S_OK;

	//�A�_�v�^�[��񋓂��A�ŏ��Ɍ��������A�_�v�^�[���g�p����
	hr = pFactory->EnumAdapters(0, (IDXGIAdapter**)&pAdapter);

#ifdef USE_GPU
	//�f�B�X�N���[�gGPU���������Ďg�p����
	IDXGIAdapter3* tmpAdapter = NULL;
	for (int i = 1; pFactory->EnumAdapters(i, (IDXGIAdapter**)&tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC1 adesc = {};
		tmpAdapter->GetDesc1(&adesc);
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{

		}
		else
		{
			pAdapter = tmpAdapter;
			break;
		}
	}
#endif
	assert(hr == S_OK);

#if 1
	{
		DXGI_ADAPTER_DESC1 adesc = {};
		pAdapter->GetDesc1(&adesc);
		OutputDebugStringW(L"Selected Adapter : ");
		OutputDebugStringW(adesc.Description);
		OutputDebugStringW(L"\n");
	}
#endif

	//�t�B�[�`�����x����
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	//�f�o�C�X�̍쐬
	for (int i = 0; i < sizeof(levels) / sizeof(D3D_FEATURE_LEVEL); i++)
	{
		hr = D3D12CreateDevice(
			pAdapter,
			levels[i],
			IID_PPV_ARGS(&pDevice)
		);
		if (hr == S_OK)
		{
			pDevice->SetName(L"vnDirect3D::pDevice");
			break;
		}
	}
	assert(pDevice != NULL);
}

void vnDirect3D::createCommandQueue()
{
	HRESULT hr = S_OK;

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC command_queue_desc = {};
	command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;			//�R�}���h���X�g�ƍ��킹��
	command_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	//�w��Ȃ�
	command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;			//�^�C���A�E�g�Ȃ�
	command_queue_desc.NodeMask = 0;	//�A�_�v�^�[��������g��Ȃ��Ƃ���0

	//�R�}���h�L���[�̍쐬
	hr = pDevice->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&pCmdQueue));
	assert(hr == S_OK);
	pCmdQueue->SetName(L"vnDirect3D::pCmdQueue");

	//�t�F���X�C�x���g�̍쐬
	hFenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
	assert(hFenceEvent != NULL);
	//�R�}���h�L���[�p�̃t�F���X�̐���
	hr = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pQueueFence));
	assert(hr == S_OK);
	pQueueFence->SetName(L"vnDirect3D::pQueueFence");

	fenceValue = 1;
}

void vnDirect3D::createSwapChain()
{
	HRESULT hr = S_OK;

	//�X���b�v�`�F�C���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = (UINT)vnMainFrame::screenWidth;
	swapChainDesc.Height = (UINT)vnMainFrame::screenHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//�X���b�v�`�F�C���̍쐬
	hr = pFactory->CreateSwapChainForHwnd(
		pCmdQueue,
		hWnd,
		&swapChainDesc,
		NULL,
		NULL,
		(IDXGISwapChain1**)&pSwapChain
	);
	assert(hr == S_OK);

	//�J�����g�̃o�b�N�o�b�t�@�̃C���f�b�N�X���擾����
	rtvIndex = pSwapChain->GetCurrentBackBufferIndex();

	//�t���X�N���[���ւ̑J�ڂ��T�|�[�g���Ȃ�
	hr = pFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);	//�X���b�v�`�F�C���̍쐬��
}

void vnDirect3D::createRenderTargetView()
{
	HRESULT hr = S_OK;

	//RTV�p�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
	heap_desc.NumDescriptors = frameCount;
	heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heap_desc.NodeMask = 0;

	//RTV�p�f�X�N���v�^�q�[�v�̍쐬
	hr = pDevice->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&pDH_RTV));
	assert(hr == S_OK);
	pDH_RTV->SetName(L"vnDirect3D::pDH_RTV");

	//�f�B�X�N���v�^�̃T�C�Y���擾
	UINT size = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//�����_�[�^�[�Q�b�g���쐬
	for (UINT i = 0; i < frameCount; i++)
	{
		//�X���b�v�`�F�C������o�b�t�@���󂯎��
		hr = pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pRenderTarget[i]));
		assert(hr == S_OK);
		pRenderTarget[i]->SetName(L"vnDirect3D::pRenderTarget");
		//RenderTargetView���쐬���ăq�[�v�f�X�N���v�^�ɓo�^
		hRTV[i] = pDH_RTV->GetCPUDescriptorHandleForHeapStart();
		hRTV[i].ptr += size * i;
		pDevice->CreateRenderTargetView(pRenderTarget[i], NULL, hRTV[i]);
	}
}

void vnDirect3D::createDepthStencilBuffer()
{
	HRESULT hr = S_OK;

	//�[�x�o�b�t�@�쐬
	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Alignment = 0;
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//2�����̃e�N�X�`���f�[�^
	depthResDesc.Width = vnMainFrame::screenWidth;					//�����_�[�^�[�Q�b�g�Ɠ���
	depthResDesc.Height = vnMainFrame::screenHeight;				//�����_�[�^�[�Q�b�g�Ɠ���
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;					//�[�x�l�������ݗp�t�H�[�}�b�g
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.SampleDesc.Quality = 0;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//�[�x�X�e���V���Ƃ��Ďg�p
	depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthResDesc.MipLevels = 0;

	//�f�v�X�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	depthHeapProp.CreationNodeMask = 0;
	depthHeapProp.VisibleNodeMask = 0;

	//�N���A�o�����[
	D3D12_CLEAR_VALUE _depthClearValue = {};
	_depthClearValue.DepthStencil.Depth = 1.0f;
	_depthClearValue.DepthStencil.Stencil = 0;
	_depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//32bit�[�x�l�Ƃ��ăN���A

	hr = pDevice->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //�f�v�X�������݂Ɏg�p
		&_depthClearValue,
		IID_PPV_ARGS(&pDepthBuffer));
	assert(hr == S_OK);
	pDepthBuffer->SetName(L"vnDirect3D::pDepthBuffer");

	//�[�x�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[1�̂�
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[�Ƃ��Ďg��
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	hr = pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&pDH_DSV));
	assert(hr == S_OK);
	pDH_DSV->SetName(L"vnDirect3D::pDH_DSV");

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	pDevice->CreateDepthStencilView(pDepthBuffer, &dsvDesc, pDH_DSV->GetCPUDescriptorHandleForHeapStart());

	//�n���h���̎擾
	hDSV = pDH_DSV->GetCPUDescriptorHandleForHeapStart();
}

void vnDirect3D::createCommandList()
{
	HRESULT hr = S_OK;

	//�R�}���h�A���P�[�^�̍쐬
	hr = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAllocator));
	assert(hr == S_OK);
	pCmdAllocator->SetName(L"vnDirect3D::pCmdAllocator");

	//�R�}���h���X�g�̍쐬(�R�}���h�A���P�[�^�ƃo�C���h)
	hr = pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator, NULL, IID_PPV_ARGS(&pCmdList));
	assert(hr == S_OK);
	pCmdList->SetName(L"vnDirect3D::pCmdList");
}

void vnDirect3D::createRootSignature()
{
	HRESULT hr = S_OK;

	//���[�g�V�O�l�`���̐ݒ�

	D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};//�e�N�X�`���ƒ萔�̂Q��
	descTblRange[0].NumDescriptors = 1;							//�e�N�X�`���ЂƂ�
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//��ʂ̓e�N�X�`��
	descTblRange[0].BaseShaderRegister = 0;						//0�ԃX���b�g����
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descTblRange[1].NumDescriptors = 1;							//�萔�ЂƂ�
	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//��ʂ͒萔
	descTblRange[1].BaseShaderRegister = 0;						//0�ԃX���b�g����
	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER rootparam = {};
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam.DescriptorTable.pDescriptorRanges = &descTblRange[0];			//�f�X�N���v�^�����W�̃A�h���X
	rootparam.DescriptorTable.NumDescriptorRanges = _countof(descTblRange);	//�f�X�N���v�^�����W��
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_���猩����

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_DESC	root_signature_desc = {};
	root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	root_signature_desc.NumParameters = 1;
	root_signature_desc.pParameters = &rootparam;
	root_signature_desc.NumStaticSamplers = 1;
	root_signature_desc.pStaticSamplers = &samplerDesc;

	//���[�g�V�O�l�`���̃o�C�i���R�[�h���쐬
	ID3DBlob* rSigBlob = NULL;
	ID3DBlob* rSigBlobErr = NULL;
	hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &rSigBlob, &rSigBlobErr);
	assert(hr == S_OK);

	//���[�g�V�O�l�`���̍쐬
	hr = pDevice->CreateRootSignature(0, rSigBlob->GetBufferPointer(), rSigBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
	assert(hr == S_OK);
	pRootSignature->SetName(L"vnDirect3D::pRootSignature");

	SAFE_RELEASE(rSigBlob);
	SAFE_RELEASE(rSigBlobErr);
}

void vnDirect3D::createWhiteTexture()
{
	HRESULT hr = S_OK;

	//�e�N�X�`���̍쐬

	//�e�N�X�`���̃T�C�Y
	UINT32 texWidth = 32;
	UINT32 texHeight = 32;

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;							//����Ȑݒ�
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;	//���C�g�o�b�N
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;			//�]����L0(CPU��)���璼�ڍs��
	texHeapProp.CreationNodeMask = 0;
	texHeapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC texResDesc = {};
	texResDesc.Alignment = 0;
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResDesc.Width = texWidth;
	texResDesc.Height = texHeight;
	texResDesc.DepthOrArraySize = 1;
	texResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//RGBA�t�H�[�}�b�g
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 0;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.MipLevels = 1;

	hr = pDevice->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		NULL,
		IID_PPV_ARGS(&pWhiteTex)
	);
	assert(hr == S_OK);
	pWhiteTex->SetName(L"vnDirect3D::pWhiteTex");

	//�s�N�Z���f�[�^
	UINT32* pixelData = new UINT32[texWidth * texHeight];
	memset(pixelData, 0xff, sizeof(UINT32) * texWidth * texHeight);

	hr = pWhiteTex->WriteToSubresource(
		0,
		NULL,
		pixelData,
		sizeof(UINT32) * texWidth,				//1���C���T�C�Y
		sizeof(UINT32) * texWidth * texHeight	//�S�T�C�Y
	);
	delete[] pixelData;

	//�V�F�[�_�[���\�[�X�r���[�f�X�N���v�V����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
}


void vnDirect3D::terminate()
{
	SAFE_RELEASE(pWhiteTex);

	SAFE_RELEASE(pRootSignature);

	SAFE_RELEASE(pCmdList);
	SAFE_RELEASE(pCmdAllocator);
	for (int i = 0; i < frameCount; i++)
	{
		SAFE_RELEASE(pRenderTarget[i]);
	}
	SAFE_RELEASE(pDepthBuffer);
	SAFE_RELEASE(pDH_RTV);
	SAFE_RELEASE(pDH_DSV);
	SAFE_RELEASE(pQueueFence);
	CloseHandle(hFenceEvent);
	SAFE_RELEASE(pCmdQueue);
	SAFE_RELEASE(pSwapChain);

	//ID3D12DebugDevice* debugInterface;
	//pDevice->QueryInterface(&debugInterface);
	//debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//debugInterface->Release();

	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

}

void vnDirect3D::beginRender(void)
{
	HRESULT hr = S_OK;

	static D3D12_VIEWPORT viewport = { 0.0f, 0.0f, (float)vnMainFrame::screenWidth, (float)vnMainFrame::screenHeight, 0.0f, 1.0f };
	static D3D12_RECT scissorRect = { 0, 0, static_cast<LONG>(vnMainFrame::screenWidth), static_cast<LONG>(vnMainFrame::screenHeight) };

	hr = pCmdAllocator->Reset();
	//assert(hr == S_OK);

	hr = pCmdList->Reset(pCmdAllocator, NULL);
	//assert(hr == S_OK);

	pCmdList->SetGraphicsRootSignature(pRootSignature);
	pCmdList->RSSetViewports(1, &viewport);
	pCmdList->RSSetScissorRects(1, &scissorRect);

	//�����_�[�^�[�Q�b�g�ւ̃��\�[�X�o���A
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = pRenderTarget[rtvIndex];
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//BarrierDesc.Aliasing.pResourceBefore
	//BarrierDesc.Aliasing.pResourceAfter
	//BarrierDesc.UAV
	pCmdList->ResourceBarrier(1, &BarrierDesc);

	//�����_�[�^�[�Q�b�g�Ɛ[�x�o�b�t�@�̐ݒ�
	pCmdList->OMSetRenderTargets(1, &hRTV[rtvIndex], true, &hDSV);

	//�����_�[�^�[�Q�b�g�̃N���A
	pCmdList->ClearRenderTargetView(hRTV[rtvIndex], vnMainFrame::clearColor, 0, NULL);
	//�[�x�o�b�t�@�̃N���A
	pCmdList->ClearDepthStencilView(hDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, NULL);

}

void vnDirect3D::beginRender2(void)
{
}

void vnDirect3D::finishRender(void)
{
	HRESULT hr = S_OK;

#if 0	//���̌�Direct2D�̕`�悪����ꍇ�͂����Ńv���[���e�[�V�����ֈڍs���Ȃ�

	//�v���[���e�[�V�����ւ̃��\�[�X�o���A
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = pRenderTarget[rtvIndex];
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	pCmdList->ResourceBarrier(1, &BarrierDesc);

#endif

	hr = pCmdList->Close();
	//assert(hr == S_OK);

	ID3D12CommandList* ppCommandLists[] = { pCmdList };
	pCmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void vnDirect3D::present(void)
{
	HRESULT hr = S_OK;

	//�X���b�v�`�F�C���̃v���[���e�[�V����
	hr = pSwapChain->Present(1, 0);

	//�`��̏I���҂�
	const UINT64 fence = fenceValue;
	hr = pCmdQueue->Signal(pQueueFence, fence);
	fenceValue++;

	if (pQueueFence->GetCompletedValue() < fence)
	{
		hr = pQueueFence->SetEventOnCompletion(fence, hFenceEvent);
		WaitForSingleObject(hFenceEvent, INFINITE);
	}

	//�o�b�N�o�b�t�@�̃C���f�b�N�X���擾
	rtvIndex = pSwapChain->GetCurrentBackBufferIndex();
}


ID3D12Device* vnDirect3D::getDevice(void)
{
	return pDevice;
}

ID3D12CommandQueue* vnDirect3D::getCommandQueue(void)
{
	return pCmdQueue;
}

ID3D12RootSignature* vnDirect3D::getRootSignature(void)
{
	return pRootSignature;
}

ID3D12GraphicsCommandList* vnDirect3D::getCommandList(void)
{
	return pCmdList;
}

ID3D12Resource* vnDirect3D::getRenderTarget(int id)
{
	return pRenderTarget[id];
}


UINT vnDirect3D::getIndex(void)
{
	return rtvIndex;
}

ID3D12Resource* vnDirect3D::getWhiteTexture()
{
	return pWhiteTex;
}

D3D12_SHADER_RESOURCE_VIEW_DESC* vnDirect3D::getWhiteTextueViewDesc()
{
	return &srvDesc;
}
