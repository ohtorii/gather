﻿#include "stdafx.h"




///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };



///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////
static bool ExistUpper(const std::wstring&word) {
	if (word.cend() != std::find_if(word.cbegin(), word.cend(), [](auto&c) {return iswupper(c); })) {
		return true;
	}
	return false;
}

/*大文字小文字を区別せずに文字列str1の中に文字列str2が存在するかどうか調べる

引数
str1	大文字と小文字が混在した文字列
str2	全て小文字
*/
static bool StrStrNoCase(const std::wstring &str1, const std::wstring &str2) {
	std::wstring lower;
	lower.resize(str1.size());
	std::transform(str1.cbegin(), str1.cend(), lower.begin(), towlower);
	if (lower.find(str2) == std::wstring::npos) {
		return false;
	}	
	return true;
}

static bool MatchAll(const std::wstring &line, const std::vector<std::wstring>& tokens) {
#if 1
	//大文字と小文字を区別せず検索する。
	std::wstring lower;	
	for (auto&word : tokens) {
		lower.resize(word.size());
		std::transform(word.cbegin(), word.cend(), lower.begin(), towlower);
		if (!StrStrNoCase(line, lower)) {
			return false;
		}
	}
	return true;
#else
	//Smart case
	for (auto&word : tokens) {
		if (ExistUpper(word)) {
			//英大文字（A-Z）あり
			//大文字と小文字を区別して検索する。
			if (line.find(word) == std::wstring::npos) {
				return false;
			}
		} else {
			//英大文字（A-Z）なし
			//大文字と小文字を区別せず検索する。
			if (! StrStrNoCase(line, word)) {
				return false;
			}
		}
	}
	return true;
#endif
}

///////////////////////////////////////////////////////////////////////////////
//	Filter
///////////////////////////////////////////////////////////////////////////////
class Filter {
public:
	Filter(HidemaruView& hidemaru_view) : m_hidemaru_view(hidemaru_view)
	{
		m_current_hidemaru_lineno=1;//memo: 秀丸エディタの行番号は1スタート
		m_collapsed_index=0 ;
		m_first_match=true;
	};

