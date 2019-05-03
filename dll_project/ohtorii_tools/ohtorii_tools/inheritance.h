#pragma once
#include<string>
#include<vector>


class Unity;



/**�J�C���h�̌p�����̐����Ɗi�[
*/
class Inheritance {
public:	
	Inheritance(Unity*instance);
	bool Generate();

private:
	struct ReferenceCounter{
		std::wstring	m_kind;
		INT_PTR			m_counter;
	};

	struct ResolveAction {
		/**�J�C���h��
		(e.g.)	"file_mru"
				"common"
		*/
		std::wstring	m_kind;

		/*�A�N�V�����ւ̃C���f�b�N�X
		Kind::m_actions[m_action_index];
		*/
		size_t			m_action_index;

		/**�A�N�V������
		Kind::m_actions[m_action_index]==m_action\name;

		(e.g.)	"preview"
				"append"
		*/
		std::wstring	m_action_name;	
	};

	//memo: ���я����d�v�Ȃ̂�std::vector<>�𗘗p
	void FindCommonKind(std::vector<std::wstring> &out_common_kinds);	
	void FindCommonKindRecursive(std::vector<ReferenceCounter>	&out_reference_counter, const std::wstring&kind_name);

	void MakeResolveActions(std::vector<std::wstring> &common_kinds);

	Unity	*m_instance;
	/**�A�N�V�����̌��ꗗ
	*/
	std::vector<ResolveAction>	m_resolve_actions;
};

