#include"stdafx.h"


Inheritance::Inheritance(Unity*instance) : m_instance(instance){
	
}

bool Inheritance::Generate() {
	/**�I�����ꂽ���ŗ��p���Ă��鋤�ʃJ�C���h
	(��)
	common_kinds[0]="common";
	common_kinds[1]="cdable";
	*/
	std::vector<std::wstring> common_kinds;
	FindCommonKind(common_kinds);
	if (true) {
		//
		//debug
		//
		WCHAR	buf[256];
		const auto num = common_kinds.size();
		_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("==== common_kinds ==== (num=%zd)"), num);
		OutputDebugString(buf);
		for (size_t i = 0; i < num ; ++i) {
			_snwprintf_s(buf, _countof(buf),_TRUNCATE, _T("  [%zd/%zd]%s"), i+1,num,common_kinds.at(i).c_str());
			OutputDebugString(buf);
		}
	}

	MakeResolveActions(common_kinds);

	if (true) {
		//
		//debug
		//
		WCHAR	buf[256];
		const auto num = m_resolve_actions.size();
		_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("==== m_resolve_actions ==== (num=%zd)"), num);
		OutputDebugString(buf);
		for (size_t i = 0; i < num; ++i) {
			const auto & action = m_resolve_actions.at(i);
			_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("  [%zd/%zd]%s.%s(%zd)"), i + 1, num, action.m_kind.c_str(), action.m_action_name.c_str(), action.m_action_index);
			OutputDebugString(buf);
		}
	}

	return true;
}

void Inheritance:: FindCommonKind(std::vector<std::wstring> &out_common_kinds) {
	out_common_kinds.clear();
	
	//
	//memo
	//	*reference_counter[]�̎g����
	//	  reference_counter[�J�C���h��]=�Q�Ɛ�
	//
	//	*�unum_selection<=�Q�Ɛ��v�̃A�C�e�������ʃJ�C���h�ł��B
	//		���J�C���h�ɓ���J�C���h�i�Ⴆ��common�j��������o�����邱�Ƃ����邽�߁A
	//		�Q�Ɛ���num_selection���傫���Ȃ邱�Ƃ�����B
	//
	std::vector<ReferenceCounter>	reference_counter;

	const auto	num_selection	= m_instance->QueryRefineSearch()->GetSelectionCandidateCount();

	{
		auto*refine_search	= m_instance->QueryRefineSearch();
		auto*candidates		= m_instance->QueryCandidates();
		auto*sources		= m_instance->QuerySources();
		auto*kinds			= m_instance->QueryKinds();
		//
		// ���ʃJ�C���h���W�߂�
		//
		if(true){
			//debug
			WCHAR	buf[256];
			_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("num_selection=%zd"), num_selection);
			OutputDebugString(buf);
		}
		for (INT_PTR selected_index = 0; selected_index < num_selection; ++selected_index) {
			auto candidate_index = refine_search->GetSelectionCandidateIndex(selected_index);
			if (candidate_index == UNITY_NOT_FOUND_INDEX) {
				continue;
			}
			auto*source_name= candidates->GetSourceName(candidate_index);
			auto*source		= sources->FindSource(source_name);
			if (!source) {
				continue;
			}
			FindCommonKindRecursive(reference_counter, source->m_default_kind);
		}
	}

	if (true) {
		//debug
		WCHAR	buf[256];
		_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("==== reference_count ==== (num=%zd)"), reference_counter.size());
		OutputDebugString(buf);
		for (const auto&item : reference_counter) {
			_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("  %s=%zd"), item.m_kind.c_str(), item.m_counter);
			OutputDebugString(buf);
		}
	}

	// ���ʂ̃J�C���h�����t����
	for (const auto&item : reference_counter) {
		if (num_selection <= item.m_counter) {
			out_common_kinds.emplace_back(item.m_kind);
		}
	}
}

void Inheritance::FindCommonKindRecursive(std::vector<ReferenceCounter> &out_reference_counter, const std::wstring&kind_name) {
	if (kind_name.empty()) {
		return;
	}

	auto*kinds= m_instance->QueryKinds();
	auto*kind = kinds->FindKind(kind_name.c_str());
	if (!kind) {
		return;
	}
	
	auto it = std::find_if(out_reference_counter.begin(), out_reference_counter.end(), [kind](const auto &item){return item.m_kind == kind->m_name; });
	if (it == out_reference_counter.end()) {
		out_reference_counter.push_back({ kind->m_name,1 });
	} else {
		++(it->m_counter);
	}

	//���J�C���h���ċA�I�ɏ�������
	for (const auto &base_kind_name : kind->m_base_kind) {
		FindCommonKindRecursive(out_reference_counter,base_kind_name);
	}
}

void Inheritance::MakeResolveActions(std::vector<std::wstring> &common_kinds)
{
	/*memo
	m_resolve_actions�𐶐�����

	e.g.
	�i���́j
	common_kinds
		[1/2]file_mru
		[2/2]common

	�i�o�́j
	m_resolve_actions
		[1/9]common.nop(0)
		[2/9]common.yank(1)
		[3/9]common.yank_escape(2)
		[4/9]common.insert(3)
		[5/9]common.append(4)
		[6/9]common.insert_directory(5)
		[7/9]common.append_directory(6)
		[8/9]file_mru.preview(7)				<--file_mru��preview���Ăяo���B
		[9/9]common.echo(8)
	*/
	m_resolve_actions.clear();
	/*���J�C���h���珈�����邽�ߔz����t���ɂ���B*/
	std::reverse(common_kinds.begin(), common_kinds.end());

	auto*kinds = m_instance->QueryKinds();
	for (const auto&kind_name:common_kinds) {
		auto*kind=kinds->FindKind(kind_name.c_str());
		if (kind == nullptr) {
			continue;
		}
		
		size_t action_index = 0;
		for (const auto& action:kind->m_actions) {
			auto it = std::find_if(	m_resolve_actions.begin(), 
									m_resolve_actions.end(), 
									[&kind_name, &action](const auto &item) {return /*(item.m_kind==kind_name) &&*/ (item.m_action_name==action.m_name); });
			
			if (it == m_resolve_actions.end()) {
				//�V�K�ǉ�
				m_resolve_actions.push_back({ kind_name, action_index, action.m_name});
			}
			else {
				//�����̃A�N�V������h���J�C���h�ŏ㏑������
				it->m_kind = kind_name;
			}
			++action_index;
		}
	}
}