	void Generate(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates)
	{
		m_hidemaru_view.Clear();
		m_hidemaru_view.m_collapsed.Clear();

		{
			const size_t	size = candidates.size();
			INT_PTR			header_candidate_index = UNITY_NOT_FOUND_INDEX;

			for (size_t candidate_list_index = 0; candidate_list_index < size; ++candidate_list_index) {
				const auto& candidate = candidates.at(candidate_list_index);

				if (candidate.m_header) {
					//memo: ヘッダー部は検索処理に含めない
					header_candidate_index = candidate_list_index;
					continue;
				}

				if (MatchAll(candidate.m_prefix+candidate.m_text+ candidate.m_prefix, tokens) || MatchAll(candidate.m_description, tokens)) {
					if (header_candidate_index != UNITY_NOT_FOUND_INDEX) {
						//ヘッダー部を挿入する
						const auto& header_candidate = candidates.at(header_candidate_index);
						if (header_candidate.m_source_name == candidate.m_source_name) {
							InserCandidate(header_candidate, header_candidate_index);
						}
						header_candidate_index = UNITY_NOT_FOUND_INDEX;
					}
					//検索にマッチした候補を挿入する
					InserCandidate(candidate, candidate_list_index);
				}
			}
		}
		//テキストの終端を追加
		m_hidemaru_view.m_hidemaru_text.push_back(_T('\0'));
	};

private:
	void InserCandidate(const Candidate &candidate, size_t candidate_list_index) {
		auto &		hidemaru_text = m_hidemaru_view.m_hidemaru_text;

		if (m_first_match) {
			//開始行には改行を挿入しない
			m_first_match = false;
		}
		else {
			hidemaru_text.push_back(_T('\n'));
		}

		//
		//候補と詳細のテキストを追加する
		//
		hidemaru_text.insert(hidemaru_text.end(), candidate.m_prefix.begin(),	candidate.m_prefix.end());
		hidemaru_text.insert(hidemaru_text.end(), candidate.m_text.begin(),		candidate.m_text.end());
		hidemaru_text.insert(hidemaru_text.end(), candidate.m_postfix.begin(),	candidate.m_postfix.end());
		if (!candidate.m_description.empty()) {
			hidemaru_text.push_back(_T('\t'));
			//hidemaru_text.push_back(_T('\t'));
			hidemaru_text.insert(hidemaru_text.end(), candidate.m_description.begin(), candidate.m_description.end());
		}
		
		
		if (candidate.m_selected) {
			m_hidemaru_view.m_hidemaru_maeked_lineno.push_back(m_current_hidemaru_lineno);
		}				
		m_hidemaru_view.m_collapsed.OnChangeCollapsedIndex(m_current_hidemaru_lineno, m_collapsed_index);
		m_hidemaru_view.m_collapsed.OnChangeHidemaruLineNo(m_current_hidemaru_lineno, m_collapsed_index);
		m_hidemaru_view.m_hidemaru_line_index_to_candidate_index.push_back(candidate_list_index);
		
		

		//
		//子供のテキストと詳細を追加する
		//
		for (const auto&child : candidate.m_child) {
			hidemaru_text.push_back(_T('\n'));
			++m_current_hidemaru_lineno;

			hidemaru_text.push_back(_T(' '));
			hidemaru_text.push_back(_T(' '));
			hidemaru_text.push_back(_T(' '));
			hidemaru_text.push_back(_T(' '));
			hidemaru_text.insert(hidemaru_text.end(), child.m_text.begin(), child.m_text.end());
			if (!child.m_description.empty()) {
				hidemaru_text.push_back(_T('\t'));
				//hidemaru_text.push_back(_T('\t'));
				hidemaru_text.insert(hidemaru_text.end(), child.m_description.begin(), child.m_description.end());
			}
			m_hidemaru_view.m_collapsed.OnChangeHidemaruLineNo(m_current_hidemaru_lineno, m_collapsed_index);
			m_hidemaru_view.m_hidemaru_line_index_to_candidate_index.push_back(candidate_list_index);
		}

		++m_collapsed_index;
		++m_current_hidemaru_lineno;
	};

	size_t			m_current_hidemaru_lineno;
	INT_PTR			m_collapsed_index;
	bool			m_first_match;
	HidemaruView&	m_hidemaru_view;
};

///////////////////////////////////////////////////////////////////////////////
//	CollapsedCandidate
///////////////////////////////////////////////////////////////////////////////
CollapsedCandidate::CollapsedCandidate(Unity *instance) {
	m_instance = instance;
}
	
void CollapsedCandidate::Clear() {
	m_collapsed_index_to_hidemaru_lineno.clear();
	m_hidemaru_line_index_to_collapsed_index.clear();
}

void CollapsedCandidate::Reserve(size_t size) {
	m_collapsed_index_to_hidemaru_lineno.reserve(size);
	m_hidemaru_line_index_to_collapsed_index.reserve(size);
}	

void CollapsedCandidate::OnChangeHidemaruLineNo(INT_PTR hidemaru_lineno, INT_PTR collapsed_index) {
	m_hidemaru_line_index_to_collapsed_index.push_back(collapsed_index);
}

void CollapsedCandidate::OnChangeCollapsedIndex(INT_PTR hidemaru_lineno, INT_PTR collapsed_index) {
	m_collapsed_index_to_hidemaru_lineno.push_back(hidemaru_lineno);
}

/*
return	秀丸エディタの行番号
*/
INT_PTR CollapsedCandidate::CalcHidemaruCursorLineNo(INT_PTR hidemaru_lineno, INT_PTR collapsed_delta) {
	auto hidemar_line_index = hidemaru_lineno - 1;	//0開始にする
	try {
		auto next_collapsed_index	= m_hidemaru_line_index_to_collapsed_index.at(hidemar_line_index) + collapsed_delta;				
		auto result_lineno			= m_collapsed_index_to_hidemaru_lineno.at(next_collapsed_index);
		return result_lineno;
	}
	catch (std::exception) {
	}
	return UNITY_NOT_FOUND_INDEX;
}



