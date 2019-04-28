#pragma once
#include"define.h"


//�G�ۃG�f�B�^�֕Ԃ����
struct Output {
	void Clear() {
		m_text.clear();
		m_hidemaru_lineno_to_candidate_index.clear();
		m_hidemaru_maeked_lineno.clear();
	};

	void Reserve(size_t size) {
		const size_t text_line_char = 80;
		m_text.reserve(size*text_line_char);
		m_hidemaru_lineno_to_candidate_index.reserve(size);
		m_hidemaru_maeked_lineno.reserve(size);
	};

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_text,
			m_hidemaru_lineno_to_candidate_index,
			m_hidemaru_maeked_lineno
		);
	};

	//�G�ۃG�f�B�^�֕Ԃ�������(Ex. "foo.txt\nbar.txt\nhoge.cpp")
	std::vector<std::wstring::value_type>	m_text;

	/*�u�G�ۃG�f�B�^�̍s�ԍ��v����u��⃊�X�g�̃C���f�b�N�X�v���擾����e�[�u��

	(�g�p��)
	���̃C���f�b�N�X = m_hidemaru_lineno_to_candidate_index[�G�ۃG�f�B�^�̍s�ԍ�];
	*/
	std::vector<INT_PTR>					m_hidemaru_lineno_to_candidate_index;

	//�G�ۃG�f�B�^�Ń}�[�N���Ă���s�ԍ�(�C���f�b�N�X��1�n�܂�)
	std::vector<INT_PTR>					m_hidemaru_maeked_lineno;
};


class Unity;

class RefineSearch {
public:
	RefineSearch(Unity* instance);
	bool Do(const WCHAR* search_words);
	WCHAR* GetResult();

	/*�G�ۃG�f�B�^�̍s�ԍ���ݒ肷��
	*/
	void SetHidemaruLineno(INT_PTR hidemaru_line_no);

	////////////////////////////////////////////////////////////////////////////
	//�ϊ�
	////////////////////////////////////////////////////////////////////////////
	/**�G�ۃG�f�B�^�̍s�ԍ������⃊�X�g�̃C���f�b�N�X���擾����
	return	����	0�ȏ�̐��l
			���s	-1
	*/
	INT_PTR ConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no);


	////////////////////////////////////////////////////////////////////////////
	//�}�[�N
	////////////////////////////////////////////////////////////////////////////
	/*�s���u�}�[�N����E�}�[�N��������v
	hidemaru_line_no	�G�ۃG�f�B�^�̃J�[�\���ʒu�́A�G�f�B�^�I�Ɍv�Z�����s�ԍ��ł��B
	�@					�t�@�C���̐擪���P�ł��B
	*/
	INT_PTR ChangeMarked(INT_PTR hidemaru_line_no, bool is_selected);
	
	
	/*�}�[�N����Ă���t�@�C�������擾����i�G�ۃG�f�B�^�̍s�ԍ��o�[�W�����j
	return	����	�t�@�C����
			���s	�󕶎�
	*/
	//WCHAR* GetMarkedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no);
	
	/*�}�[�N���ꂽ�s�����擾����
	*/
	INT_PTR GetMarkedCount();

	/*�}�[�N�̃C���f�b�N�X����G�ۃG�f�B�^�̍s�ԍ��֕ϊ�����
	return	�G�ۃG�f�B�^�̍s�ԍ�
	*/
	INT_PTR ConvertSelectedIndexToHidemaruLineno(INT_PTR marked_index);

	/*�}�[�N�C���f�b�N�X������C���f�b�N�X�֕ϊ�����
	*/
	INT_PTR ConvertMarkIndexToCandidatesIndex(INT_PTR marked_index);

	/*�}�[�N�����t�@�C�������擾����
	*/
	//WCHAR* GetSelectedFilename(INT_PTR marked_index);

	/*�}�[�N�������̏����擾����
	*/
	Candidate* GetMarkedCandidate(INT_PTR marked_index);	

	////////////////////////////////////////////////////////////////////////////
	//�I��
	////////////////////////////////////////////////////////////////////////////
	
	/** �ŏ��ɑI���������̃C���f�b�N�X���擾����
	return	not -1	���C���f�b�N�X
			-1		����
	*/
	INT_PTR	GetFirstSelectionCandidateIndex();

	/** �I��������␔���擾����
	*/
	INT_PTR	GetSelectionCandidateCount();

	/** �I���������̃C���f�b�N�X���擾����
	*/
	INT_PTR	GetSelectionCandidateIndex(INT_PTR selected_index);

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_output, 
			m_hidemaru_line_no
		);
	};

private:
	Unity*				m_instance;
	///��������
	Output				m_output;
	///�G�ۃG�f�B�^�̍s�ԍ�
	INT_PTR				m_hidemaru_line_no;

	void Filter(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates);
};