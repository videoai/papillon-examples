/*
 * Copyright (C) 2015 Digital Barriers plc. All rights reserved.
 * Contact: http://www.digitalbarriers.com/
 *
 * This file is part of the Papillon SDK.
 *
 * You can't use, modify or distribute any part of this file without
 * the explicit written agreements of Digital Barriers plc.
 */

/**
 * @file ExampleFaceDetector.cpp
 * @brief Example program that runs face-detector on a webcam.
 */
#include <PapillonCore.h>


using namespace papillon;

const bool ENABLE_FACE_LOCALISER = false;
const float SCALE_FACTOR = 0.75f;
const PString SAMPLE_DIR = PPath::Join(PUtils::GetEnv("PAPILLON_INSTALL_DIR"), "Data", "Samples");

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

PResult ProcessEvents(const PList<PEvent> & events, PSocket socket) {

    // Got through each event
    for (int32 i = 0, ni = events.Size(); i < ni; i++) {
        PEvent event = events.Get(i);

        // Send event, what ever it is, on the ether....
        PMemoryStream memoryStream;
        memoryStream.WriteObject(event);
        if(socket.Send(memoryStream.GetByteArray()).Failed()) {
            P_LOG_WARNING << "Failed to send event";
        }

    }
    return PResult::C_OK;
}


int main(int argc, char** argv) {

    PLog::OpenConsoleLogger("console", PLog::E_LEVEL_INFO);
    PapillonSDK::Initialise(argv[0]).OrDie();

    POption opt(argc, argv);
    opt.AddStandardOptions(); // set-up logging
    PString inputVideoFile = opt.String("inputVideo,i", PPath::Join(SAMPLE_DIR, "face_log.mov"), "Input video file");
    PString endPoint = opt.String("uri,iu", "tcp://*:12345", "Publish events to this port");
    PString outputVideoFile = opt.String("outputVideo,o", "out.avi", "Output video file");
    int32 maxFrames = opt.Int("frames,f", 0, "Maximum number of frames to process");
    int32 skip = opt.Int("skip,ss", 0, "Skip this number of frames at the beginning");
    bool profile = opt.Bool("profile,p", false, "Use profile face detector");
    bool blur = opt.Bool("blur,b", false, "Blur the faces");
    double scale = opt.Double("scale,s", 1.0, "If displaying, then scale the output image");
    opt.Check().OrDie();
 
    // Have we asked fr help?
    if (opt.Has("h") || opt.Has("help")) {
        P_LOG_INFO << opt.ToStringHelp();
        return 0;
    }

    // We always need an input video stream
    PInputVideoStream ivs;
    PInputVideoStream::Open(inputVideoFile, ivs).OrDie();

    // We want to send the events somehere, so lets open a publisher socket 
    PSocket socket;
    PSocket::Open(socket, PSocket::E_PUBLISHER, PSocket::E_BIND, endPoint).OrDie();
    socket.GetBoundPoint(endPoint).OrDie(); // not sure why we need this...


    // We are running face-log, so lets set it up
    PProperties faceLogOptions;
    faceLogOptions.Set("MinDetectionSize", 30);
    faceLogOptions.Set("MaxDetectionSize", 1000);
    faceLogOptions.Set("Profile", profile);
    faceLogOptions.Set("Blur", blur);
    PAnalytics faceLog;
    PAnalytics::Create("FaceLog", faceLogOptions, faceLog).OrDie();

    PList<PEvent> events;
    PFrame frame;
    int frameNumber = 0;
    while (ivs.GetFrame(frame).Ok()) {
   
        // we might of been asked to only analyse a set number of frames
        frameNumber++;
        if(frameNumber < skip)
            continue;
 
        faceLog.Apply(frame, events);
        ProcessEvents(events, socket);
        P_LOG_DEBUG << "(" << frameNumber << "/" << maxFrames << ") found " << events << " events.";

        PImage displayImage = frame.GetImage().DeepCopy();
        faceLog.DrawOSD(displayImage);

        if(frameNumber == maxFrames) {
            P_LOG_INFO << "Maximum number of frames reached.";
            break;
        }

    } // end while


    // OK we have finished video, lets check we have no events hanging around
    // If we do, we should handle them
    faceLog.Finish(events);
    ProcessEvents(events, socket);

    // Lets make sure we close things properly
    socket.Close();
    P_LOG_INFO << "Server finished, please press key";

    return 0;
}
