#include"stdafx.h"



RecurringTask::RecurringTask() {

}

RecurringTask::~RecurringTask() {
	Destroy();
}

void RecurringTask::Destroy() {
	for (auto &task : m_tasks) {
		task->Terminate();
	}
	m_tasks.clear();
}

void RecurringTask::Update() {	
	//DebugLog(_T("RecurringTask::Update"));

	//�^�X�N�����s����
	for (auto &task:m_tasks) {
		if (task->Finished()) {
			continue;
		}
		task->Excute();
	}

	//�I�������^�X�N���폜����
	auto first = m_tasks.begin();
	auto end = m_tasks.end();
	for (; first != end;) {
		if ((*first)->Finished()) {
			auto target = first;
			++first;
			m_tasks.erase(target);
			//DebugLog(_T("  -> Erase task."));
		}
		else {
			++first;
		}
	}
	//DebugLog(_T("  -> Finish"));
}

