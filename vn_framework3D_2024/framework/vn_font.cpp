//--------------------------------------------------------------//
//	"vn_Font.cpp"												//
//		�t�H���g�`��											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

WCHAR vnFont::fontName[256][64];
int vnFont::fontNum = 0;

ID2D1SolidColorBrush	*vnFont::pBrush = NULL;
IDWriteTextFormat		*vnFont::pTextFormat = NULL;
D2D1_RECT_F				vnFont::rect;

float		vnFont::Size = 16.0f;
float		vnFont::X = 0.0f;
float		vnFont::Y = 0.0f;
DWORD		vnFont::Color = 0xffffffff;

vnFont::stStringInfo	vnFont::strInfo[vnFontSTRING_MAX];
int			vnFont::registerNum=0;
int			vnFont::registerCount = 0;
WCHAR		*vnFont::allocPtr=NULL;
WCHAR		*vnFont::currentPtr=NULL;

IDWriteTextFormat *vnFont::currentTextFormat = NULL;

bool vnFont::initialize()
{
	registerNum = 0;
	registerCount = 0;
	allocPtr = new WCHAR[vnFontCHARACTER_MAX];
	currentPtr = allocPtr;

	HRESULT hr;

	//�g�p�ł���t�H���g���̎擾
	memset(fontName, 0, sizeof(fontName));
	IDWriteFontCollection* fonts = NULL;
	hr = vnDirect2D::getDWFactory()->GetSystemFontCollection(&fonts);
	if (hr == S_OK)
	{
		WCHAR locale[LOCALE_NAME_MAX_LENGTH];
		GetUserDefaultLocaleName(locale, _countof(locale));
		unsigned int const count = fonts->GetFontFamilyCount();
		for (unsigned int familyIndex = 0; familyIndex != count && fontNum < vnFontNUM_MAX; familyIndex++)
		{
			IDWriteFontFamily* family = NULL;
			fonts->GetFontFamily(familyIndex, &family);
			IDWriteLocalizedStrings* names = NULL;
			hr = family->GetFamilyNames(&names);
			if (hr != S_OK)continue;
			unsigned int nameIndex = 0;
			BOOL exists = false;
			hr = names->FindLocaleName(locale, &nameIndex, &exists);
			if (hr == S_OK && exists == TRUE)
			{
				hr = names->GetString(nameIndex, fontName[fontNum], _countof(fontName[fontNum]));
				//vnFont::output(L"%s\n", fontName[fontNum]);
				fontNum++;
			}
			family->Release();
			names->Release();
		}
		fonts->Release();
	}

	//�u���V�̐���
	vnDirect2D::getDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);
	if (hr != S_OK)
	{
		return false;
	}

	//�t�H���g�̍쐬
	pTextFormat = create(L"Meiryo", (int)Size);
	if (pTextFormat == NULL)
	{
		return false;
	}
	currentTextFormat = pTextFormat;

	rect.left = 0.0f;
	rect.top = 0.0f;
	rect.right = (float)vnMainFrame::screenWidth;
	rect.bottom = (float)vnMainFrame::screenHeight;
	
	return true;
}

void vnFont::terminate()
{
	delete[] allocPtr;
	SAFE_RELEASE(pTextFormat);
	SAFE_RELEASE(pBrush);
}

void vnFont::registerString(const WCHAR *string, UINT32 count)
{
	//return;
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4302)
	if(string==NULL)return;
	if(registerNum>=vnFontSTRING_MAX)return;
	if (registerCount+count >= vnFontCHARACTER_MAX)return;
	strInfo[registerNum].x = X;
	strInfo[registerNum].y = Y;
	strInfo[registerNum].color = Color;
	strInfo[registerNum].ptr = currentPtr;
	strInfo[registerNum].count = count;
	strInfo[registerNum].textFormat = currentTextFormat;
	wcsncpy_s((WCHAR*)currentPtr, vnFontCHARACTER_MAX-(reinterpret_cast<int>(currentPtr)- reinterpret_cast<int>(allocPtr)), string, count);
	currentPtr+=count;
	registerNum++;
	registerCount += count;
#pragma warning(pop)
}

