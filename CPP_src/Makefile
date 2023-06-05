# compiler flags:
CFLAGS  = -std=c++17 -s -Os -Wall
LIBS  = -lstdc++fs -static
DEFINES = -DWIN

TARGET = kln_installer
TARGET2 = offset_calculator
 
all: Linux Windows

Linux: $(TARGET).cpp
	g++ $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LIBS)
	g++ $(CFLAGS) -o $(TARGET2) $(TARGET2).cpp	

Windows: $(TARGET).cpp
	x86_64-w64-mingw32-g++ $(CFLAGS) $(DEFINES) -o $(TARGET).exe $(TARGET).cpp $(LIBS)
	x86_64-w64-mingw32-g++ $(CFLAGS) $(DEFINES) -o $(TARGET2).exe $(TARGET2).cpp
clean:
	$(RM) $(TARGET) $(TARGET).exe $(TARGET2) $(TARGET2).exe