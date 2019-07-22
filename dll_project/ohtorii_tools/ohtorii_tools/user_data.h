#pragma once
#include<string>
#include<vector>
#include<unordered_map>


class UserData {
public:
	/**���̃��[�U�[�f�[�^��ݒ肷��i������Łj
	����L�[������ꍇ�͏㏑�����܂��B
	return bool	true	����
				false	���s
	*/
	template<class Archive> void serialize(Archive & archive) {
		archive(m_string);
	};
	bool			SetUserData(const WCHAR* key, const WCHAR*data);
	const WCHAR*	GetUserData(const WCHAR* key, const WCHAR*	default_data = _T(""))const;
private:
	///���[�U�[�f�[�^�i������j
	std::unordered_map<std::wstring, std::wstring>	m_string;
};
