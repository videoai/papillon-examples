/**
 * @file doFaceLogClient.cc 
 * @brief Listen for face log events and display them.
 */
#include <PapillonCore.h>


using namespace papillon;

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


int main(int argc, char** argv) {

    PLog::OpenConsoleLogger("console", PLog::E_LEVEL_INFO);
    PapillonSDK::Initialise(argv[0]).OrDie();

    POption opt(argc, argv);
    opt.AddStandardOptions(); // set-up logging
    PString endPoint = opt.String("uri,u", "tcp://localhost:12345", "Publish events to this port");
    opt.Check().OrDie();

    if (opt.Has("h") || opt.Has("help")) {
        P_LOG_INFO << opt.ToStringHelp();
        return 0;
    }

    // Lets listen to events
    PSocket socket;
    PSocket::Open(socket, PSocket::E_SUBSCRIBER, PSocket::E_CONNECT, endPoint).OrDie();
    socket.Subscribe();

    // loop forever
    while(true) {

        if(socket.IsPendingIncomingMessage().Ok()) {
            PByteArray byteArray;
            socket.Receive(byteArray);

            PMemoryStream memoryStream(byteArray);
            PEvent event;
            memoryStream.ReadObject(event);
            P_LOG_INFO << event.ToString();
            if(event.GetEventType() == "Face") {
                // FIXME: as this is done per detection, we will get same frame more than once.  
                PDetection detection;
                event.GetPayload().Get("Detection", detection);
                PUtils::DisplayDetection(detection, "Detection");
                
            } else if(event.GetEventType() == "Track") {
                // OK lets get some information about the track
                PImage thumbnail;
                event.GetPayload().Get(papillon::C_OPTION_THUMBNAIL, thumbnail);
                thumbnail.Display("Track Thumbnail", 250);
                int64 startFrame;
                int32 numberOfFrames;
                event.GetPayload().Get("FrameNumber", startFrame);
                event.GetPayload().Get("NumberOfFrames", numberOfFrames);
                P_LOG_INFO << "Track Event: " << event.GetEventAnnotation() << " Start-frame: " << startFrame << " Number-frames: " << numberOfFrames;
            }
        }

    } // end forever

    socket.Close();
    return 0;
}
