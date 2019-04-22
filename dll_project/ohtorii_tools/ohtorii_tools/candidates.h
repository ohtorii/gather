#pragma once
#include<string>
#include<vector>


///���
struct Candidate {
	Candidate() {
		m_selected = false;
		m_fource_show = false;
	};

	/*Candidate(const WCHAR*source_name,const WCHAR*text) :
		m_source_name(source_name) ,
		m_text(text)
	{
		m_selected = false;
		m_fource_show = false;
	};*/
	Candidate(const WCHAR*source_name, const WCHAR*text, const WCHAR*description, const WCHAR*user_data) :
		m_source_name(source_name),
		m_text(text), 
		m_description(description),
		m_user_data(user_data) 
	{
		m_selected = false;
		m_fource_show = false;
	};
	///�\�[�X��
	std::wstring			m_source_name;
	///�t�@�C�����X�g����ǂݍ��񂾃t�@�C����
	std::wstring			m_text;	
	///����
	std::wstring			m_description;
	///���[�U�[�f�[�^
	std::wstring			m_user_data;
	///���̃e�L�X�g���G�ۃG�f�B�^���őI������Ă��邩�ǂ���
	bool					m_selected;
	///�K���\�������₩�ǂ���
	bool					m_fource_show;
};
/*
struct SourceCandidate{
	SourceCandidate() {
	}
	SourceCandidate(const WCHAR*source_name) : 
		m_source_name(source_name)
	{

	}
	std::wstring			m_source_name;
	std::vector<Candidate>	m_candidates;
};
*/

class Candidates {
public:
	Candidates();
	//bool AppendCandidate(const WCHAR*source_name, const WCHAR*candidate);
	bool AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description=_T(""), const WCHAR*user_data= _T(""));
	//const std::vector<SourceCandidate>&	GetSources()const { return m_sources; }
	const std::vector<Candidate>&	GetCandidates()const	{ return m_candidates; }
	std::vector<Candidate>&			GetCandidates()			{ return m_candidates; }

	const WCHAR* GetSourceName(INT_PTR index)const;
	const WCHAR* GetText(INT_PTR index)const;
	const WCHAR* GetDescription(INT_PTR index)const;
	const WCHAR* GetUserData(INT_PTR index)const;

private:
	/*SourceCandidate* AppendIfNotExist(const WCHAR* source_name);
	SourceCandidate* Find(const WCHAR* source_name);
	*/
	//memo: ���я����d�v�Ȃ̂�vector<>�𗘗p
	//std::vector<SourceCandidate>	m_sources;

	//memo: ���я����d�v�Ȃ̂�vector<>�𗘗p
	std::vector<Candidate>	m_candidates;

};