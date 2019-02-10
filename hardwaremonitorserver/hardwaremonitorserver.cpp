#include "TCHAR.h"
#include "pdh.h"
#include <stdio.h>
#include <cmath>
#include <powerbase.h>

#pragma pack(push, 1)
struct HardwareInfoData
{
	struct Cpu
	{
		unsigned char load;
		unsigned char temp;
		unsigned short freq;
	} cpu;
	struct Memory
	{
		unsigned short usedMb;
		unsigned short totalMb;
	} mem;
};
#pragma pack(pop)
const unsigned char startByte = 0x3C;

typedef struct _PROCESSOR_POWER_INFORMATION {
	ULONG Number;
	ULONG MaxMhz;
	ULONG CurrentMhz;
	ULONG MhzLimit;
	ULONG MaxIdleState;
	ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

unsigned char GetCpuLoad()
{
	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	return unsigned char(round(counterVal.doubleValue));
}

unsigned short GetCpuClock()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	auto power = new PROCESSOR_POWER_INFORMATION[info.dwNumberOfProcessors];
	CallNtPowerInformation(ProcessorInformation, NULL, 0, power, sizeof(PROCESSOR_POWER_INFORMATION) * info.dwNumberOfProcessors);
	unsigned short result = unsigned short(power[0].CurrentMhz);
	delete[] power;
	return result;
}

HardwareInfoData::Memory GetMemoryInfo()
{
	MEMORYSTATUSEX status = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&status);
	return { unsigned short((status.ullTotalPhys - status.ullAvailPhys) / 1024ull / 1024ull), unsigned short(status.ullTotalPhys / 1024ul / 1024ul) };
}

void WriteToSerial(HANDLE com, const void* data, size_t size)
{
	DWORD written = 0;
	WriteFile(com, data, DWORD(size), &written, NULL);
}

void InitCom(HANDLE com)
{
	DCB dcbSerialParams = { 0 }; // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	auto Status = GetCommState(com, &dcbSerialParams);
	dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None
	SetCommState(com, &dcbSerialParams);

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
	SetCommTimeouts(com, &timeouts);
}

int main(int argc, const char* argv[])
{
	const char* serialName = argc > 1 ? argv[1] : "COM3";
	HANDLE com = CreateFileA(serialName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	InitCom(com);
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);

	while (true) 
	{
		HardwareInfoData data = {};
		data.cpu.load = GetCpuLoad();
		data.cpu.freq = GetCpuClock();
		data.mem = GetMemoryInfo();

		printf("cpu=%u%%, %uMHz; ram=%u/%uMB\n", data.cpu.load, data.cpu.freq, data.mem.usedMb, data.mem.totalMb);

		WriteToSerial(com, &startByte, 1);
		WriteToSerial(com, &data, 8);

		Sleep(250);
	}
	CloseHandle(com);
	return 0;
}