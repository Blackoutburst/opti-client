clang \
src/utils/*.c \
src/*.c \
src/graphics/*.c \
src/window/*.c \
src/world/*.c \
src/network/*.c \
-Iincludes -L/usr/lib -pthread -lglfw -lGL -Wno-deprecated-declarations -W -Wall -Wextra -Wno-unused-parameter -g3 -fno-omit-frame-pointer -fsanitize=address