///////////////////////////////////////////////////////////////////////////////
//	RefineSearch
///////////////////////////////////////////////////////////////////////////////
RefineSearch::RefineSearch(Unity*instance) : 
	m_instance(instance), 
	m_hidemaru_view(instance),
	m_hidemaru_line_no(0) 
{
}

void RefineSearch::SetHidemaruLineno(INT_PTR hidemaru_line_no) {
	m_hidemaru_line_no = hidemaru_line_no;
}


bool RefineSearch::Do(const WCHAR* search_words) {
	try{
		m_instance->QueryASyncFiles().Exec();

		std::vector<std::wstring> tokens;
		tokens.reserve(16);
		Tokenize(tokens, const_cast<WCHAR*>(search_words), _T(" \t\n"));
		
		Candidates::ContainerType& candidates = m_instance->QueryCandidates().GetCandidates();
		//memo: std::vector<>のメモリ予約
		m_hidemaru_view.Reserve(candidates.size());
		
		Filter filter(m_hidemaru_view);
		{
			Candidates::ContainerType::scoped_lock locker(candidates);
			filter.Generate(
				tokens,
				candidates);
		}
	}
	catch (std::exception) {
		return false;
	}
	return true;
}

WCHAR* RefineSearch::GetResult() {
	if(m_hidemaru_view.m_hidemaru_text.empty()){
		return gs_empty;
	}
	return &m_hidemaru_view.m_hidemaru_text.at(0);
}


