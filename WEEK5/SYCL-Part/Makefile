all: output.o

output.o: main.cpp vec3.h ray.h hittable.h render_kernel.h sphere.h rtweekend.h camera.h
	icpx -fsycl main.cpp -o output.o

clean:
	rm -f output.o