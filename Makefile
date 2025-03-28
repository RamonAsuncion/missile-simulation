all:
	g++ -std=c++17 *.cpp -o missile-simulator -I/opt/homebrew/Cellar/sfml/3.0.0_1/include -L/opt/homebrew/Cellar/sfml/3.0.0_1/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f missile-simulator
