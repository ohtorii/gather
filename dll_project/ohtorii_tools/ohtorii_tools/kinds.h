#pragma once
#include<string>
#include<vector>
#include<unordered_map>


class Action {
public:
	///�\���Ɏg�p���閼�O
	std::wstring		m_name;
	///�G�ۃ}�N���̃��x����
	std::wstring		m_function;
	///�A�N�V�����̐���
	std::wstring		m_description;
	///�A�N�V�������s��ɏI�����邩�ǂ���
	std::wstring		m_is_quit;
	///�A�N�V�����𕡐��I���ɑ΂��Ď��s���邩�ǂ���
	std::wstring		m_is_multi_selectable;

};

class Kind {
public:
	std::wstring				m_name;
	std::wstring				m_description;
	std::wstring				m_default_action;
	std::vector<std::wstring>	m_inheritance;
	/// m_actions[�A�N�V������]=Action;
	std::unordered_map<std::wstring, Action>		m_actions;
};

class Kinds {
public:
	//Kinds();
	void Clear();
	WCHAR* Create(const WCHAR* kind_ini);

	/// m_kind[�J�C���h��]=Kind;
	std::unordered_map<std::wstring, Kind>		m_kinds;
};

