#include "FolderImageThread.h"

#include "FolderImageImpl.h"

FolderImageThread::FolderImageThread(FolderImageImpl* pImag)
	: imag(pImag)
{}

void FolderImageThread::run()
{
	HTask task;

	while (imag->m_tasks.remove(task))
		task->execute();
}
