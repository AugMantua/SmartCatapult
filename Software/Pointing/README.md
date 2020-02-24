# Sistema puntamento

## Table of Contents
- [Link utili](#Link)
  - [ESP32](#esp32)




## Link

### Librerie / progetti open

Convolutional Neural Networks [Darknet](https://github.com/pjreddie/darknet).
YOLO(You Only Look Once) [YOLO](https://github.com/pjreddie/darknet/wiki/YOLO:-Real-Time-Object-Detection).
YOLO Wrapper - [Alturos.Yolo](https://github.com/AlturosDestinations/Alturos.Yolo)

### Directory Structure

You should have this files in your program directory.

    .
    ├── Alturos.Yolo.dll              # C# yolo wrapper
    ├── x64/
    │   ├── yolo_cpp_dll_cpu.dll      # yolo runtime for cpu
    │   ├── yolo_cpp_dll_gpu.dll      # yolo runtime for gpu
    │   ├── cudnn64_7.dll             # required by yolo_cpp_dll_gpu (optional only required for gpu processig)
    │   ├── opencv_world340.dll       # required by yolo_cpp_dll_xxx (process image as byte data detect_mat)
    │   ├── pthreadGC2.dll            # required by yolo_cpp_dll_xxx (POSIX Threads)
    │   ├── pthreadVC2.dll            # required by yolo_cpp_dll_xxx (POSIX Threads)
    │   ├── msvcr100.dll              # required by pthread (POSIX Threads)







