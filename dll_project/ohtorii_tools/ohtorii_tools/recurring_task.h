#pragma once
#include <functional>
#include <utility>
#include<list>


class TaskBase {
public:
	virtual ~TaskBase() {};
	///���s����
	virtual void Excute()=0;	
	///�I��������
	virtual void Terminate()=0;
	///�I���������ǂ������ׂ�
	virtual bool Finished() = 0;
};


///����I�ȏ������s���N���X
class RecurringTask {
public:
	typedef std::unique_ptr<TaskBase> TaskType;
	
	RecurringTask();
	~RecurringTask();
	void Destroy();

	///���C���X���b�h�������I�ɌĂԏ���
	void Update();

	///���s����^�X�N�̓o�^
	template<typename T, class... _Valty>
	bool Register(_Valty&&... _Val) {
		try {
			m_tasks.emplace_back(std::make_unique<T>(std::forward<_Valty>(_Val)...));
			return true;
		}
		catch (std::exception) {
			//pass
		}
		return false;
	}


private:
	RecurringTask(const RecurringTask&);
	RecurringTask& operator=(const RecurringTask&);
	
	void Main();

	typedef std::list<TaskType> TaskListType;
	TaskListType  m_tasks;
};