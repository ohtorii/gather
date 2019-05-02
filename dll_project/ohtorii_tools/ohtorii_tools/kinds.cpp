#include"stdafx.h"



///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static 			WCHAR	gs_empty[]		= { 0 };
static const	WCHAR	*gs_prefix		=_T("action.");
static const	size_t	gs_prefix_size	= 7;


///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////
static bool ContainsAction(const WCHAR*str) {
	return std::equal(gs_prefix, gs_prefix + gs_prefix_size, str);
}

static bool StringToBool(const WCHAR* str) {
	if (_wcsicmp(str, _T("false")) == 0) {
		return false;
	}
	if (_wcsicmp(str, _T("0")) == 0) {
		return false;
	}
	return true;
}


static void GatherActionSections(std::vector<std::wstring> &dst, const WCHAR* filename) {
	WCHAR buf[2 * 1024];
	//[action.*]�Z�N�V�����̃p�[�X
	//static const std::wstring	prefix(_T("action."));
	//const		size_t			prefix_size = prefix.size();
	
	GetPrivateProfileSectionNames(buf, _countof(buf), filename);
	
	size_t i = 0;
	while (((i + 1) < _countof(buf)) && (buf[i] != 0) && (buf[i + 1] != 0)) {
		WCHAR* top = &buf[i];
		//OutputDebugString(top);
		if (ContainsAction(top)) {
			dst.push_back(top);
			//OutputDebugString(top);
		}
		i += wcslen(top) + 1;//+1��'\0'��ǂݔ�΂�
	}
}

static void ParseActionSection(Action &dst, const WCHAR*section_name, const WCHAR* filename) {
	/*�A�N�V�����̃Z�N�V�������p�[�X����
	*/
	WCHAR buf[2*1024];

	//memo: "action.nop" -> "nop"
	dst.m_name.assign(section_name+gs_prefix_size);

	GetPrivateProfileString(section_name, _T("function"), _T(""), buf, _countof(buf), filename);
	dst.m_label.assign(buf);

	GetPrivateProfileString(section_name, _T("description"), _T(""), buf, _countof(buf), filename);
	dst.m_description.assign(buf);

	GetPrivateProfileString(section_name, _T("is_quit"), _T(""), buf, _countof(buf), filename);
	dst.m_is_quit = StringToBool(buf);
	
	GetPrivateProfileString(section_name, _T("is_multi_selectable"), _T(""), buf, _countof(buf), filename);
	dst.m_is_multi_selectable = StringToBool(buf);
}

