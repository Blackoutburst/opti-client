#!/bin/bash
readonly NAME="game"
readonly SRC="src/*.c src/renderer/*.c src/window/*.c src/utils/*.c src/debug/*.c src/devices/*.c"
readonly INCLUDE="-Iinclude"

readonly I_VK="$HOME/vulkan/1.4.304.1/macOS/include"
readonly L_VK="$HOME/vulkan/1.4.304.1/macOS/lib"
readonly VK="-I$I_VK -L$L_VK -lvulkan -Wl,-rpath,$L_VK"

readonly I_GLFW="$HOME/glfw/include/"
readonly L_GLFW="$HOME/glfw/lib-arm64"
readonly GLFW="-I$I_GLFW -L$L_GLFW -lglfw3"

readonly F_ERROR="-Wall -Wextra -Wpedantic"
readonly F_DEBUG="-g3 -fno-omit-frame-pointer -fsanitize=address -fsanitize-address-use-after-return=always"
readonly F_DISABLED=""
readonly FRAMEWORKS="-framework Cocoa -framework IOKit"

### GCC ###

printf "\e[94mGCC\e[0m: "
gcc -o $NAME $SRC $INCLUDE $VK $GLFW $F_ERROR $F_DEBUG $F_DISABLED $FRAMEWORKS
GCC_EXIT=$?

if [ $GCC_EXIT -ne 0 ]; then
    printf "\e[91mFAILED\e[0m\n"
else
    printf "\e[92mOK\e[0m\n"
fi

### CLANG ###

printf "\e[94mCLANG\e[0m: "
clang -o $NAME $SRC $INCLUDE $VK $GLFW $F_ERROR $F_DEBUG $F_DISABLED $FRAMEWORKS
CLANG_EXIT=$?

if [ $CLANG_EXIT -ne 0 ]; then
    printf "\e[91mFAILED\e[0m\n"
else
    printf "\e[92mOK\e[0m\n"
fi

