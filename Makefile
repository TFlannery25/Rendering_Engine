CXX = g++
CXXFLAGS = -I/mingw64/include/SDL2 -Iinclude 
LDFLAGS = -L/mingw64/lib -lSDL2 -lopengl32

app.exe: main.cpp src/glad.c src/Shader.cpp src/Mesh.cpp src/Transform.cpp src/Object.cpp
	$(CXX) main.cpp src/glad.c src/Shader.cpp src/Mesh.cpp src/Transform.cpp src/Object.cpp -o app.exe $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f app.exe
