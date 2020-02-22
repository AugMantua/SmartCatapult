using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;

using AUGMANSmartCatapultGUI.DataStreaming.DataHandler;
using System.IO;
using System.Drawing;

namespace AUGMANSmartCatapultGUI
{
    /// <summary>
    /// Logica di interazione per MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly BackgroundWorker receiverServerWorker;
        private DataStreaming.Receiver.server ReceiverServer;
        private byte[] Stream;
        private streamerPacketHeader packetHeader;

        public MainWindow()
        {
            this.ReceiverServer = new DataStreaming.Receiver.server();
            this.ReceiverServer.SetListener();
            /*Async receiver server #BEG*/
            receiverServerWorker = new BackgroundWorker();
            receiverServerWorker.DoWork += ReceiverServerWorker_DoWork;
            receiverServerWorker.RunWorkerCompleted += ReceiverServerWorker_RunWorkerCompleted;
            receiverServerWorker.ProgressChanged += ReceiverServerWorker_ProgressChanged;
            receiverServerWorker.WorkerReportsProgress = true;
            /*Async receiver server #END*/

            InitializeComponent();

            receiverServerWorker.RunWorkerAsync();
        }

        unsafe private void ReceiverServerWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            //this.ServerText.Text = e.UserState.ToString() + "\n";
            //Stream = Encoding.ASCII.GetBytes(e.UserState.ToString());
            packetHeader =  deserializer.getPacketHeader(e.UserState as byte[]);
            byte[] var = new byte[153600];
            int counter = 153600;
            int offset = sizeof(DataStreaming.DataHandler.streamerPacketHeader) + 66 ;
            try
            {
                for (int i = 0; i < counter; i++)
                {
                    var[i] = (e.UserState as byte[])[i + offset];
                }
            }catch(Exception)
            {
                return;
            }
            ImageHolder.Source = null;

            Bitmap bmp  = deserializer.CreateBitmap(320, 240, var, System.Drawing.Imaging.PixelFormat.Format16bppRgb565, "C:\\temp\\stream.bmp");
            bmp.RotateFlip(RotateFlipType.RotateNoneFlipY);
            ImageHolder.Source = deserializer.Convert(bmp); 

        }

        private void ReceiverServerWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            throw new NotImplementedException();
        }

        private void ReceiverServerWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while (!receiverServerWorker.CancellationPending)
            {
                byte[] tmp = this.ReceiverServer.StartListening().ToArray();
                if ( tmp.Length > 0)
                {
                    receiverServerWorker.ReportProgress(0, tmp);
                }
            }
        }
    }
}
