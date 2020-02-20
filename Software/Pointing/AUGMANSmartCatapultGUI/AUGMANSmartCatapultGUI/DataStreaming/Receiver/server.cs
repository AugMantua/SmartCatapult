using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Security;
using System.Runtime.InteropServices;

namespace AUGMANSmartCatapultGUI.DataStreaming.Receiver
{
    public class server
    {
        TcpListener serverSocket;
        TcpClient clientSocket;
        public string data;
        public string str_tmp;
        private string pass;
        private System.Security.SecureString pass_secure;
        byte[] bytes;
        private Socket listener;
        private IPHostEntry ipHostInfo;
        private IPAddress ipAddress;
        private IPEndPoint localEndPoint;
        private bool isListenerSet;
        private Socket handler;
        private bool isBoundToNode;


        public server()
        {
            serverSocket = new TcpListener(8888);
            clientSocket = default(TcpClient);
        }

        public int SetListener()
        {
            // Data buffer for incoming data.
            bytes = new Byte[1024];
            isListenerSet = false;

            // Establish the local endpoint for the socket.
            // Dns.GetHostName returns the name of the 
            // host running the application.
            ipHostInfo = Dns.Resolve(Dns.GetHostName());
            ipAddress = ipHostInfo.AddressList[2];
            localEndPoint = new IPEndPoint(ipAddress, 11000);

            // Create a TCP/IP socket.
            listener = new Socket(AddressFamily.InterNetwork,SocketType.Stream, ProtocolType.Tcp);

            // Bind the socket to the local endpoint and 
            // listen for incoming connections.
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(10);
            }catch(Exception e)
            {
                Console.WriteLine(e.Message);
                return -1;
            }
            isListenerSet = true;
            return 1;
        }

        public string StartListening()
        {
            if(!isListenerSet)
            {
                SetListener();
            }
            Console.WriteLine("Waiting for a connection on ip = {0}", ipAddress);
            // Program is suspended while waiting for an incoming connection.
            if (isBoundToNode)
            {
                if (!handler.Connected)
                {
                    this.handler = listener.Accept();
                    this.isBoundToNode = true;
                }
            }
            else
            {
                this.handler = listener.Accept();
                this.isBoundToNode = true;
            }

                
            data = null;

            // An incoming connection needs to be processed.
            while (true)
            {
                bytes = new byte[4096];
                int bytesRec = handler.Receive(bytes);
                data += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                if (data.IndexOf("<$>EOF<$>") > -1 || bytesRec.Equals(0))
                {
                    break;
                }
            }
            data = data.Replace("<$>EOF<$>", "");
            Console.WriteLine(data.Length);
            return data;
        }

        public void CloseConnection()
        {
            handler.Shutdown(SocketShutdown.Both);
            handler.Close();
        }
    }
}
