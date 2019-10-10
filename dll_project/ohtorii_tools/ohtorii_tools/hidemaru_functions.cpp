#include "stdafx.h"
/*

�i���j�j
�K�v�Ƃ��ꂽ�Ƃ��ɏ��߂Ċ֐��|�C���^���擾���܂��B
�Ȃ̂ŁA���O�̏������͕K�v����܂���B

�i�����j
�R�[�h�͈ȉ��̃w���v���Q�Ƃ��č쐬���܂����B
�G�ۃG�f�B�^�}�N���w���v���ڎ��� DLL������G�ۃG�f�B�^�̊֐��Ăяo�� 
*/


HidemaruFunctions::HidemaruFunctions() {
	m_initialized = false;
	pfnHidemaru_Hidemaru_EvalMacro = nullptr;
}

void HidemaruFunctions::Initialize() {
	if (m_initialized) {
		return;
	}
	HINSTANCE hinstExe = GetModuleHandle(NULL);
	if (hinstExe==nullptr) {
		return;
	}

	*(FARPROC*)&pfnHidemaru_Hidemaru_EvalMacro = GetProcAddress(hinstExe, "Hidemaru_EvalMacro");
	
	m_initialized = true;
}

bool HidemaruFunctions::EvalMacro(WCHAR* pwsz) {
	Initialize();
	if (pfnHidemaru_Hidemaru_EvalMacro) {
		return pfnHidemaru_Hidemaru_EvalMacro(pwsz);
	}
	return false;
}