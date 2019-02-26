using System;
using System.Drawing;
using System.IO.Ports;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace COMDisplay
{
    public partial class Form1 : Form
    {
        private SerialPort port;
        private const byte startMarker = 0x4E;
        private bool startMarkerReceived = false;
        private byte action = 0;
        private Bitmap img;

        private const byte actionSetPixel = 1; // byte x, byte y, byte set
        private const byte actionClear = 2; // byte width, byte height
        private int SetPixelActionSize = 3;
        private int ClearActionSize = 2;

        public Form1(string[] args)
        {
            string comPortName = args.Length > 0 ? args[0] : "COM3";
            port = new SerialPort(comPortName, 9600, Parity.None, 8, StopBits.One);
            port.DataReceived += DataReceived;
            port.Open();
            Clear(128, 64);
            InitializeComponent();
            SetStyle(ControlStyles.Opaque, true);
        }

        private void DataReceived(object s, SerialDataReceivedEventArgs e)
        {
            if (!startMarkerReceived)
            {
                while (port.BytesToRead > 0)
                {
                    if (Convert.ToByte(port.ReadByte()) == startMarker)
                    {
                        startMarkerReceived = true;
                        break;
                    }
                }
            }
            if (startMarkerReceived && action == 0 && port.BytesToRead > 0)
            {
                action = Convert.ToByte(port.ReadByte());
                if (action > actionClear)
                {
                    action = 0;
                    startMarkerReceived = false;
                }
            }
            if (startMarkerReceived && action == actionSetPixel && port.BytesToRead >= SetPixelActionSize)
            {
                byte x = Convert.ToByte(port.ReadByte());
                byte y = Convert.ToByte(port.ReadByte());
                bool set = port.ReadByte() != 0;
                SetPixel(x, y, set);
                action = 0;
                startMarkerReceived = false;
            }
            if (startMarkerReceived && action == actionClear && port.BytesToRead >= ClearActionSize)
            {
                byte x = Convert.ToByte(port.ReadByte());
                byte y = Convert.ToByte(port.ReadByte());
                Clear(x, y);
                action = 0;
                startMarkerReceived = false;
            }
        }

        private void SetPixel(byte x, byte y, bool set)
        {
            Invoke(new Action(() =>
            {
                img.SetPixel(x, y, set ? Color.Black : Color.White);
                Invalidate();
            }));
        }

        private void Clear(int width, int height)
        {
            if (img != null)
            {
                img.Dispose();
            }
            img = new Bitmap(width, height);
            using (Graphics g = Graphics.FromImage(img))
            {
                g.Clear(Color.White);
            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            using (Graphics g = CreateGraphics())
            {
                g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                int scale = Math.Min(ClientSize.Width / img.Width, ClientSize.Height / img.Height);
                g.DrawImage(img, 0, 0, img.Width * scale, img.Height * scale);
            }
        }
    }
}
