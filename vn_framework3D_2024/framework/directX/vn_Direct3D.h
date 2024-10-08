//--------------------------------------------------------------//
//	"vn_Direct3D.h"												//
//		Direct3D�Ǘ�											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnDirect3D
{
public:
	static constexpr UINT frameCount = 2;

private:
	static UINT rtvIndex;

	static IDXGIFactory4* pFactory;	//DXGI�t�@�N�g��
	static IDXGIAdapter3* pAdapter;	//�A�_�v�^�[
	static ID3D12Device* pDevice;	//D3D12�f�o�C�X
	static IDXGISwapChain3* pSwapChain;	//�X���b�v�`�F�C��
	static ID3D12CommandQueue* pCmdQueue;	//�R�}���h�L���[
	static HANDLE						hFenceEvent;	//�t�F���X�C�x���g
	static ID3D12Fence* pQueueFence;	//�R�}���h�L���[�p�̃t�F���X
	static ID3D12DescriptorHeap* pDH_RTV;	//�f�B�X�N���v�^�q�[�v(RenderTargetView)
	static ID3D12DescriptorHeap* pDH_DSV;	//�f�B�X�N���v�^�q�[�v(DepthStencilView)
	static D3D12_CPU_DESCRIPTOR_HANDLE		hRTV[frameCount];	//�f�B�X�N���v�^�n���h��(RenderTargetView)
	static D3D12_CPU_DESCRIPTOR_HANDLE		hDSV;				//�f�B�X�N���v�^�n���h��(DepthStencilView)
	static ID3D12Resource* pRenderTarget[frameCount];	//�����_�[�^�[�Q�b�g
	static ID3D12Resource* pDepthBuffer;
	static ID3D12CommandAllocator* pCmdAllocator;	//�R�}���h�A���P�[�^
	static ID3D12GraphicsCommandList* pCmdList;	//�R�}���h���X�g
	static ID3D12RootSignature* pRootSignature;	//���[�g�V�O�l�`��

	static UINT fenceValue;

	static ID3D12Resource* pWhiteTex;
	static D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;

	//�������֐�
	static void createFactory();
	static void createDevice();
	static void createCommandQueue();
	static void createSwapChain();
	static void createRenderTargetView();
	static void createDepthStencilBuffer();
	static void createCommandList();
	static void createRootSignature();

	static void createWhiteTexture();


public:
	//�t���[�����[�N�Ǘ�
	static int initialize(void);
	static void terminate(void);

	//�`��V�X�e���R�[��
	static void beginRender(void);
	static void beginRender2(void);
	static void finishRender(void);
	static void present(void);

	//�C���^�[�t�F�C�X�֘A�̎擾
	static ID3D12Device* getDevice(void);
	static ID3D12CommandQueue* getCommandQueue(void);
	static ID3D12RootSignature* getRootSignature(void);

	static ID3D12GraphicsCommandList* getCommandList(void);

	static ID3D12Resource* getRenderTarget(int id);

	static UINT getIndex(void);

	static ID3D12Resource* getWhiteTexture();
	static D3D12_SHADER_RESOURCE_VIEW_DESC* getWhiteTextueViewDesc();
};