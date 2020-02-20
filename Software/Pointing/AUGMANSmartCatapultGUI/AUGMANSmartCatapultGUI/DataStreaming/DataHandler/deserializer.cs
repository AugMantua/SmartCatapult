using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AUGMANSmartCatapultGUI.DataStreaming.DataHandler
{
    /**
    * 
    * header structure definition
    * this header must contain timing information of the bitmap
    * master will reply to the stream with position of the target if found
    * and time of the original frame in order to understand in case speed/acceleration ecc
    * If there is a target into the frame master will set targetInFrame and give % of confident
    */
    struct streamerPacketHeader
    {
        /**/
        UInt32 counter;
        UInt32 frameTick;
        /**/
        bool targetInFrame;
        UInt32 targetConfident;
        /**/
        UInt32 sizeofBuffer;
        UInt32 sizeofHeader;
    };


    public class deserializer
    {

    }
}
