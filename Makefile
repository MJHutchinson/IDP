# Automatically generated Makefile for arm
LIBPATH = lib
LIBARMPATH = lib-arm

main.arm :  sensor_interface.arm.o main.arm.o line_follow.arm.o arm_control.arm.o
	 arm-linux-gnueabi-g++ -L${LIBARMPATH} -o main.arm  sensor_interface.arm.o  main.arm.o  line_follow.arm.o  arm_control.arm.o  -lrobot

sensor_interface.arm.o: sensor_interface.cc sensor_interface.h
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I${LIBARMPATH} -I${LIBPATH} -c sensor_interface.cc -o sensor_interface.arm.o
main.arm.o: main.cc main.h
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I${LIBARMPATH} -I${LIBPATH} -c main.cc -o main.arm.o
line_follow.arm.o: line_follow.cc line_follow.h
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I${LIBARMPATH} -I${LIBPATH} -c line_follow.cc -o line_follow.arm.o
arm_control.arm.o: arm_control.cc
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I${LIBARMPATH} -I${LIBPATH} -c arm_control.cc -o arm_control.arm.o
