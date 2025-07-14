# ✅ Week 3: Port the basic sphere renderer to SYCL for GPU acceleration.

Here is my GPU-track of WEEK-5. This folder has files to compile the main.cpp program in gpu by sycl.
I learnt sycl and many more in this week. This week was more amazing and interesting!!

# Note:
1. My pc is Windows,AMD Ryzen-7 hence Intel One API won't work on my GPU.
2. Thus I have made my program main.cpp to run on my CPU rather than GPU.
3. If you want to test my code or run, change Line 17 in main.cpp from
  auto queue = sycl::queue(sycl::cpu_selector_v); to   auto queue = sycl::queue(sycl::gpu_selector_v);
4. In case if you want to render image more clearly then replace the lambda function randf() with xorand() function.
