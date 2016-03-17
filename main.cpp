/*
 * main.cpp
 *
 *  Created on: Mar 16, 2016
 *      Author: Tra Quang Kieu
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <time.h>

#include <signal.h>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;


struct __Time
{
	int hour;
	int minute;
	int second;
};

int save_time(int _code, struct __Time * time);
int read_time(int _code, struct __Time * time);


static inline void newline() { printf("\r\n"); };

#define DEBUG 1

void die(int code)
{
	printf("Program exit with code: %d.", code);
	newline();
	exit(code);
}

bool skip = false;


int main(int argc, char * argv[])
{
	signal(SIGINT, die);

	if (getuid())
	{
		printf("Program must be run as root.");
		newline();
		exit (1);
	}
	else
	{

	}

	time_t shutdown_time, current_time;

#if defined(WIN32)
	struct tm halt;
#else
	struct tm * halt;
#endif

	time(&shutdown_time);
	time(&current_time);

	struct __Time halt_time;

	// recover halt time
	read_time(0, &halt_time);
	if (halt_time.hour > 60 || halt_time.hour < 0 || \
		halt_time.minute > 60 || halt_time.minute < 0 || \
		halt_time.second > 60 || halt_time.second < 0)
	{
		halt_time.hour = 17;
		halt_time.minute = 0;
		halt_time.second = 0;
		save_time(0, &halt_time);
	}

#if defined (WIN32)
	localtime_s(&halt, &shutdown_time);
#else
	halt = localtime(&shutdown_time);
#endif
	halt->tm_hour = halt_time.hour;
	halt->tm_min = halt_time.minute;
	halt->tm_sec = halt_time.second;
	shutdown_time = mktime(halt);

	char * time_str = (char *)malloc(100);
#if defined(WIN32)
	ctime_s(time_str, 100, &current_time);
#else
	time_str = ctime(&current_time);
#endif
	printf("Program start at: %s", time_str);
	newline();

#if defined(WIN32)
	ctime_s(time_str, 100, &shutdown_time);
#else
	time_str = ctime(&shutdown_time);
#endif
	printf("System will be halt at: %s", time_str);
	newline();

#if DEBUG
	for (int i = 0; i < argc; i++)
	{
		printf("Params[%d]: %s.", i, argv[i]);
		newline();
	}
#endif

	if (argc > 1)
	{
		if (memcmp(argv[1], "-c", 2) == 0)
		{
#if DEBUG
			printf("Change halt time.");
			newline();
#endif
			// change shutdown time
#if defined (WIN32)
			sscanf_s(argv[2], "%d-%d-%d", &halt.tm_hour, &halt.tm_min, &halt.tm_sec);
			sscanf_s(argv[2], "%d-%d-%d", &halt_time.hour, &halt_time.minute, &halt_time.second);
#else
			sscanf(argv[2], "%d-%d-%d", &halt->tm_hour, &halt->tm_min, &halt->tm_sec);
			sscanf(argv[2], "%d-%d-%d", &halt_time.hour, &halt_time.minute, &halt_time.second);
#endif
			shutdown_time = mktime(halt);
			save_time(0, &halt_time);

#if defined(WIN32)
	ctime_s(time_str, 100, &shutdown_time);
#else
	time_str = ctime(&shutdown_time);
#endif
	newline();
	printf("System will be halt at: %s", time_str);
	newline();

		}
		else if (memcmp(argv[1], "-a", 2) == 0)
		{
#if DEBUG
			printf("Skip halt.");
#endif
			skip = true;
		}
		else if (memcmp(argv[1], "-u", 2) == 0)
		{
#if DEBUG
			printf("Un-skip halt.");
#endif
			skip = false;
		}
		else if (memcmp(argv[1], "-h", 2) == 0)
		{
#if DEBUG
			printf("Help command.");
			newline();
#endif
			printf("Command format: <cmd> <params>");
			newline();
			printf("Command available:");
			newline();
			printf("-h : help");
			newline();
			printf("-c : change halt time.");
			newline();
			printf("-a : skip today halt.");
			newline();
			printf("-u : un-skip today halt.");
			newline();
			printf("Time format: HH-MM-SS");
			newline();
		}
		else
		{
#if DEBUG
			printf("Normal startup.");
			newline();
#endif
		}
	}

#if TEST
	// test file
	ctime_s(time_str, 100, &start_time);
	printf("Save time: %s", time_str);
	save_time(0, start_time);
	read_time(0, &shutdown_time);
	ctime_s(time_str, 100, &shutdown_time);
	printf("File time: %s", time_str);
#endif

	for (;;)
	{
		time(&current_time);
		switch ((int)(difftime(shutdown_time, current_time) / 1))
		{
		case 300:
			printf("System will be shutdown in 5 minutes.");
			newline();
			break;
		case 3:
			printf("System will be shutdown in 3 seconds.");
			newline();
			break;
		case 2:
			printf("System will be shutdown in 2 seconds.");
			newline();
			break;
		case 1:
			printf("System will be shutdown in 1 seconds.");
			newline();
			break;
		case 0:
			printf("System will be shutdown in 0 seconds.");
			newline();
#if defined(WIN32)
			system("shutdown -s -t 0");
#else
			system("shutdown -h now");
#endif
			exit(1);
			break;
		default:
#if DEBUG
			printf("Time elapse: %ds", (int)(difftime(shutdown_time, current_time) / 1));
			newline();
#endif
			break;
		}
#if defined(WIN32)
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	return 0;
}

int save_time(int _code, struct __Time * time)
{
	ofstream f;
	f.open("/tqk.db", ios::binary);
	f.seekp(_code);
	f.write((char *)time, sizeof(struct __Time));
	f.close();
	return 0;
}

int read_time(int _code, struct __Time * time)
{
	ifstream f;
	f.open("/tqk.db", ios::binary);
	f.seekg(_code);
	f.read((char *)time, sizeof(struct __Time));
	return 0;
}

