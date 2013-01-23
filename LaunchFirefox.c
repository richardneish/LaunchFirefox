#include <windows.h>
#include <stdio.h>
#include <tchar.h>

/**
 * Build a command line that looks like\;
 *   "c:\Program Files (x86)\Mozilla Firefox\firefox.exe" \ 
 *     -P " Richard" \ 
 *     -remote "openURL(<URL_FROM_argv[1]>)"
 */


#define CMD_TEMPLATE "\"C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe\" -P \"Richard\" \"%s\""
#define MAX_CMD_LENGTH 32767

int CALLBACK WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow
)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    TCHAR cmdline[MAX_CMD_LENGTH];

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( lpCmdLine == NULL || strlen(lpCmdLine) == 0 )
    {
        GetModuleFileName(NULL, cmdline, MAX_CMD_LENGTH);
        MessageBox(NULL, "Usage: LaunchFirefox [url]", NULL, 0L);
        return;
    }
    sprintf(cmdline, CMD_TEMPLATE, lpCmdLine);

    // Start the child process. 
    if( !CreateProcess( NULL,   // Module name
        cmdline,          // Command line
        NULL,             // Process handle not inheritable
        NULL,             // Thread handle not inheritable
        FALSE,            // Set handle inheritance to FALSE
        0,                // No creation flags
        NULL,             // Use parent's environment block
        NULL,             // Use parent's starting directory 
        &si,              // Pointer to STARTUPINFO structure
        &pi )             // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

