#include"stdafx.h"

ASyncFile::ASyncFile() {
	m_file = nullptr;
	m_mode = 0;
}

ASyncFile::ASyncFile(const WCHAR*source_name, const WCHAR*filename) :
	m_source_name(source_name),
	m_filename(filename) 
{
	m_file = nullptr;
	m_mode = 0;
}

bool ASyncFile::OpenFile() {
	if (m_file != nullptr) {
		//�t�@�C���͂��łɊJ����Ă���
		return true;
	}
	m_file = _wfsopen(m_filename.c_str(), _T("rS, ccs=UTF-8"), _SH_DENYNO);
	if ( m_file != nullptr) {
		return true;
	}
	return false;
}

void ASyncFile::Exec(Candidates::ContainerType&dst) {
	TCHAR	line[4096];

	switch (m_mode) {
	case 0:
		if (! OpenFile()) {
			return;
		}
		m_file_read_start_clock = std::chrono::system_clock::now();
		++m_mode;
	case 1:
		if (fgetws(line, _countof(line), m_file) != nullptr) {
			dst.emplace_back(m_source_name.c_str(), line);
			m_file_read_start_clock = std::chrono::system_clock::now();
			return;
		}

		//
		//�ǂݍ��ލs��������Ȃ��ꍇ
		//
		{
			//�t�@�C�����e���쐬���̉\�������邽�ߏ����҂B
			auto duration = std::chrono::system_clock::now() - m_file_read_start_clock;
			auto second = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
			if (second < 8) {
				return;
			}
		}
		
		//���΂炭�҂��Ă��s��ǂݍ��߂Ȃ����߃t�@�C���������I�������Ƃ݂Ȃ��B
		++m_mode;
	case 2:
		fclose(m_file);
		m_file = nullptr;
		m_mode = -1;
	default:
		//pass
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	ASyncFiles
/////////////////////////////////////////////////////////////////////////////
ASyncFiles::ASyncFiles(Unity* instance) {
	m_instance = instance;
}

INT_PTR ASyncFiles::AppendCandidate(const WCHAR* source_name, const WCHAR* filename) {
	try {
		m_async_files.emplace_back(source_name, filename);
	}
	catch (std::exception) {
		return UNITY_NOT_FOUND_INDEX;
	}
	return m_async_files.size() - 1;
}

void ASyncFiles::Exec() {
	auto& candidates = m_instance->QueryCandidates().GetCandidates();
	for (auto&file:m_async_files) {
		file.Exec(candidates);
	}
}
