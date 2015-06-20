OS := $(shell uname)
ARCH := $(shell uname -m)
SOURCES = src/main.cpp src/C_LISTENER.cpp src/C_SOCKET.cpp
EXECUTABLE_NAME = LeapMotionEngine

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY := lib/x64/libLeap.so -Wl,-rpath,lib/x64
  else
    LEAP_LIBRARY := lib/x86/libLeap.so -Wl,-rpath,lib/x86
  endif
else
  # OS X
  LEAP_LIBRARY := lib/libLeap.dylib
endif

all: 
	$(CXX) -Wall -g -I include $(SOURCES) -o $(EXECUTABLE_NAME) $(LEAP_LIBRARY)
	
ifeq ($(OS), Darwin)
	install_name_tool -change @loader_path/libLeap.dylib @executable_path/lib/libLeap.dylib LeapMotionEngine
endif

clean:
	rm -rf LeapMotionEngine LeapMotionEngine.dSYM
