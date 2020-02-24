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
using Alturos.Yolo.Model;
using AUGMANSmartCatapultGUI.Pointing.FrameArray;

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
        private Pointing.NN.Core pointer;
        private Pointing.FrameArray.Handler frameArrayHandler;

        public MainWindow()
        {
            this.ReceiverServer = new DataStreaming.Receiver.server();
            this.ReceiverServer.SetListener();

            /*Pointing system #BEG*/
            this.pointer = new Pointing.NN.Core();
            this.pointer.StartNet();
            /*Frame Array init*/
            frameArrayHandler = new Pointing.FrameArray.Handler();
            /*Pointing system #BEG*/

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
            /*Questa parte è da pulire creando un metodo nella classe di deserializzazione oppure nel puntamento*/
            packetHeader =  deserializer.getPacketHeader(e.UserState as byte[]);
            byte[] var = new byte[153600];
            int counter = 153600;
            int offset = sizeof(streamerPacketHeader) + 66 ;
            counter = (e.UserState as byte[]).Length < 153600 ? (e.UserState as byte[]).Length : 153600;
            if((counter + offset ) > (e.UserState as byte[]).Length)
                return;
            try
            {
                for (int i = 0; i < counter; i++)
                {
                    var[i] = (e.UserState as byte[])[i + offset];
                }
            }catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }
            ImageHolder.Source = null;

            Bitmap bmp  = deserializer.CreateBitmap(320, 240, var, System.Drawing.Imaging.PixelFormat.Format16bppRgb565, "C:\\temp\\stream.bmp");
            bmp.RotateFlip(RotateFlipType.RotateNoneFlipY);
            using (MemoryStream ms = new MemoryStream())
            {
                bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Png);
                YoloItem tmp = pointer.AnalyzeStream(ms);
                detectionObject obj;
                if (tmp != null)
                {
                    obj.nnItem = tmp;
                    obj.originHeader = packetHeader;
                    obj.originHeader.targetInFrame = true;
                    obj.originHeader.targetConfident = Convert.ToUInt16(tmp.Confidence*10);
                }
            }
            ImageHolder.Source = deserializer.ConvertBitmapToSource(bmp); 
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
