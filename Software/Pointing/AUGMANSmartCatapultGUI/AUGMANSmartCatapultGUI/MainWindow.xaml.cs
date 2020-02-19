﻿using System;
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

namespace AUGMANSmartCatapultGUI
{
    /// <summary>
    /// Logica di interazione per MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly BackgroundWorker receiverServerWorker;
        private DataStreaming.Receiver.server ReceiverServer;

        public MainWindow()
        {
            this.ReceiverServer = new DataStreaming.Receiver.server();
            this.ReceiverServer.SetListener();
            receiverServerWorker = new BackgroundWorker();
            receiverServerWorker.DoWork += ReceiverServerWorker_DoWork;
            receiverServerWorker.RunWorkerCompleted += ReceiverServerWorker_RunWorkerCompleted;
            receiverServerWorker.ProgressChanged += ReceiverServerWorker_ProgressChanged;
            receiverServerWorker.WorkerReportsProgress = true;

            InitializeComponent();

            receiverServerWorker.RunWorkerAsync();
        }

        private void ReceiverServerWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.ServerText.Text = e.UserState.ToString() + "\n";
        }

        private void ReceiverServerWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            throw new NotImplementedException();
        }

        private void ReceiverServerWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while (!receiverServerWorker.CancellationPending)
            {
                string tmp = this.ReceiverServer.StartListening();
                if ( tmp.Length > 0)
                {
                    receiverServerWorker.ReportProgress(0, tmp);
                }
            }
        }
    }
}
