#include <windows.h>
#include <jni.h>
#include <stdio.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
    HWND defView = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL);
    if (defView) {
        HWND* workerw = (HWND*)lparam;
        *workerw = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
        return FALSE;
    }
    return TRUE;
}

HWND FindWorkerW() {
    HWND progman = FindWindow("Progman", NULL);
    if (!progman) {
        OutputDebugString("FindWindow(\"Progman\", NULL) failed.\n");
        return NULL;
    }

    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
    OutputDebugString("Sent message to Progman to spawn WorkerW.\n");

    HWND workerw = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)&workerw);

    if (!workerw) {
        OutputDebugString("EnumWindows failed to find WorkerW.\n");
    } else {
        OutputDebugString("WorkerW window found.\n");
    }

    return workerw;
}

BOOL GetDesktopSize(int* width, int* height) {
    RECT desktopRect;
    const HWND hDesktop = GetDesktopWindow();
    if (!GetWindowRect(hDesktop, &desktopRect)) {
        OutputDebugString("GetWindowRect failed.\n");
        return FALSE;
    }
    *width = desktopRect.right - desktopRect.left;
    *height = desktopRect.bottom - desktopRect.top;
    char buffer[100];
    sprintf(buffer, "Desktop size: %d x %d\n", *width, *height);
    OutputDebugString(buffer);
    return TRUE;
}

JNIEXPORT jboolean JNICALL Java_dev_blackoutburst_game_window_Window_attachToWorkerW(JNIEnv* env, jclass clazz, jlong hwnd) {
    HWND workerw = FindWorkerW();
    if (!workerw) {
        OutputDebugString("FindWorkerW returned NULL.\n");
        return JNI_FALSE;
    }

    if (SetParent((HWND)hwnd, workerw) == NULL) {
        DWORD error = GetLastError();
        char buffer[256];
        sprintf(buffer, "SetParent failed with error: %d\n", error);
        OutputDebugString(buffer);
        return JNI_FALSE;
    } else {
        OutputDebugString("SetParent succeeded.\n");
    }

    LONG style = GetWindowLong((HWND)hwnd, GWL_STYLE);
    style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
    if (SetWindowLong((HWND)hwnd, GWL_STYLE, style) == 0) {
        DWORD error = GetLastError();
        char buffer[256];
        sprintf(buffer, "SetWindowLong (GWL_STYLE) failed with error: %d\n", error);
        OutputDebugString(buffer);
        return JNI_FALSE;
    } else {
        OutputDebugString("SetWindowLong (GWL_STYLE) succeeded.\n");
    }


    LONG exStyle = GetWindowLong((HWND)hwnd, GWL_EXSTYLE);
    exStyle &= ~(WS_EX_APPWINDOW);
    exStyle |= WS_EX_TOOLWINDOW;
    if (SetWindowLong((HWND)hwnd, GWL_EXSTYLE, exStyle) == 0) {
        DWORD error = GetLastError();
        char buffer[256];
        sprintf(buffer, "SetWindowLong (GWL_EXSTYLE) failed with error: %d\n", error);
        OutputDebugString(buffer);
        return JNI_FALSE;
    } else {
        OutputDebugString("SetWindowLong (GWL_EXSTYLE) succeeded.\n");
    }


    int width, height;
    if (!GetDesktopSize(&width, &height)) {
        OutputDebugString("GetDesktopSize failed.\n");
        return JNI_FALSE;
    }


    if (!SetWindowPos(
        (HWND)hwnd,
        HWND_BOTTOM,
        0,
        0,
        width,
        height,
        SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED
    )) {
        DWORD error = GetLastError();
        char buffer[256];
        sprintf(buffer, "SetWindowPos failed with error: %d\n", error);
        OutputDebugString(buffer);
        return JNI_FALSE;
    } else {
        OutputDebugString("SetWindowPos succeeded.\n");
    }

    return JNI_TRUE;
}

