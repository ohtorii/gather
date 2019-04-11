#pragma once


///���
struct Candidate {
	Candidate() {
		m_selected = false;
		m_fource_show = false;
	};

	Candidate(const WCHAR*text) : m_text(text) {
		m_selected = false;
		m_fource_show = false;
	};
	Candidate(const WCHAR*text, const WCHAR*user_data) : m_text(text),m_user_data(user_data) {
		m_selected = false;
		m_fource_show = false;
	};
	///�t�@�C�����X�g����ǂݍ��񂾃t�@�C����
	std::wstring			m_text;
	///���[�U�[�f�[�^
	std::wstring			m_user_data;
	///���̃e�L�X�g���G�ۃG�f�B�^���őI������Ă��邩�ǂ���
	bool					m_selected;
	///�K���\�������₩�ǂ���
	bool					m_fource_show;
};

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


class Candidates {
public:
	Candidates();
	bool AppendCandidate(const WCHAR*source_name, const WCHAR*candidate);
	bool AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*user_data);
	
private:
	SourceCandidate* AppendIfNotExist(const WCHAR* source_name);
	SourceCandidate* Find(const WCHAR* source_name);

	//memo: ���я����厖�Ȃ̂�vector<>�𗘗p
	std::vector<SourceCandidate>	m_sources;
};