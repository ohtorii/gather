#include"stdafx.h"


void DebugLog(const WCHAR *fmt, ...)
{
	va_list		ap;
	int			len = 0;
	std::vector<wchar_t>	buffer;

	va_start(ap, fmt);
	len = _vscwprintf(fmt, ap) + 1;//+1 == '\0'
	if (len) {
		buffer.resize(len);
		vswprintf_s(buffer.data(), len, fmt, ap);

		// VisualStudio�̃f�o�b�O�E�B���h�E�ɂ͕K���o�͂���
		OutputDebugString(buffer.data());
	}
	va_end(ap);
}

void DebugLogLastError(DWORD errorcode) {
	LPVOID lpMsgBuf = 0;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER  //      �e�L�X�g�̃��������蓖�Ă�v������
		| FORMAT_MESSAGE_FROM_SYSTEM    //      �G���[���b�Z�[�W��Windows���p�ӂ��Ă�����̂��g�p
		| FORMAT_MESSAGE_IGNORE_INSERTS,//      ���̈����𖳎����ăG���[�R�[�h�ɑ΂���G���[���b�Z�[�W���쐬����
		NULL, errorcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//   ������w��
		(LPTSTR)&lpMsgBuf,                          //      ���b�Z�[�W�e�L�X�g���ۑ������o�b�t�@�ւ̃|�C���^
		0,
		NULL);
	DebugLog(_T("%s"), lpMsgBuf);
	LocalFree(lpMsgBuf);
}
