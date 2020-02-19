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
        UInt32 counter;
        UInt32 frameTick;
        bool targetInFrame;
        UInt32 targetConfident;
        UInt32 frameSize;
        //
        UInt32 sizeofBuffer;
        UInt32 sizeofHeader;
    };

    /**
    * Packet data
    * contains the pointer to the buffer and the size of the frame + rgb565 header
    */
    unsafe struct streamerPacketData
    {
        void* bitmapHeader;
        void* bufferPointer;
        UInt32 sizeofBuffer;
        UInt32 sizeofHeader;
    };

    /**
    * 
    * Packet 
    * contains header, data and total size
    * where total size is header size + data size(where data size is bitmap header + frame data)
    * 
    */
    struct streamerPacket
    {
        streamerPacketHeader header;
        streamerPacketData   data;
        UInt32 sizeofPacket;
    };

    public class deserializer
    {

    }
}
