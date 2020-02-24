using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Alturos.Yolo.Model;
using AUGMANSmartCatapultGUI.DataStreaming.DataHandler;

namespace AUGMANSmartCatapultGUI.Pointing.FrameArray
{
    public struct detectionObject
    {
        public streamerPacketHeader originHeader;
        public YoloItem nnItem;
    };
    public class Handler
    {
        private List<detectionObject> detectionList;

        public Handler()
        {
            this.detectionList = new List<detectionObject>();
        }

        public int AddElement(detectionObject obj)
        {
            try
            {
                this.detectionList.Add(obj);
                //First in first out
                if(detectionList.Count > 10)
                {
                    detectionList.RemoveAt(0);
                }
            }catch(Exception e)
            {
                return -1;
            }
            return 1;
        }

        public detectionObject getElement()
        {
            if(detectionList.Count > 0)
            {
                detectionObject tmp = detectionList[detectionList.Count - 1];
                detectionList.RemoveAt(detectionList.Count - 1);
                return tmp;
            }
            else
            {
                throw new AccessViolationException();
            }
        }
    }

}
