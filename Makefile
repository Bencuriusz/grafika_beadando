all:
	gcc main.c camera.c -Lutils -lobj -Wall -lSOIL -lglut32 -lopengl32 -lglu32 -lm -o birkas_bence.exe

