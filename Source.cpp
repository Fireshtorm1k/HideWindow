#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <wchar.h> 
#include <stdio.h>



void getAllProcessIdsByProcNameW(LPCWSTR lpczProcName, std::vector<DWORD>* pidsVec)
{
    pidsVec->clear(); 
    PROCESSENTRY32W procEntry = { sizeof(PROCESSENTRY32W) };
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
    if (Process32FirstW(hSnapshot, &procEntry)) 
    {
        while (Process32NextW(hSnapshot, &procEntry)) 
        {
            
            if (wcscmp(procEntry.szExeFile, lpczProcName) == 0)
            {
                pidsVec->push_back(procEntry.th32ProcessID);
            }
        }
    }

    CloseHandle(hSnapshot); 
}


void getAllWindowHandlesByPyPidsVec(const std::vector<DWORD>* pidsVec, std::vector<HWND>* hwndVec)
{
    hwndVec->clear(); 
    HWND hCurrentWindow = nullptr; 
    DWORD dwProcessID = 0; 

    for (size_t i = 0; i < pidsVec->size(); i++) 
    {
        while (true)
        {
            hCurrentWindow = FindWindowExW(nullptr, hCurrentWindow, nullptr, nullptr); 
           
            if (hCurrentWindow == nullptr) 
            {
                break; 
            } и
            else
            {
                GetWindowThreadProcessId(hCurrentWindow, &dwProcessID); 
                
                if (dwProcessID == (*pidsVec)[i]) 
                {
                    hwndVec->push_back(hCurrentWindow); 
                }
            }
        }
    }
}


int main()
{
    LPCWSTR lpczProcName = L"YourApp.exe";
    std::vector<DWORD> pidsVec{}; 
    std::vector<HWND> hwndVec{}; 

    getAllProcessIdsByProcNameW(lpczProcName, &pidsVec);
    if (!pidsVec.empty()) 
    {
        getAllWindowHandlesByPyPidsVec(&pidsVec, &hwndVec);
        if (!hwndVec.empty())
        {
            for (size_t i = 0; i < hwndVec.size(); i++)
            {
                
                wprintf(L"%p\n", hwndVec[i]);
                HideWindow(hwndVec[i], SW_HIDE);
            }
        }

        else wprintf(L"The PIDs haven't visible (no only message) windows.\n"); 
    }

    else wprintf(L"There're no processes with specified name.\n"); 
    return 0;
}
