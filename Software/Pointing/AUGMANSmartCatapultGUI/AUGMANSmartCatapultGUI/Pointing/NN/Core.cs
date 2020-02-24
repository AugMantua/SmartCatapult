using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Alturos.Yolo;
using Alturos.Yolo.Model;

namespace AUGMANSmartCatapultGUI.Pointing.NN
{
    public class Core
    {
        private YoloWrapper yoloWrapper;
        private bool readyToStream;
        private const string net = "chrome";
        private string[] objects;
        private const double _MIN_CONFIDENCE = 0.10;

        public bool StartNet()
        {
            if (File.Exists($"trainfiles/{net}.cfg") && File.Exists($"trainfiles/{net}.weights") && File.Exists($"trainfiles/{net}.names"))
            {
                yoloWrapper = new YoloWrapper($"trainfiles/{net}.cfg", $"trainfiles/{net}.weights", $"trainfiles/{net}.names");
                if (yoloWrapper.EnvironmentReport.CudaExists == false)
                {
                    giveErrorMessage("Install CUDA 10");
                }
                if (yoloWrapper.EnvironmentReport.CudnnExists == false)
                {
                    giveErrorMessage("Cudnn doesn't exist");
                }
                if (yoloWrapper.EnvironmentReport.MicrosoftVisualCPlusPlusRedistributableExists == false)
                {
                    giveErrorMessage("Install Microsoft Visual C++ 2017 Redistributable");
                }
                /*if (yoloWrapper.DetectionSystem.ToString() != "GPU")
                {
                    giveErrorMessage("No GPU card detected. Exiting...");
                }*/
                objects = File.ReadAllLines($"trainfiles/{net}.names");
                return true;
            }
            return false;
        }

        public YoloItem AnalyzeStream(MemoryStream ms)
        {
            IEnumerable<Alturos.Yolo.Model.YoloItem> items = yoloWrapper.Detect(ms.ToArray());
            if (items.Count() > 0)
            {
                foreach (var item in items)
                {
                    if(item.Confidence > _MIN_CONFIDENCE && objects.Contains<string>(item.Type))
                    {
                        return item;
                    }
                }
            }
            return null;
        }

        private void giveErrorMessage(string s)
        {
            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }
    }
}
