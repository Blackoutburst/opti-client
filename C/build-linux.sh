clang \
src/utils/*.c \
src/*.c \
src/graphics/*.c \
src/window/*.c \
src/world/*.c \
src/network/*.c \
src/core/*.c \
src/ui/*.c \
src/entity/*.c \
-Iincludes -L/usr/lib -pthread -lglfw -lGL -Wno-deprecated-declarations -W -Wall -Wextra -Wno-implicit-fallthrough -Wno-unused-parameter -g3 -fno-omit-frame-pointer -fsanitize=address