static bool CheckAppendable(bool candidate_is_multi_select, bool action_is_multi_selectable) {
	if (candidate_is_multi_select) {
		if (action_is_multi_selectable) {
			return true;
		}
		return false;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////
//	class Action
///////////////////////////////////////////////////////////////////////////////
Action::Action() {
	m_is_quit = false;
	m_is_multi_selectable = false;
}


///////////////////////////////////////////////////////////////////////////////
//	class Kind
///////////////////////////////////////////////////////////////////////////////
Kind::Kind() {

}

Kind::Kind(const std::wstring		&name,
	const std::wstring				&description,
	const std::wstring				&default_action,
	const std::vector<std::wstring>	&base_kind):
	m_name(name),
	m_description(description),
	m_default_action(default_action),
	m_base_kind(base_kind)
{

}

Action* Kind::FindAction(const WCHAR* action_name) {
	size_t size = m_actions.size();
	for (size_t i = 0; i < size; ++i) {
		auto&item=m_actions.at(i);
		if (wcscmp(item.m_name.c_str(), action_name) == 0) {
			return &m_actions.at(i);
		}
	}
	return nullptr;
}


///////////////////////////////////////////////////////////////////////////////
//	class Kinds
///////////////////////////////////////////////////////////////////////////////
void Kinds::Clear() {
	m_kinds.clear();
}

WCHAR* Kinds::Create(const WCHAR* kind_ini) {
	static std::wstring	name;	//static�ɂ���: �G�ۃG�f�B�^�֕������Ԃ����ߐÓI�ȃ������̈�Ƃ���
	Kind dst;

//	std::wstring	description;
	//std::wstring	default_action;
	//std::vector<std::wstring>	base_kind;

	{
		File*			file = Unity::Instance().lock()->QueryFile();
		std::wstring	temp_filename;

		if (!file->CreateTempFile(temp_filename)) {
			return gs_empty;
		}

		const WCHAR*cname = temp_filename.c_str();
		file->RegistAfterDelete(cname);
		if (!file->WriteToFile(cname, kind_ini)) {
			return gs_empty;
		}

		WCHAR buf[4 * 1024];
		GetPrivateProfileString(_T("property"), _T("name"), _T(""), buf, _countof(buf), cname);
		dst.m_name.assign(buf);
		if (dst.m_name.size() == 0) {
			return gs_empty;
		}

		GetPrivateProfileString(_T("property"), _T("description"), _T(""), buf, _countof(buf), cname);
		dst.m_description.assign(buf);

		GetPrivateProfileString(_T("property"), _T("default_action"), _T(""), buf, _countof(buf), cname);
		dst.m_default_action.assign(buf);

		{
			GetPrivateProfileString(_T("property"), _T("base_kind"), _T(""), buf, _countof(buf), cname);
			Tokenize(dst.m_base_kind,buf,_T(" \t"));
			if(0){
				//debug
				OutputDebugString(_T("==== Inheritance ===="));
				for (const auto&item : dst.m_base_kind) {
					OutputDebugString(item.c_str());
				}
			}
		}

		{
			std::vector<std::wstring> action_sections;
			action_sections.reserve(16);
			GatherActionSections(action_sections, cname);

			Action action;
			for (const auto& section_name: action_sections) {
				ParseActionSection(action, section_name.c_str(), cname);
				//dst.m_actions.emplace(section_name, action);
				dst.m_actions.emplace_back(action);
			}
		}
	}

	name.assign(dst.m_name);
	m_kinds.emplace_back(dst);
	return const_cast<WCHAR*>(name.c_str());
}

Kind* Kinds::FindKind(const WCHAR* kind_name) {	
	auto it = std::find_if(
				m_kinds.begin(), 
				m_kinds.end(), 
				[kind_name](const auto&item) {return item.m_name.compare(kind_name) == 0; }
				);
	if (it == m_kinds.end()) {
		return nullptr;
	}
	return &(*it);
}

size_t Kinds::FindKindIndex(const WCHAR* kind_name) {
	const auto num = m_kinds.size();
	for (size_t i = 0; i < num; ++i) {
		if (m_kinds.at(i).m_name.compare(kind_name) == 0) {
			return i;
		}
	}
	return UNITY_NOT_FOUND_INDEX;
}

size_t Kinds::FindActionIndex(size_t kind_index, const WCHAR* action_name) {
	const auto & actions = m_kinds.at(kind_index).m_actions;
	const auto num = actions.size();
	for (size_t i = 0; i < num; ++i) {
		if (actions.at(i).m_name.compare(action_name) == 0) {
			return i;
		}
	}
	return UNITY_NOT_FOUND_INDEX;
}

/****************************************************************************
	Kind
****************************************************************************/
const WCHAR* Kinds::GetKindName(size_t kind_index) {
	try {
		return m_kinds.at(kind_index).m_name.c_str();
	}catch (std::exception) {
		//pass
	}
	return gs_empty;
}

const WCHAR* Kinds::GetKindDescription(size_t kind_index) {
	try {
		return m_kinds.at(kind_index).m_description.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}

const WCHAR* Kinds::GetDefaultAction(size_t kind_index) {
	try {
		return m_kinds.at(kind_index).m_default_action.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}

/****************************************************************************
	Action
****************************************************************************/
const WCHAR* Kinds::GetActionName(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_name.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}
const WCHAR* Kinds::GetActionLabelName(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_label.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}
const WCHAR* Kinds::GetActionDescription(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_description.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}
bool Kinds::IsActionQuit(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_is_quit;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}
bool Kinds::IsActionMultiSelectable(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_is_multi_selectable;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}


const WCHAR* Kinds::GetDefaultActionLabelName(const WCHAR* kind_name){
	OutputDebugString(_T("GetHidemaruLabelName"));
	auto*kind = FindKind(kind_name);
	if (kind == nullptr) {
		OutputDebugString(_T("kind == nullptr"));
		return gs_empty;
	}
	OutputDebugString(_T("kind->m_default_action="));
	OutputDebugString(kind->m_default_action.c_str());
	auto *action = kind->FindAction(kind->m_default_action.c_str());
	if (action == nullptr) {
		OutputDebugString(_T("action == nullptr"));
		return gs_empty;
	}
	OutputDebugString(_T("action->m_function="));
	OutputDebugString(action->m_label.c_str());
	return action->m_label.c_str();
}

bool Kinds::GenerateKindCandidates(INT_PTR instance_index) {
	OutputDebugString(_T("GenerateKindCandidates"));
	std::weak_ptr<Unity> instance = Unity::Instance(instance_index);
	if (instance.expired()) {
		OutputDebugString(_T("@1"));
		return false;
	}
	
	{
		auto*inheritance=instance.lock()->QueryInheritance();
		inheritance->Generate();
		return false;

	}

	bool			is_multi_select = false;
	std::wstring	first_source_name;
	{
		RefineSearch*	search = instance.lock()->QueryRefineSearch();
		const auto		num_selection = search->GetMarkedCount();
		if (num_selection == 0) {
			OutputDebugString(_T("@2"));
			return false;
		}

		for (auto select = 0; select < num_selection; ++select) {
			Candidate* candidate = search->GetMarkedCandidate(select);
			if (candidate == nullptr) {
				OutputDebugString(_T("@2.1"));
				continue;
			}
			//Memo: �ŏ��ɑI�����ꂽ�\�[�X��
			OutputDebugString(_T("candidate->m_source_name="));
			OutputDebugString(candidate->m_source_name.c_str());
			first_source_name = candidate->m_source_name;
			break;
		}

		if (first_source_name.empty()) {
			OutputDebugString(_T("@3"));
			return false;
		}
		if (2 <= num_selection) {
			is_multi_select = true;
		}else {
			is_multi_select = false;
		}		
	}	

	//�\�[�X������f�B�t�H���g�J�C���h�����o��
	auto* source= instance.lock()->QuerySources()->FindSource(first_source_name.c_str());
	if (source == nullptr) {
		OutputDebugString((std::wstring(_T("@4: first_source_name="))+ first_source_name).c_str());
		return false;
	}
	const auto &default_kind = source->m_default_kind;

	auto* kind = instance.lock()->QueryKinds()->FindKind(default_kind.c_str());
	if (kind == nullptr) {
		OutputDebugString(_T("@5"));
		return false;
	}

	//���݂̃C���X�^���X�֌���ǉ�����
	{
		std::wstring candidate;
		auto* candidates = Unity::Instance().lock()->QueryCandidates();
		for (const auto& action : kind->m_actions) {
			if (CheckAppendable(is_multi_select, action.m_is_multi_selectable)) {
				auto candidate_index = candidates->AppendCandidate(_T("action"), action.m_name.c_str(), action.m_description.c_str());
				candidates->SetUserData(candidate_index, _T("__kind__"), kind->m_name.c_str());
			}
		}
	}
	OutputDebugString(_T("Finish"));
	return true;
}