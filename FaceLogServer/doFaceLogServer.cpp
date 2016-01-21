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

PResult ProcessEvents(PSocket socket, const PList<PEvent> & events) {

    // Got through each event
    for (int32 i = 0, ni = events.Size(); i < ni; i++) {
        PEvent event = events.Get(i);

        // Send event
        PMemoryStream memoryStream;
        memoryStream.WriteObject(event);
        if(socket.Send(memoryStream.GetByteArray()).Failed()) {
            P_LOG_WARNING << "Failed to send event";
        }
        P_LOG_INFO << "Event published...";

    }
    return PResult::C_OK;
}


int main(int argc, char** argv) {

    PLog::OpenConsoleLogger("console", PLog::E_LEVEL_INFO);
    PapillonSDK::Initialise(argv[0]).OrDie();

    POption opt(argc, argv);
    opt.AddStandardOptions(); // set-up logging
    PString inputVideoFile = opt.String("inputVideo,i", PPath::Join(SAMPLE_DIR, "mc_guests01.mkv"), "Input video file");
    PString endPoint = opt.String("uri,iu", "tcp://*:12345", "Publish events to this port");
    PString outputVideoFile = opt.String("outputVideo,o", "out.avi", "Output video file");
    int32 maxFrames = opt.Int("frames,f", 0, "Maximum number of frames to process");
    int32 skip = opt.Int("skip,ss", 0, "Skip this number of frames at the beginning");
    double scale = opt.Double("scale,s", 1.0, "Scale the output image");
    bool profile = opt.Bool("profile,p", false, "Use profile face detector");
    bool blur = opt.Bool("blur,b", false, "Blur the faces");
    opt.Check().OrDie();

    if (opt.Has("h") || opt.Has("help")) {
        P_LOG_INFO << opt.ToStringHelp();
        return 0;
    }

    PInputVideoStream ivs;
    PInputVideoStream::Open(inputVideoFile, ivs).OrDie();

    // Lets publish events somewhere
    PSocket socket;
    PSocket::Open(socket, PSocket::E_PUBLISHER, PSocket::E_BIND, endPoint).OrDie();
    socket.GetBoundPoint(endPoint).OrDie();

    // Create an output stream for H.264 encoding
    POutputVideoStream outputStream;
    if (POutputVideoStream::Open(PString("file:%0?encode_with=opencv&fps=5").Arg(outputVideoFile),  outputStream).Failed())
        return PResult::Error("Cannot open video output stream");

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
    P_LOG_INFO << "max frames: "  << maxFrames;
    while (ivs.GetFrame(frame).Ok()) {
        frameNumber++;
        if(frameNumber < skip)
            continue;

        faceLog.Apply(frame, events);
        ProcessEvents(socket, events);
        P_LOG_DEBUG << "(" << frameNumber << "/" << maxFrames << ") found " << events << " events.";

        PImage displayImage = frame.GetImage().DeepCopy();
        faceLog.DrawOSD(displayImage);

        if(scale < 1.0) {
            displayImage.Resize(displayImage, 0.5f, papillon::PImage::E_IGNORE_ASPECT_RATIO, PImage::E_INTERPOLATION_NEAREST);
        }
        //displayImage.Display("Face Log");

        if(frameNumber == maxFrames) {
            P_LOG_INFO << "Maximum number of frames reached.";
            break;
        }

        outputStream.PutImage(displayImage);


    } // end while


    // OK we have finished video, lets check we have no events hanging around
    faceLog.Finish(events);
    ProcessEvents(socket, events);

    outputStream.Release();

    socket.Close();

    P_LOG_INFO << "Server finished, please press key";


    //PapillonSDK::Finalise();
    return 0;
}
