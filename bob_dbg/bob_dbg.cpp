/**
 * @file    main.cpp
 * @brief   Debugger application's entry point.
 *
 * @author  Yonhgwhan, Noh (fixbrain@gmail.com)
 * @date    2015.11.17 11:17 created.
 * @copyright All rights reserved by Yonghwan, Noh.
**/

#include "stdafx.h"

/// @brief
int main()
{
    // create process with /dbg
    STARTUPINFO			si = {0};
	PROCESS_INFORMATION pi = {0};
    wchar_t* target = L"c:\\windows\\system32\\calc.exe";


	if (TRUE != CreateProcessW(
					target, 
					NULL, 
					NULL, 
					NULL, 
					FALSE, 
					CREATE_NEW_CONSOLE | DEBUG_ONLY_THIS_PROCESS, 
					NULL, 
					NULL, 
					&si, 
					&pi))
	{
        log("CreateProcess() failed. gle = %u", GetLastError());
		return -1;
	}

    log("process created. pid = %u", pi.dwProcessId);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

    DEBUG_EVENT debug_event = {0};	
	while (true)
	{
		if (TRUE != WaitForDebugEvent(&debug_event, 100)) continue;

		//> debug callback ȣ�� 
		//DWORD continue_status = _debug_callback(&debug_event, _debug_callback_tag);
        char* str = NULL;
        switch (debug_event.dwDebugEventCode)
        {
        case EXCEPTION_DEBUG_EVENT: str = "EXCEPTION_DEBUG_EVENT"; break;
        case CREATE_THREAD_DEBUG_EVENT:  str = "CREATE_THREAD_DEBUG_EVENT"; break;
        case CREATE_PROCESS_DEBUG_EVENT: str = "CREATE_PROCESS_DEBUG_EVENT"; break;
        case EXIT_THREAD_DEBUG_EVENT:	 str = "EXIT_THREAD_DEBUG_EVENT"; break;
        case EXIT_PROCESS_DEBUG_EVENT:	 str = "EXIT_PROCESS_DEBUG_EVENT"; break;
        case LOAD_DLL_DEBUG_EVENT:		 str = "LOAD_DLL_DEBUG_EVENT"; break;
        case UNLOAD_DLL_DEBUG_EVENT:	 str = "UNLOAD_DLL_DEBUG_EVENT"; break;
        case OUTPUT_DEBUG_STRING_EVENT:	 str = "OUTPUT_DEBUG_STRING_EVENT"; break;
        case RIP_EVENT:					 str = "RIP_EVENT"; break;
        default:
            str = "unknown";
        }

        log("debug event code = %s", str);
		//> continue_status ����
		//>
		//>	DBG_CONTINUE
		//>		EXCEPTION_DEBUG_EVENT �� ���
		//>			��� exception processing �� ���߰� �����带 ��� ����
		//>		EXCEPTION_DEBUG_EVENT �� �ƴ� ��� 
		//>			������ ��� ���� 
		//>
		//>	DBG_EXCEPTION_NOT_HANDLED
		//>		EXCEPTION_DEBUG_EVENT �� ���
		//>			exception processing �� ��� ����.
		//>			first-chance exception �� ��� seh �ڵ鷯�� search, dispatch ������ ����
		//>			first-chance exception �� �ƴ϶�� ���μ����� ���� ��
		//>		EXCEPTION_DEBUG_EVENT �� �ƴ� ��� 
		//>			������ ��� ���� 
        DWORD continue_status = DBG_CONTINUE;

        ContinueDebugEvent(
            debug_event.dwProcessId,
            debug_event.dwThreadId,
            continue_status);

		////> debuggee is terminating
		//if (EXIT_PROCESS_DEBUG_EVENT == debug_event.dwDebugEventCode)
		//{
		//	_stop_debug_loop = true;
		//	continue;
		//}
	}
}

