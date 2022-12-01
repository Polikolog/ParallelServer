#pragma once
#include "Global.h"

DWORD WINAPI GarbageCleaner(LPVOID pPrm) // очистка ненужных клиентов
{
	cout << "GarbageCleaner запущен;\n" << endl;
	DWORD rc = 0;
	try
	{
		while (*((TalkersCmd*)pPrm) != Exit && &Work > 0)
		{
			EnterCriticalSection(&scListContact);

			for (auto iterator = Contacts.begin(); iterator != Contacts.end();)
			{
				if (iterator->sthread == Contact::FINISH || iterator->sthread == Contact::TIMEOUT || iterator->sthread == Contact::ABORT || iterator->type == Contact::EMPTY)
				{
					printf("IP удаленного клиента:-%s", inet_ntoa(iterator->prms.sin_addr));
					cout << " с кодом " << iterator->sthread << ";" << endl;
					if (iterator->type == Contact::EMPTY) InterlockedIncrement(&Fail);
					else
					{
						if (iterator->sthread == Contact::FINISH) InterlockedIncrement(&Finished);
						if (iterator->sthread == Contact::TIMEOUT) InterlockedIncrement(&Fail);
						if (iterator->sthread == Contact::ABORT) InterlockedIncrement(&Fail);
						CloseHandle(iterator->hthread);
						CloseHandle(iterator->htimer);
					}
					closesocket(iterator->s);
					iterator = Contacts.erase(iterator);
					InterlockedDecrement(&Work);
				}

				else iterator++;
			}
			LeaveCriticalSection(&scListContact);
			Sleep(1000);
		}
	}
	catch (string errorMsgText)
	{
		std::cout << errorMsgText << endl;
	}
	cout << "GarbageCleaner остановлен;\n" << endl;
	ExitThread(rc);
}