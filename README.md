To compile this project :

Download the outdated "Dev-Cpp 5.1.1 TDM-GCC 4.9.2 Setup.exe"
Install it and exit.

Download the freeglut archive file from this link click on freeglut for MinGW to download the Martin Payne binaries. https://www.transmissionzero.co.uk/software/freeglut-devel/ 

Download and extract freeglut-MinGW-3.0.0-1 archive to get into the freeglut folder.

1. Enter the freeglut folder and copy the files from freeglut/include/GL/
File 1 freeglut.h
File 2 freeglut_ext.h
File 3 freeglut_std.h
File 4 glut.h

To
C:Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include/GL

And from freeglut/lib/x64
File 1 libfreeglut.a
File 2 libfreeglut_static.a

To
C:Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib

and from freeglut/bin/x64
freeglut.dll

To C/Windows/System32

Make a new Console Application project and Right click the project in the project list tree. Right click ->Project Options and go to the parameters tab. Then add 

-lopengl32
-lfreeglut
-lglu32

Then delete all text in your main file and copy and paste the code inside the main.cpp file into your main file and compile and run it.
