#pragma once
#include<string>
#include<vector>


///���
struct Candidate {
	Candidate();
	Candidate(const WCHAR*source_name, const WCHAR*text, const WCHAR*description);
	/**���̃��[�U�[�f�[�^��ݒ肷��i������Łj
	����L�[������ꍇ�͏㏑�����܂��B
	return bool	true	����
				false	���s
	*/
	bool SetUserData(const WCHAR* key, const WCHAR*data);

	/**���̃��[�U�[�f�[�^��ݒ肷��i���l�Łj
	����L�[������ꍇ�͏㏑�����܂��B
	return bool	true	����
				false	���s
	*/
	bool SetUserData(const WCHAR* key, INT_PTR data);

	const WCHAR*	GetUserData(const WCHAR* key, const WCHAR*	default_data=_T(""));
	INT_PTR			GetUserData(const WCHAR* key, INT_PTR		default_data=0);

	///�\�[�X��
	std::wstring			m_source_name;
	///�t�@�C�����X�g����ǂݍ��񂾃t�@�C����
	std::wstring			m_text;	
	///����
	std::wstring			m_description;
	///���[�U�[�f�[�^�i������j
	std::unordered_map<std::wstring, std::wstring>	m_user_data_string;
	///���[�U�[�f�[�^�i���l�j
	std::unordered_map<std::wstring, INT_PTR>		m_user_data_numeric;
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
	
	/**����ǉ�����
	return ���ւ̃C���f�b�N�X
	*/
	INT_PTR AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description=_T(""));
	
	/**���̃��[�U�[�f�[�^��ݒ肷��i������Łj
	����L�[������ꍇ�͏㏑�����܂��B
	return bool	true	����
				false	���s
	*/
	bool SetUserData(INT_PTR index, const WCHAR* key, const WCHAR*data);

	/**���̃��[�U�[�f�[�^��ݒ肷��i���l�Łj
	����L�[������ꍇ�͏㏑�����܂��B
	return bool	true	����
				false	���s
	*/
	bool SetUserData(INT_PTR index, const WCHAR* key, INT_PTR data);

	const WCHAR*	GetUserData(INT_PTR index, const WCHAR* key, const WCHAR*	default_data = _T(""));
	INT_PTR			GetUserData(INT_PTR index, const WCHAR* key, INT_PTR		default_data = 0);

	//const std::vector<SourceCandidate>&	GetSources()const { return m_sources; }
	const std::vector<Candidate>&	GetCandidates()const	{ return m_candidates; }
	std::vector<Candidate>&			GetCandidates()			{ return m_candidates; }

	const WCHAR* GetSourceName(INT_PTR index)const;
	const WCHAR* GetText(INT_PTR index)const;
	const WCHAR* GetDescription(INT_PTR index)const;	

private:
	/*SourceCandidate* AppendIfNotExist(const WCHAR* source_name);
	SourceCandidate* Find(const WCHAR* source_name);
	*/
	//memo: ���я����d�v�Ȃ̂�vector<>�𗘗p
	//std::vector<SourceCandidate>	m_sources;

	//memo: ���я����d�v�Ȃ̂�vector<>�𗘗p
	std::vector<Candidate>	m_candidates;

};