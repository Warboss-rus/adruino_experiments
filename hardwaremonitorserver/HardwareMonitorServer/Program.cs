using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Runtime.InteropServices;
using OpenHardwareMonitor.Hardware;

namespace HardwareMonitorServer
{
    class Program
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct HardwareInfo
        {
            public byte cpuLoad;
            public byte cpuTemp;
            public ushort cpuFreq;
            public ushort memUsed;
            public ushort memTotal;
            public byte gpuLoad;
            public byte gpuTemp;
            public ushort gpuFreq;
            public ushort gpuMemFreq;

            public byte[] ToBytes()
            {
                byte[] bytes = new byte[Size()];
                GCHandle pinStructure = GCHandle.Alloc(this, GCHandleType.Pinned);
                try
                {
                    Marshal.Copy(pinStructure.AddrOfPinnedObject(), bytes, 0, bytes.Length);
                    return bytes;
                }
                finally
                {
                    pinStructure.Free();
                }
            }

            public int Size()
            {
                return Marshal.SizeOf(typeof(HardwareInfo));
            }
        }

        private struct Sensors
        {
            public List<ISensor> cpuTemp;
            public ISensor cpuLoad;
            public ISensor cpuFreq;
            public ISensor gpuLoad;
            public List<ISensor> gpuTemp;
            public ISensor gpuFreq;
            public ISensor gpuMemFreq;
        }

        static void Main(string[] args)
        {
            var myComputer = new Computer();
            myComputer.CPUEnabled = true;
            myComputer.GPUEnabled = true;
            myComputer.Open();

            Sensors sensors = new Sensors();
            sensors.cpuTemp = new List<ISensor>();
            sensors.gpuTemp = new List<ISensor>();
            foreach (var hardwareItem in myComputer.Hardware)
            {
                switch (hardwareItem.HardwareType)
                {
                    case HardwareType.CPU:
                        GetCpuSensors(hardwareItem, ref sensors);
                        break;
                    case HardwareType.GpuAti:
                    case HardwareType.GpuNvidia:
                        GetGPUSensors(hardwareItem, ref sensors);
                        break;
                }
            }

            string comPortName = args.Length > 0 ? args[0] : "COM3";
            SerialPort port = new SerialPort(comPortName, 9600, Parity.None, 8, StopBits.One);
            port.Open();

            HardwareInfo info = new HardwareInfo();
            while (true)
            {
                foreach (var hardwareItem in myComputer.Hardware)
                {
                    hardwareItem.Update();
                }

                info.cpuLoad = Convert.ToByte(GetSensorData(sensors.cpuLoad));
                info.cpuTemp = Convert.ToByte(GetMaxSensorValue(sensors.cpuTemp));
                info.cpuFreq = Convert.ToUInt16(GetSensorData(sensors.cpuFreq));
                info.gpuLoad = Convert.ToByte(GetSensorData(sensors.gpuLoad));
                info.gpuTemp = Convert.ToByte(GetMaxSensorValue(sensors.gpuTemp));
                info.gpuFreq = Convert.ToUInt16(GetSensorData(sensors.gpuFreq));
                info.gpuMemFreq = Convert.ToUInt16(GetSensorData(sensors.gpuMemFreq));
                GetRAMValues(ref info);


                byte[] bytes = new byte[1];
                bytes[0] = 0x3C;
                port.Write(bytes, 0, 1);
                port.Write(info.ToBytes(), 0, 8);
                Console.WriteLine("CPU: {0}% {1}°С {2}MHz, Memory: {3}/{4}MB, GPU: {5}% {6}°С {7}/{8}MHz", 
                    info.cpuLoad, info.cpuTemp, info.cpuFreq, info.memUsed, info.memTotal, info.gpuLoad, info.gpuTemp, info.gpuFreq, info.gpuMemFreq);
                System.Threading.Thread.Sleep(250);
            }
        }

        static float GetSensorData(ISensor sensor)
        {
            if (sensor != null)
            {
                if (sensor.Value.HasValue)
                {
                    return sensor.Value.Value;
                }
            }
            return 0;
        }

        static float GetMaxSensorValue(List<ISensor> sensors)
        {
            float result = 0;
            foreach (ISensor sensor in sensors)
            {
                float value = GetSensorData(sensor);
                if (value > result)
                {
                    result = value;
                }
            }
            return result;
        }

        static void GetCpuSensors(IHardware hardwareItem, ref Sensors sensors)
        {
            foreach (var sensor in hardwareItem.Sensors)
            {
                if (sensor.Value.HasValue)
                {
                    switch (sensor.SensorType)
                    {
                        case SensorType.Temperature:
                            sensors.cpuTemp.Add(sensor);
                            break;
                        case SensorType.Load:
                            if (sensor.Name.Contains("Total") || sensors.cpuLoad == null)
                            {
                                sensors.cpuLoad = sensor;
                            }
                            break;
                        case SensorType.Clock:
                            if (sensor.Index != 0)
                            {
                                sensors.cpuFreq = sensor;
                            }
                            break;
                    }
                }
            }
        }

        static void GetGPUSensors(IHardware hardwareItem, ref Sensors sensors)
        {
            foreach (var sensor in hardwareItem.Sensors)
            {
                if (sensor.Value.HasValue)
                {
                    switch (sensor.SensorType)
                    {
                        case SensorType.Temperature:
                            sensors.gpuTemp.Add(sensor);
                            break;
                        case SensorType.Load:
                            sensors.gpuLoad = sensor;
                            break;
                        case SensorType.Clock:
                            if (sensor.Name.Contains("Memory"))
                            {
                                sensors.gpuMemFreq = sensor;
                            }
                            else
                            {
                                sensors.gpuFreq = sensor;
                            }
                            break;
                    }
                }
            }
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        private class MEMORYSTATUSEX
        {
            public uint dwLength;
            public uint dwMemoryLoad;
            public ulong ullTotalPhys;
            public ulong ullAvailPhys;
            public ulong ullTotalPageFile;
            public ulong ullAvailPageFile;
            public ulong ullTotalVirtual;
            public ulong ullAvailVirtual;
            public ulong ullAvailExtendedVirtual;
            public MEMORYSTATUSEX()
            {
                dwLength = (uint)Marshal.SizeOf(typeof(MEMORYSTATUSEX));
            }
        }


        [return: MarshalAs(UnmanagedType.Bool)]
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool GlobalMemoryStatusEx([In, Out] MEMORYSTATUSEX lpBuffer);

        static void GetRAMValues(ref HardwareInfo info)
        {
            MEMORYSTATUSEX memStatus = new MEMORYSTATUSEX();
            if (GlobalMemoryStatusEx(memStatus))
            {
                info.memTotal = Convert.ToUInt16(memStatus.ullTotalPhys / 1024 / 1024);
                info.memUsed = Convert.ToUInt16((memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024);
            }
        }
    }
}
