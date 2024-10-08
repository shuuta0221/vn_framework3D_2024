//--------------------------------------------------------------//
//	"vn_model.h"												//
//		���f���f�[�^�`��N���X									//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

#include "vn_object.h"

class vnModel : public vnObject
{
protected:
	struct stMaterial
	{
		//�J���[
		XMVECTOR	Diffuse;	//�g�U�F
		XMVECTOR	Ambient;	//���F
		XMVECTOR	Specular;	//���ʔ��ːF
		//�e�N�X�`��
		ID3D12Resource* texbuff;
		//�萔�o�b�t�@
		ID3D12Resource* constBuff;
		ID3D12DescriptorHeap* basicDescHeap;
		stConstantBuffer* pConstBuffer;
	};
	
	//���f���f�[�^
	vnModelData		*pModelData;
	
	//�}�e���A��
	stMaterial		*pMaterials;

	//���_�o�b�t�@
	ID3D12Resource* pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//�C���f�b�N�X�o�b�t�@
	ID3D12Resource* pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

public:
	//�R���X�g���N�^
	vnModel(const WCHAR *folder, const WCHAR *file);
	//�f�X�g���N�^
	~vnModel();
	
	//�V�X�e���֐�
	virtual void execute();
	virtual void render();

	//���f���f�[�^(vnm�t�@�C��)�̏��̎擾
	int getVertexNum();
	int getIndexNum();
	int getFaceNum();
	int getMaterialNum();
	int getMeshNum();
	const char *getTextureName(int id);

	vnModel_MeshData *getMesh();	//���b�V���f�[�^(�擪�A�h���X)�̎擾
	vnVertex3D *getVertex();		//���_�f�[�^(�擪�A�h���X)�̎擾
	LPWORD getIndex();				//�C���f�b�N�X�f�[�^(�擪�A�h���X)�̎擾

	//�g�U�F�̐ݒ�
	void setDiffuse(float r, float g, float b, float a, int id=-1);
	void setDiffuse(const XMVECTOR *v, int id=-1);
	void setDiffuseR(float value, int id=-1);
	void setDiffuseG(float value, int id=-1);
	void setDiffuseB(float value, int id=-1);
	void setDiffuseA(float value, int id=-1);
	void addDiffuseR(float value, int id=-1);
	void addDiffuseG(float value, int id=-1);
	void addDiffuseB(float value, int id=-1);
	void addDiffuseA(float value, int id=-1);

	//�g�U�F�̎擾
	XMVECTOR *getDiffuse(int id=0);
	float getDiffuseR(int id=0);
	float getDiffuseG(int id=0);
	float getDiffuseB(int id=0);
	float getDiffuseA(int id=0);

	//���F�̐ݒ�
	void setAmbient(float r, float g, float b, float a, int id=-1);
	void setAmbient(const XMVECTOR *v, int id=-1);
	void setAmbientR(float value, int id=-1);
	void setAmbientG(float value, int id=-1);
	void setAmbientB(float value, int id=-1);
	void setAmbientA(float value, int id=-1);
	void addAmbientR(float value, int id=-1);
	void addAmbientG(float value, int id=-1);
	void addAmbientB(float value, int id=-1);
	void addAmbientA(float value, int id=-1);

	//���F�̎擾
	XMVECTOR *getAmbient(int id=0);
	float getAmbientR(int id=0);
	float getAmbientG(int id=0);
	float getAmbientB(int id=0);
	float getAmbientA(int id=0);

	//���ʔ��ːF�̐ݒ�
	void setSpecular(float r, float g, float b, float p, int id = -1);
	void setSpecular(const XMVECTOR *v, int id = -1);
	void setSpecularR(float value, int id = -1);
	void setSpecularG(float value, int id = -1);
	void setSpecularB(float value, int id = -1);
	void setSpecularPower(float value, int id = -1);
	void addSpecularR(float value, int id = -1);
	void addSpecularG(float value, int id = -1);
	void addSpecularB(float value, int id = -1);
	void addSpecularPower(float value, int id = -1);

	//���ʔ��ːF�̎擾
	const XMVECTOR *getSpecular(int id = 0);
	float getSpecularR(int id = 0);
	float getSpecularG(int id = 0);
	float getSpecularB(int id = 0);
	float getSpecularPower(int id = 0);

};
