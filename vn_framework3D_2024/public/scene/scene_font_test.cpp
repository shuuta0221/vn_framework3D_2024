//--------------------------------------------------------------//
//	"scene_font_test.cpp"										//
//		�V�[���F�t�H���g�e�X�g									//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�Q�lURL
//https://docs.microsoft.com/ja-jp/archive/msdn-magazine/2013/november/windows-with-c-exploring-fonts-with-directwrite-and-modern-c

#define FONT_NAME_X			(40.0f)
#define FONT_NAME_Y			(40.0f)
#define FONT_NAME_LINE		(26.0f)
#define SAMPLE_TEXT_X		(640.0f)
#define SAMPLE_TEXT_Y		(40.0f)
#define SAMPLE_TEXT_LINE	(40.0f)

/*

----���g�ŕ����̃t�H���g���쐬���A������`��Ɏg�p����T���v��----

vnFont�N���X���APC�ɃC���X�g�[������Ă���t�H���g�𒲂ׂăt�H���g����ێ����Ă��܂��B
�����{�ꃍ�P�[�������t�H���g�Ɍ���܂��B

vnFont::create()�֐����ĂԂ��ƂŁA���g�Ńt�H���g�����w�肵�č쐬�A�ȍ~vnFont::print()�֐��Ŏg�����Ƃ��ł��܂��B

�t�H���g���g�p���邽�߂ɂ́AIDWriteTextFormat�̃|�C���^���K�v�ɂȂ�܂��B(create()�֐��̖߂�l)
�t�H���g�̍쐬�Ɏ��s�����ꍇ�́ANULL���Ԃ���A���̃t�H���g�͎g�p���邱�Ƃ��ł��܂���B

�w�肵�����t�H���g�ŕ`�悷�镶�����print()����O�ɁAvnFont::setTextFormat()�Ńt�H���g���w�肵�Ă����܂��B
��x�w�肷��ƁA�ȍ~��print()�͂��ׂĂ��̃t�H���g�ŕ`�悳��܂��B

�f�t�H���g�̃t�H���g�ɖ߂��ꍇ�́AvnFont::setTextFormat()�������Ȃ��ŌĂяo���܂��B

�y�d�v�z
�g�p���Ȃ��Ȃ�������t�H���g�́ASAFE_RELEASE()�}�N�����g���A�K���폜���Ă����K�v������܂��B

*/

//������
bool SceneFontTest::initialize()
{
	Cursor = 0;

	//�g�p�ł���t�H���g�̐����擾
	FontNum = vnFont::getFontNum();

	//�쐬�����t�H���g�p�̏����m�ۂ��Ă����ϐ���K�v�Ȑ��쐬
	textFormat = new IDWriteTextFormat * [FontNum];

	for (int i = 0; i < FontNum; i++)
	{
		//�t�H���g���ƃT�C�Y���w�肵�ăt�H���g���쐬(�t�H���g���͒��ڎw�肷�邱�Ƃ��\)
		textFormat[i] = vnFont::create(vnFont::getFontName(i), 24);
	}

	return true;
}

//�I��
void SceneFontTest::terminate()
{
	//�`��t�H���g���f�t�H���g�ɖ߂�
	vnFont::setTextFormat();

	for (int i = 0; i < FontNum; i++)
	{
		//�쐬�����t�H���g���폜����
		SAFE_RELEASE(textFormat[i]);
	}
	//�쐬�����t�H���g�p�̏����m�ۂ��Ă����ϐ����폜
	delete[] textFormat;
}

//�����֐�
void SceneFontTest::execute()
{
	if(vnKeyboard::trg(DIK_UP) && --Cursor<0)
	{	//���L�[�ŃJ�[�\������Ɉړ�
		Cursor = FontNum -1;
	}
	else if(vnKeyboard::trg(DIK_DOWN) && ++Cursor>= FontNum)
	{	//���L�[�ŃJ�[�\�������Ɉړ�
		Cursor = 0;
	}

	//�t�H���g���̕`��
	for (int i = 0; i < FontNum; i++)
	{
		//�I��/��I���ɂ���ĕ����̐F��ς���
		DWORD col = i == Cursor ? 0xffffff80 : 0xffffffff;
		//���ꂩ��`��(vnFont::print)����t�H���g���w�肷��
		vnFont::setTextFormat(textFormat[i]);
		//�w�肳�ꂽ���Ńt�H���g����`�悷��
		vnFont::print(FONT_NAME_X, FONT_NAME_Y + FONT_NAME_LINE * (float)i, col, vnFont::getFontName(i));
	}

	//�I�����ꂽ�t�H���g�ŃT���v��������̕`����s��

	int line = 0;
	//���ꂩ��`��(vnFont::print)����t�H���g���w�肷��
	vnFont::setTextFormat(textFormat[Cursor]);
	//�I���t�H���g��
	vnFont::print(SAMPLE_TEXT_X, SAMPLE_TEXT_Y + SAMPLE_TEXT_LINE * line++, 0xffffffff, L"\"%s\"", vnFont::getFontName(Cursor));
	//�T���v��������@
	vnFont::print(SAMPLE_TEXT_X, SAMPLE_TEXT_Y + SAMPLE_TEXT_LINE * line++, 0xffffffff, L"�y�T���v��������z");
	//�T���v��������A
	vnFont::print(SAMPLE_TEXT_X, SAMPLE_TEXT_Y + SAMPLE_TEXT_LINE * line++, 0xffffffff, L"vn_framework_2021 - Font Test");

	//���ݎ������擾���ĕ\������
	time_t now = time(NULL);
	struct tm pnow;
	localtime_s(&pnow, &now);
	WCHAR week[][3] = { L"��",L"��",L"��",L"��",L"��",L"��",L"�y" };

	//�N����
	vnFont::print(SAMPLE_TEXT_X, SAMPLE_TEXT_Y + SAMPLE_TEXT_LINE * line++, 0xffffffff, L"%2d�N%2d��%2d��(%s)",
		pnow.tm_year + 1900,
		pnow.tm_mon + 1,
		pnow.tm_mday,
		week[pnow.tm_wday]);

	//���ݎ���
	vnFont::print(SAMPLE_TEXT_X, SAMPLE_TEXT_Y + SAMPLE_TEXT_LINE * line++, 0xffffffff, L"%d:%d:%d", 
		pnow.tm_hour, 
		pnow.tm_min, 
		pnow.tm_sec);

	vnScene::execute();
}

//�`��֐�
void SceneFontTest::render()
{
	vnScene::render();
}

