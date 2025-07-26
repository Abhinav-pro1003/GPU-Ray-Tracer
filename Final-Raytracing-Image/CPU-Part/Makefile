all: output.o

output.o: main.cpp vec3.h ray.h color.h hittable.h hittable_list.h sphere.h rtweekend.h camera.h interval.h material.h
	g++ main.cpp -o output.o

clean:
	rm -f output.o