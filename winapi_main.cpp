#include <windows.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return -1;
    }

    std::string cmd = argv[1];
    for (size_t i = 2; i < argc; ++i)
    {
        cmd += " ";
        cmd += argv[i];
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    HANDLE g_hChildStd_OUT_Rd = NULL;
    HANDLE g_hChildStd_OUT_Wr = NULL;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&sa, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &sa, 0))
    {
        return -1;
    }

    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        return -1;
    }

    si.hStdOutput = g_hChildStd_OUT_Wr;

    // Uncommenting this line fixed the bug
    si.hStdError = g_hChildStd_OUT_Wr; 

    if (!CreateProcess(NULL, const_cast<LPSTR>(cmd.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        CloseHandle(g_hChildStd_OUT_Rd);
        CloseHandle(g_hChildStd_OUT_Wr);
        return -1;
    }

    CloseHandle(g_hChildStd_OUT_Wr);

    CHAR buffer[4096];
    DWORD bytesRead;
    BOOL success = FALSE;

    while (TRUE)
    {
        success = ReadFile(g_hChildStd_OUT_Rd, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        if (!success || bytesRead == 0)
        {
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(g_hChildStd_OUT_Rd);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
