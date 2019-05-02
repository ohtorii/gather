#pragma once
#include<string>
#include<vector>


class Unity;


struct Method{
	/**�J�C���h��
	Ex. "file_mru"
		"common"
	*/
	std::wstring	m_kind;

	/*�A�N�V�����ւ̃C���f�b�N�X
	Kind::m_actions[m_action_index];
	*/
	size_t			m_action_index;

	/**�A�N�V������
	Ex. "preview"
		"append"
	*/
	//std::wstring	m_action;	
};

/**�J�C���h�̌p�����̐����Ɗi�[
*/
class Inheritance {
public:	
	Inheritance(Unity*instance);
	bool Generate();
private:
	void FindCommonKind(std::vector<std::wstring> &out_common_kinds);	
	
	Unity	*m_instance;
	/**�A�N�V�����̌��ꗗ
	*/
	std::vector<Method>			m_actions;
};

