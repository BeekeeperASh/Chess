#ifndef CONNECTORA_H
#define CONNECTORA_H

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

STARTUPINFO stiA = { 0 };
SECURITY_ATTRIBUTES satsA = { 0 };
PROCESS_INFORMATION piA = { 0 };
HANDLE pipin_wA, pipin_rA, pipout_wA, pipout_rA;
BYTE bufferA[65536];
DWORD writA, excodeA, readA, availableA;


void ConnectToEngineA(WCHAR* path)
{
    pipin_wA = pipin_rA = pipout_wA = pipout_rA = NULL;
    satsA.nLength = sizeof(satsA);
    satsA.bInheritHandle = TRUE;
    satsA.lpSecurityDescriptor = NULL;

    CreatePipe(&pipout_rA, &pipout_wA, &satsA, 0);
    CreatePipe(&pipin_rA, &pipin_wA, &satsA, 0);

    stiA.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    stiA.wShowWindow = SW_HIDE;
    stiA.hStdInput = pipin_rA;
    stiA.hStdOutput = pipout_wA;
    stiA.hStdError = pipout_wA;

    CreateProcess(NULL, path, NULL, NULL, TRUE, 0, NULL, NULL, &stiA, &piA);
}


std::string getNextMoveA(std::string position)
{
    std::string str;
    position = "position startpos moves " + position + "\ngo\n";

    WriteFile(pipin_wA, position.c_str(), position.length(), &writA, NULL);
    Sleep(300);

    PeekNamedPipe(pipout_rA, bufferA, sizeof(bufferA), &readA, &availableA, NULL);
    do
    {
        ZeroMemory(bufferA, sizeof(bufferA));
        if (!ReadFile(pipout_rA, bufferA, sizeof(bufferA), &readA, NULL) || !readA) break;
        bufferA[readA] = 0;
        str += (char*)bufferA;
    } while (readA >= sizeof(bufferA));

    int n = str.find("bestmove");
    if (n == -1) return "error";

    return str.substr(n + 9, 6);
}


void CloseConnectionA()
{
    WriteFile(pipin_wA, "quit\n", 5, &writA, NULL);
    if (pipin_wA != NULL) CloseHandle(pipin_wA);
    if (pipin_rA != NULL) CloseHandle(pipin_rA);
    if (pipout_wA != NULL) CloseHandle(pipout_wA);
    if (pipout_rA != NULL) CloseHandle(pipout_rA);
    if (piA.hProcess != NULL) CloseHandle(piA.hProcess);
    if (piA.hThread != NULL) CloseHandle(piA.hThread);
}


#endif CONNECTORA_H