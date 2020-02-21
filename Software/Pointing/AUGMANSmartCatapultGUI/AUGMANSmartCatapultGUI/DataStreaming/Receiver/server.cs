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
        private List<byte> Internalbuffer;



        public server()
        {
            serverSocket = new TcpListener(8888);
            clientSocket = default(TcpClient);
            Internalbuffer = new List<byte>();
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
            ipAddress = ipHostInfo.AddressList[3];
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

        public List<byte> StartListening()
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
            Internalbuffer.Clear();

            // An incoming connection needs to be processed.
            while (true)
            {
                bytes = new byte[8192];
                int bytesRec = handler.Receive(bytes);
                for(int i = 0; i < bytesRec; i++)
                {
                    Internalbuffer.Add(bytes[i]);
                }
                //Vale la pena di fare una funzione per cercare EOF non testuale
                if (Encoding.ASCII.GetString(bytes, 0, bytesRec).IndexOf("<EOF>") > -1 || bytesRec.Equals(0))
                {
                    break;
                }
            }
            //data = data.Replace("<EOF>", "");
            return Internalbuffer;
        }
       
        public void CloseConnection()
        {
            handler.Shutdown(SocketShutdown.Both);
            handler.Close();
        }
    }
}
