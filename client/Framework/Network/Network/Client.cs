using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;

namespace Network
{
    class Client
    {
        private Socket mSocket = null;
        private ManualResetEvent mConnTimeoutEvent = new ManualResetEvent(false);
        private SpRpc mRpc = null;
        private int mSession = 0;

        private const int MAX_CONNECT_TIME = 30000;
        private const int MAX_BUFFER_SIZE = 65535;

        private byte[] mReceiveBuffer = new byte[MAX_BUFFER_SIZE];
        private byte[] mSendBuffer = new byte[MAX_BUFFER_SIZE];

        private Queue<SpRpcResult> mReceiveQueue = new Queue<SpRpcResult>();

        public bool Start(string serverIP, short port, string packageName, string s2c, string c2s)
        {
            bool result = true;

            try
            {
                mSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                IPAddress ipAddress = IPAddress.Parse(serverIP);
                IPEndPoint ipEndPoint = new IPEndPoint(ipAddress, port);
                mSocket.ReceiveBufferSize = MAX_BUFFER_SIZE;
                mSocket.NoDelay = true;

                mSocket.BeginConnect(ipEndPoint, new AsyncCallback(ConnectCallback), mSocket);
                if (mConnTimeoutEvent.WaitOne(MAX_CONNECT_TIME, false))
                {
                    if (mSocket.Connected)
                    {
                        mRpc = SpRpc.Create(s2c, packageName);
                        mRpc.Attach(c2s);

                        mSocket.BeginReceive(mReceiveBuffer, 0, mReceiveBuffer.Length, SocketFlags.None, new AsyncCallback(ReceiveCallback), this);
                    }
                }
                else
                {
                    // Block time out and do not recevie any signal
                    DebugHelper.Log("Block time out and do not recevie any signal");
                }
            }
            catch(Exception ex)
            {
                DebugHelper.Log(ex.ToString());

                result = false;
            }

            return result;
        }

        public void SendRequest(string protocolName, SpObject spObject)
        {
            mSession++;
            SpStream spStream = new SpStream();
            spStream.Write((short)0);

            mRpc.Request(protocolName, spObject, mSession, spStream);
            int len = spStream.Length - 2;
            spStream.Buffer[0] = (byte)((len >> 8) & 0xff);
            spStream.Buffer[1] = (byte)(len & 0xff);

            mSocket.BeginSend(mSendBuffer, 0, mSendBuffer.Length, SocketFlags.None, new AsyncCallback(SendRequestCallback), this);
        }

        public Queue<SpRpcResult> GetAndClearReceiveQueue()
        {
            Queue<SpRpcResult> resultList;

            if (mReceiveQueue != null)
            {
                mReceiveQueue.Clear();
                resultList = mReceiveQueue;
            }
            else
            {
                resultList = new Queue<SpRpcResult>();
            }

            return resultList;
        }

        // ------------------------------------------------------------
        // Inner call
        // ------------------------------------------------------------

        private void ConnectCallback(IAsyncResult asyncResult)
        {
            try
            {
                TcpClient tcpClient = asyncResult.AsyncState as TcpClient;
                if (tcpClient != null && tcpClient.Client != null)
                {
                    tcpClient.EndConnect(asyncResult);
                }

            }
            catch(Exception ex)
            {
                DebugHelper.Log(ex.ToString());
            }
            finally
            {
                // stop connect block
                mConnTimeoutEvent.Set();
            }
        }

        private static void SendRequestCallback(IAsyncResult asyncResult)
        {
            Client client = asyncResult.AsyncState as Client;
            client.Send(asyncResult);
        }

        private void Send(IAsyncResult asyncResult)
        {

        }

        private static void ReceiveCallback(IAsyncResult asyncResult)
        {
            Client client = asyncResult.AsyncState as Client;
            client.Receive(asyncResult);
        }

        private void Receive(IAsyncResult asyncResult)
        {
            int receiveLength = mSocket.EndReceive(asyncResult);
            int read = 0;

            while (receiveLength > read)
            {
                if (read >= mReceiveBuffer.Length - 1)
                {
                    break;
                }

                int size = (mReceiveBuffer[read] | mReceiveBuffer[read + 1] << 8);
                read += 2;

                if (receiveLength >= read + size)
                {
                    SpStream spStream = new SpStream(mReceiveBuffer, read, size, size);
                    SpRpcResult spResult = mRpc.Dispatch(spStream);

                    mReceiveQueue.Enqueue(spResult);
                }

                read += size;
            }

            mSocket.BeginReceive(mReceiveBuffer, 0, mReceiveBuffer.Length, SocketFlags.None, new AsyncCallback(ReceiveCallback), this);
        }
    }
}
