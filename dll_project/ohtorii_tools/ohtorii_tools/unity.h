﻿#pragma once
#include<vector>
#include<string>
#include<array>
#include"define.h"
#include"sources.h"
#include"file.h"
#include"candidates.h"
#include"refine_search.h"
#include"kinds.h"
#include"inheritance.h"
#include"user_data.h"
#include"static_status.h"
#include"async_files.h"



class Unity{
public:
	Unity();
	~Unity();

	static std::weak_ptr<Unity> Instance(size_t index= UNITY_USE_CURRENT_INSTANCE);
	static bool PushContext(bool exist_context_then_delete =true);
	static bool PopContext(bool exist_context_then_delete = true);
	static size_t GetCurrentInstanceIndex();
	static bool SerializeCurrentContext(const WCHAR*output_filename);
	static bool DeSerializeToCurrentContext(const WCHAR*input_filename);
	static bool SerializeStaticStatusContext(const WCHAR*output_filename);
	static bool DeSerializeToStaticStatusContext(const WCHAR*input_filename);
	static void Destroy();
	
	///候補に変更あり
	void ChangeCandidates();
	///候補に変更があるかどうか
	bool HasChangedCandidates();
	///候補に変更なし、かつ、直前の状態を返す
	bool ClearChangedCandidatesAndReturnPrevStatus();


	/*ファイルリストのファイル名を設定する
	return	bool	true	成功
					false	失敗
	*/
	//INT_PTR SetCandidateList(WCHAR* source_filename,WCHAR*source_name,WCHAR*source_description);
	
	/*
	search_words	スペース区切りの検索文字列
					(Ex.) "c: programs .txt"
	返値		マッチした文字列
	*/
	//WCHAR* Filter(WCHAR* search_words);
	
	void AppendAfterDelete(const WCHAR*filename){}
	
	Sources& 		QuerySources();
	File&			QueryFile();
	Candidates&		QueryCandidates();
	RefineSearch&	QueryRefineSearch();
	Kinds&			QueryKinds();	
	Inheritance&	QueryInheritance();
	UserData&		QueryUserData();
	StaticStatus&	QueryStaticStatus();
	ASyncFiles&		QueryASyncFiles();

	template<class Archive> void serialize(Archive & archive) {
		archive(m_candidates,m_refine_search,m_user_data);
	};

protected:
private:
	Unity(const Unity&);
	Unity &operator=(const Unity&);


	static	std::array<std::shared_ptr<Unity>, UNITY_MAX_CONTEXT_NUM>	m_instances;
	static	size_t					m_current_instance_index;
	bool			m_changed_candidates;

	static Sources		m_sources;
	File				m_file;
	Candidates			m_candidates;
	RefineSearch		m_refine_search;
	Inheritance			m_inheritance;	
	UserData			m_user_data;
	ASyncFiles			m_async_files;
	///状態
	static StaticStatus		m_status;

	///カインドはインスタンス共通で利用するため静的領域とする
	static Kinds		m_kinds;	

	//std::vector<Candidate>	m_input;
	//HidemaruView				m_hidemaru_view;
	
	
	//bool LoadTextFile(const WCHAR*filename);
	
};
