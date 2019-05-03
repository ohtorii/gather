#pragma once
#include<string>
#include<vector>
#include<unordered_map>


class Action {
public:
	Action();
	//Action();

	///�\���Ɏg�p���閼�O
	std::wstring		m_name;
	///�G�ۃ}�N���̃��x����
	std::wstring		m_label;
	///�A�N�V�����̐���
	std::wstring		m_description;
	///�A�N�V�������s��ɏI�����邩�ǂ���
	bool				m_is_quit;
	///�A�N�V�����𕡐��I���ɑ΂��Ď��s���邩�ǂ���
	bool				m_is_multi_selectable;

};

class Kind {
public:
	Kind();
	Kind(	const std::wstring				&name,
			const std::wstring				&description,
			const std::wstring				&default_action,
			const std::vector<std::wstring>	&base_kind);
	Action* FindAction(const WCHAR* action_name);
	size_t	FindActionIndex(const WCHAR* action_name);
	std::wstring				m_name;
	std::wstring				m_description;
	std::wstring				m_default_action;
	std::vector<std::wstring>	m_base_kind;
	/// m_actions[�A�N�V������]=Action;
	//std::unordered_map<std::wstring, Action>		m_actions;
	//���я����d�v�Ȃ̂� std::vector<> �𗘗p
	std::vector<Action>		m_actions;
};

class Kinds {
public:
	//Kinds();
	void Clear();
	
	WCHAR* Create(const WCHAR* kind_ini);
	
	///�J�C���h�����t����
	Kind* FindKind(const WCHAR* kind_name);
	
	///�J�C���h�����t����
	size_t FindKindIndex(const WCHAR* kind_name);

	///�I������J�C���h�̎�ނ𐶐�����
	bool GenerateKindCandidates(INT_PTR instance_index);

	/// �G�ۂ̃��x�������擾����
	const WCHAR* GetDefaultActionLabelName(const WCHAR* kind_name);

	// �A�N�V�����̃C���f�b�N�X���擾����
	size_t FindActionIndex(size_t kind_index, const WCHAR* action_name);

	/***********************************************************************
		Kind
	***********************************************************************/
	const WCHAR* GetKindName(size_t kind_index);
	const WCHAR* GetKindDescription(size_t kind_index);
	const WCHAR* GetDefaultAction(size_t kind_index);

	/***********************************************************************
		Action
	***********************************************************************/
	const WCHAR* GetActionName(size_t kind_index, size_t action_index);
	const WCHAR* GetActionLabelName(size_t kind_index, size_t action_index);
	const WCHAR* GetActionDescription(size_t kind_index, size_t action_index);
	bool IsActionQuit(size_t kind_index, size_t action_index);
	bool IsActionMultiSelectable(size_t kind_index, size_t action_index);

private:
	///
	std::vector<Kind>		m_kinds;
};

