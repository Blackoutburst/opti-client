clang \
src/utils/*.c \
src/*.c \
src/graphics/*.c \
src/window/*.c \
src/world/*.c \
src/network/*.c \
src/core/*.c \
-Iincludes -L/opt/homebrew/lib -pthread -lglfw -framework Cocoa -framework OpenGL -framework IOKit -Wno-deprecated-declarations -W -Wall -Wextra -Wno-unused-parameter -g3 -fno-omit-frame-pointer -fsanitize=address
