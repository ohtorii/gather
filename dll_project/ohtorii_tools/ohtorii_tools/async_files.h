#pragma once
#include<string>
#include<stdio.h>

///�񓯊��t�@�C��
class ASyncFile {
public:
	ASyncFile();
	ASyncFile(const WCHAR*source_name, const WCHAR*filename);
	void Exec(Candidates::ContainerType&dst);


private:
	bool OpenFile();



	///�\�[�X��
	std::wstring			m_source_name;
	///�t�@�C����
	std::wstring			m_filename;
	FILE*					m_file;
	int						m_mode;
	std::chrono::system_clock::time_point	m_file_read_start_clock;
};

class ASyncFiles {
public:
	ASyncFiles(Unity*instance);
	INT_PTR AppendCandidate(const WCHAR* spuce_name, const WCHAR* filename);
	void Exec();

//	const std::vector<std::shared_ptr<ASyncFile>>&	GetASyncFiles()const { return m_async_files; }
	//std::vector<std::shared_ptr<ASyncFile>>&		GetASyncFiles() { return m_async_files; }

private:
	Unity*					m_instance;
	///�񓯊��t�@�C��
	std::vector<ASyncFile>	m_async_files;
};