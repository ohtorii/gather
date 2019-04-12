#pragma once

//�G�ۃG�f�B�^�֕Ԃ����
struct Output {
	void Clear() {
		m_text.clear();
		m_hidemaru_lineno_to_candidate_index.clear();
		m_hidemaru_selected_lineno.clear();
	};

	void Reserve(size_t size) {
		const size_t text_line_char = 80;
		m_text.reserve(size*text_line_char);
		m_hidemaru_lineno_to_candidate_index.reserve(size);
		m_hidemaru_selected_lineno.reserve(size);
	};

	//�G�ۃG�f�B�^�֕Ԃ�������
	std::vector<std::wstring::value_type>	m_text;

	/*�u�G�ۃG�f�B�^�̍s�ԍ��v����u��⃊�X�g�̃C���f�b�N�X�v���擾����e�[�u��

	(�g�p��)
	�t�@�C�����X�g�̍s�ԍ� = m_hidemaru_lineno_to_candidate_index[�G�ۃG�f�B�^�̍s�ԍ�];
	*/
	std::vector<__int64>					m_hidemaru_lineno_to_candidate_index;

	//�G�ۃG�f�B�^�őI�����Ă���s�ԍ�(�C���f�b�N�X��1�n�܂�)
	std::vector<__int64>					m_hidemaru_selected_lineno;
};


class RefineSearch {
public:
	bool Do(const WCHAR* search_words);
	WCHAR* GetResult();

	/*�s���u�I���E�I�������v����
	hidemaru_line_no	�G�ۃG�f�B�^�̃J�[�\���ʒu�́A�G�f�B�^�I�Ɍv�Z�����s�ԍ��ł��B
	�@					�t�@�C���̐擪���P�ł��B
	*/
	INT_PTR ChangeSelected(INT_PTR hidemaru_line_no, bool is_selected);

	/*�I���s�̌����擾����
	*/
	INT_PTR GetSelectionCount();

	/*�I���s���擾����
	return	�G�ۃG�f�B�^�̃J�[�\���ʒu�́A�G�f�B�^�I�Ɍv�Z�����s�ԍ��ł��B
	�@		�t�@�C���̐擪���P�ł��B
	*/
	INT_PTR GetSelectedLineno(INT_PTR index);

	/*�I������Ă���t�@�C�������擾����i�G�ۃG�f�B�^�̍s�ԍ��o�[�W�����j
	*/
	WCHAR* GetSelectedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no);

	/*�I������Ă���t�@�C�������擾����
	*/
	WCHAR* GetSelectedFilename(INT_PTR index);

private:
	///��������
	Output				m_output;

	void Filter(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates);
};