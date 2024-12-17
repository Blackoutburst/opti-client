REM gcc -shared -o DesktopWindow.dll -I"%JAVA_HOME%/include" -I"%JAVA_HOME%/include/win32" DesktopWindow.c
gcc -shared -o DesktopWindow.dll -IC:\Users\black\.jdks\openjdk-22.0.2\include -IC:\Users\black\.jdks\openjdk-22.0.2\include\win32 DesktopWindow.c

