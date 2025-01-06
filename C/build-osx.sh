clang \
src/utils/*.c \
src/*.c \
src/graphics/*.c \
src/window/*.c \
src/world/*.c \
-Iincludes -L/opt/homebrew/lib -lglfw -framework Cocoa -framework OpenGL -framework IOKit -Wno-deprecated-declarations -W -Wall -Wextra