void vnFont::renderString()
{
	if(pTextFormat==NULL)return;

#if 1
	static float reciprocal = 1.0f / 255.0f;
	for(int i=0; i<registerNum; i++)
	{
		//�`��ʒu�̐ݒ�
		rect.left = strInfo[i].x;
		rect.top = strInfo[i].y;

		//�`��F�̐ݒ�
		if (!i || Color != strInfo[i].color)
		{
			Color = strInfo[i].color;

			D2D1_COLOR_F col;
			col.r = (float)((Color & 0x00ff0000) >> 16) * reciprocal;
			col.g = (float)((Color & 0x0000ff00) >>  8) * reciprocal;
			col.b = (float)((Color & 0x000000ff) >>  0) * reciprocal;
			col.a = (float)((Color & 0xff000000) >> 24) * reciprocal;
			pBrush->SetColor(&col);
		}

		//�e�L�X�g�̕`��
		vnDirect2D::getDeviceContext()->DrawTextW(
			strInfo[i].ptr,
			strInfo[i].count,
			strInfo[i].textFormat,
			&rect,
			pBrush
		);
	}
#endif
	registerNum = 0;
	registerCount = 0;
	currentPtr = allocPtr;
	return;
}

//�g�p�ł���t�H���g���̎擾
int vnFont::getFontNum(void)
{
	return fontNum;
}

//�g�p�ł���t�H���g���̎擾
WCHAR* vnFont::getFontName(int id)
{
	if (id < 0 || id >= fontNum)return NULL;
	return fontName[id];
}

//�t�H���g�̍쐬
IDWriteTextFormat *vnFont::create(const WCHAR *fontname, int size)
{
	HRESULT hr;
	IDWriteTextFormat* ret = NULL;

	//�e�L�X�g�t�H�[�}�b�g�̐���
	hr = vnDirect2D::getDWFactory()->CreateTextFormat(
		fontname,					//�t�H���g �t�@�~���̖��O���܂ޕ����z��B
		NULL,						//�t�H���g �R���N�V���� �I�u�W�F�N�g�ւ̃|�C���^�[�B���ꂪ NULL �̏ꍇ�A�V�X�e�� �t�H���g �R���N�V�����������܂��B
		DWRITE_FONT_WEIGHT_NORMAL,	//���̃��\�b�h�ɂ���č쐬�����e�L�X�g �I�u�W�F�N�g�̃t�H���g�̑����������l�B
		DWRITE_FONT_STYLE_NORMAL,	//���̃��\�b�h�ɂ���č쐬�����e�L�X�g �I�u�W�F�N�g�̃t�H���g �X�^�C���������l�B
		DWRITE_FONT_STRETCH_NORMAL,	//���̃��\�b�h�ɂ���č쐬�����e�L�X�g �I�u�W�F�N�g�̃t�H���g�L�k�������l�B
		(float)size,				//DIP ("�f�o�C�X��ˑ��s�N�Z��") �P�ʂł̃t�H���g�̘_���T�C�Y�B1 DIP �� 1/96 �C���`�ł��B
		L"",						//���P�[�������܂ޕ����z��B
		&ret						//���̃��\�b�h���Ԃ����Ƃ��ɁA�V�����쐬���ꂽ�e�L�X�g�`���I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X���i�[����܂��B���s�����ꍇ�� NULL ���i�[����܂��B
	);
	return ret;
}

void vnFont::setPos(float x, float y)
{
	X = x;
	Y = y;
}

void vnFont::setPosX(float x)
{
	X = x;
}

void vnFont::setPosY(float y)
{
	Y = y;
}

void vnFont::setColor(DWORD color)
{
	Color = color;
}

void vnFont::setTextFormat(IDWriteTextFormat* tf)
{
	currentTextFormat = (tf == NULL ? pTextFormat : tf);
}

float vnFont::getPosX()
{
	return X;
}

float vnFont::getPosY()
{
	return Y;
}

DWORD vnFont::getColor()
{
	return Color;
}

//������̕`��(���C�h����)
void vnFont::print(float x, float y, DWORD color, const WCHAR *string, ...)
{
	if (string == NULL)return;

	X = x;
	Y = y;
	Color = color;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)wcslen(buf));
}

void vnFont::print(float x, float y, const WCHAR *string, ...)
{
	if (string == NULL)return;

	X = x;
	Y = y;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)wcslen(buf));
}

void vnFont::print(const WCHAR *string, ...)
{
	if (string == NULL)return;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)wcslen(buf));
}

//������̕`��(���C�h����/�������w��t��)
void vnFont::printCount(float x, float y, DWORD color, int count, const WCHAR *string, ...)
{
	if (string == NULL)return;

	X = x;
	Y = y;
	Color = color;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)count);
}

void vnFont::printCount(float x, float y, int count, const WCHAR *string, ...)
{
	if (string == NULL)return;

	X = x;
	Y = y;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)count);
}

void vnFont::printCount(int count, const WCHAR *string, ...)
{
	if (string == NULL)return;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)count);
}

//�W���o�͂ւ̕�����`��(���C�h����)(���C�h����)
void vnFont::output(const WCHAR *string, ...)
{
	if (string == NULL)return;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	OutputDebugStringW(buf);
}