INT_PTR RefineSearch::ChangeMarked(INT_PTR hidemaru_line_no, bool is_selected) {
	try {
		bool		has_change		= false;
		const auto	candidate_index = m_hidemaru_view.m_hidemaru_line_index_to_candidate_index.at(hidemaru_line_no - 1);//-1して0始まりにする
		auto&		candidates		= m_instance->QueryCandidates().GetCandidates();
		const bool	now				= candidates.at(candidate_index).m_selected;
		
		candidates.at(candidate_index).m_selected = is_selected;

		if (now) {
			if (is_selected) {
				//select -> select 
				//pass
			}
			else {
				//select -> unselect
				auto find_it = std::find(m_hidemaru_view.m_hidemaru_maeked_lineno.begin(), m_hidemaru_view.m_hidemaru_maeked_lineno.end(), hidemaru_line_no);
				if (find_it != m_hidemaru_view.m_hidemaru_maeked_lineno.end()) {
					m_hidemaru_view.m_hidemaru_maeked_lineno.erase(find_it);
					has_change = true;
				}
			}
		}
		else {
			if (is_selected) {
				//unselect -> select
				m_hidemaru_view.m_hidemaru_maeked_lineno.push_back(hidemaru_line_no);
				has_change = true;
			}
			else {
				//unselect -> unselect
				//pass
			}
		}

		if (has_change) {
			std::sort(m_hidemaru_view.m_hidemaru_maeked_lineno.begin(), m_hidemaru_view.m_hidemaru_maeked_lineno.end());
		}

		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}


INT_PTR RefineSearch::GetMarkedCount()const {
	return m_hidemaru_view.m_hidemaru_maeked_lineno.size();
}


INT_PTR RefineSearch::ConvertSelectedIndexToHidemaruLineno(INT_PTR marked_index)const {
	try {
		return m_hidemaru_view.m_hidemaru_maeked_lineno.at(marked_index);
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}

INT_PTR RefineSearch::ConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no)const {	
	/*DebugLog(_T("ConvertHidemaruLinenNoToCandidateIndex"));
	DebugLog(_T("  hidemaru_line_no=%d"),hidemaru_line_no);
	*/
	if (hidemaru_line_no <= 0) {
		return UNITY_NOT_FOUND_INDEX;
	}
	--hidemaru_line_no;//0始まりにする
	try {
		auto value = m_hidemaru_view.m_hidemaru_line_index_to_candidate_index.at(hidemaru_line_no);
		return value;
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}

INT_PTR RefineSearch::ConvertMarkIndexToCandidatesIndex(INT_PTR marked_index)const {
	INT_PTR hidemaru_line_no = ConvertSelectedIndexToHidemaruLineno(marked_index);
	if (hidemaru_line_no <= 0) {
		return UNITY_NOT_FOUND_INDEX;
	}
	--hidemaru_line_no;//0始まりにする
	try {
		return m_hidemaru_view.m_hidemaru_line_index_to_candidate_index.at(hidemaru_line_no);
	}catch(std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}
/*
Candidate* RefineSearch::GetMarkedCandidate(INT_PTR marked_index) {	
	const auto candidate_index = ConvertMarkIndexToCandidatesIndex(marked_index);
	if (candidate_index == UNITY_NOT_FOUND_INDEX) {
		return nullptr;
	}
	try {
		return &(m_instance->QueryCandidates().GetCandidates().at(candidate_index));
	}
	catch (std::exception) {
		//pass
	}
	return nullptr;
}
*/
INT_PTR	RefineSearch::GetFirstSelectionCandidateIndex()const {
	if (GetMarkedCount()) {
		const INT_PTR first_item = 0;
		return ConvertMarkIndexToCandidatesIndex(first_item);
	}	
	return ConvertHidemaruLinenNoToCandidateIndex(m_hidemaru_line_no);
}

INT_PTR	RefineSearch::GetSelectionCandidateCount()const {
	auto marked_count = GetMarkedCount();
	if (0<marked_count) {
		return marked_count;
	}
	if (m_hidemaru_line_no <= 0) {
		return 0;
	}
	return 1;
}

INT_PTR	RefineSearch::GetSelectionCandidateIndex(INT_PTR selected_index) const{
	auto marked_count = GetMarkedCount();

	/*DebugLog(_T("GetSelectionCandidateIndex"));
	DebugLog(_T("  marked_count=%d"),marked_count);
	DebugLog(_T("  selected_index=%d"),selected_index);
	*/
	if (0 < marked_count) {
		return ConvertMarkIndexToCandidatesIndex(selected_index);
	}
	if (selected_index == 0) {
		return ConvertHidemaruLinenNoToCandidateIndex(m_hidemaru_line_no);
	}
	return UNITY_NOT_FOUND_INDEX;
}

INT_PTR RefineSearch::MoveHidemaruCursorLineNo(INT_PTR current_lineno, INT_PTR candidate_delta) {			
	//DebugLog(_T("Enter MoveHidemaruCursorLineNo %d, %d"), current_lineno, candidate_delta);
	try {
		auto next_lineno			= m_hidemaru_view.m_collapsed.CalcHidemaruCursorLineNo(current_lineno, candidate_delta);
		auto next_line_index		= next_lineno - 1;
		auto next_candidate_index	= m_hidemaru_view.m_hidemaru_line_index_to_candidate_index.at(next_line_index);
		if (m_instance->QueryCandidates().GetCandidates().at(next_candidate_index).m_selectable) {
			//DebugLog(_T("  result=%d"), next_lineno);
			return next_lineno;
		}

		//
		//選択不可の候補なので次の候補を探す。
		//

		if (candidate_delta == 0) {
			//無限ループの防止
			//DebugLog(_T("  無限ループ防止"));
			return UNITY_NOT_FOUND_INDEX;
		}
		INT_PTR next_candidate_delta = 0;
		if (0 < candidate_delta) {
			next_candidate_delta = candidate_delta + 1;
		} else {
			next_candidate_delta = candidate_delta - 1;
		}
		return MoveHidemaruCursorLineNo(current_lineno,next_candidate_delta);
	}
	catch (std::exception) {
		//pass
	}

	//DebugLog(_T("  UNITY_NOT_FOUND_INDEX"));
	return UNITY_NOT_FOUND_INDEX;	
}

