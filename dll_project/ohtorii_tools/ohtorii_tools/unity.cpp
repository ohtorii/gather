﻿#include"stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
//static WCHAR	gs_empty[] = { 0 };
Unity*	Unity::m_instance=nullptr;


///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//	class Unity
///////////////////////////////////////////////////////////////////////////////
Unity* Unity::Instance(){
	if(Unity::m_instance==nullptr){
		Unity::m_instance=new Unity;
	}
	return Unity::m_instance;
}

void Unity::Destroy() {
	delete Unity::m_instance;
	Unity::m_instance = nullptr;
}

Sources* Unity::QuerySources(){
	return &m_sources;
}

File*	Unity::QueryFile(){
	return &m_file;
}

Candidates*	Unity::QueryCandidates() {
	return &m_candidates;
}

RefineSearch*	Unity::QueryRefineSearch() {
	return &m_refine_search;
}

Unity::Unity(){
	
}

Unity::~Unity(){
	
}

#if 0
INT_PTR Unity::SetCandidateList(WCHAR* source_filename,WCHAR*source_name,WCHAR*source_description){
	return LoadTextFile(source_filename);
}

bool Unity::LoadTextFile(const WCHAR*filename) {
	//OutputDebugString(L"==== Loadtext start ====");
	//OutputDebugString(filename);
	m_input.clear();

	FILE *fp = 0;
	const errno_t error = _wfopen_s(&fp, filename, L"rb");
	if (error != 0) {
		//OutputDebugString(L"@1");
		return false;
	}
	if (fp == 0) {
		return false;
	}

	bool result = false;
	std::vector<WCHAR> line;
	line.resize(64 * 1024);
	try {
		if (m_input.capacity() == 0) {
			m_input.reserve(1024);
		}
		while (true) {
			if (fgetws(&line[0], line.size(), fp) == 0) {
				break;
			}
			//OutputDebugString(line);
			m_input.emplace_back(&line[0]);
		}
		//OutputDebugString(L"@2");
		result = true;
	}
	catch (std::exception) {
		//pass
		//OutputDebugString(L"@3");
		result = false;
	}

	fclose(fp);
	//std::sort(gs_text.begin(), gs_text.end());
	//OutputDebugString(L"==== Loadtext finish ====");
	//OutputDebugString(L"@4");
	return result;
}
#endif

