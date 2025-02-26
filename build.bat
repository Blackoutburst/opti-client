@echo off
set NAME=game
set SRC=src\*.c src\renderer\*.c src\window\*.c src\utils\*.c src\debug\*.c
set INCLUDE=-Iinclude

set I_VK=%USERPROFILE%\vulkan\1.4.304.1\Include
set L_VK=%USERPROFILE%\vulkan\1.4.304.1\Lib
set VK=-I"%I_VK%" -L"%L_VK%" -lvulkan-1

set I_GLFW=%USERPROFILE%\glfw\include
set L_GLFW=%USERPROFILE%\glfw\lib-mingw-w64
set GLFW=-I"%I_GLFW%" -L"%L_GLFW%" -lglfw3dll


set F_ERROR=-Wall -Wextra -Wpedantic
set F_DEBUG=-g3 -fsanitize=address
set F_DISABLED=

rem GCC
echo GCC:
gcc -o %NAME% %SRC% %INCLUDE% %VK% %GLFW% %F_ERROR% %F_DEBUG% %F_DISABLED%
set GCC_EXIT=%ERRORLEVEL%

if %GCC_EXIT% neq 0 (
    echo FAILED
) else (
    echo OK
)

rem CLANG
echo CLANG:
clang -o %NAME% %SRC% %INCLUDE% %VK% %GLFW% %F_ERROR% %F_DEBUG% %F_DISABLED%
set CLANG_EXIT=%ERRORLEVEL%

if %CLANG_EXIT% neq 0 (
    echo FAILED
) else (
    echo OK
)

