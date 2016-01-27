/**
 * @author Kieron Messer
 * @file doMetaData.cpp
 * @brief Example on how to get at the meta data produced by detector
 */
#include <PapillonCore.h>


using namespace papillon;

const PString SAMPLE_DIR = PPath::Join(PUtils::GetEnv("PAPILLON_INSTALL_DIR"), "Data", "Samples");

int main(int argc, char** argv) {

    PLog::OpenConsoleLogger("console", PLog::E_LEVEL_INFO);
    PapillonSDK::Initialise(argv[0]).OrDie();
    
    POption opt(argc, argv);
    PString inputVideo = opt.String("inputVideoStream, i", PPath::Join(SAMPLE_DIR, "face_log.mov"), "Input video stream");
    opt.AddStandardOptions(); // set-up logging
    opt.Check().OrDie();

    // Open video stream
    PInputVideoStream inputVideoStream;
    PInputVideoStream::Open(inputVideo, inputVideoStream);
    
    // Create a detector, or die as we can not do anything without it
    PDetector detector;
    PDetector::Create("FaceDetector", "", detector).OrDie();
    PDetectorOptions detectorOptions;
    
    PFrame frame;
    while(inputVideoStream.GetFrame(frame)) {
    
        P_LOG_INFO << "Detecting frame " << frame.GetFrameNumber() << " " << frame.GetTimestampUTC();
        
        // now lets do the detection
        PDetectionList detectionList;
        detector.Detect(frame, detectorOptions, detectionList);
        
        // there are often multiple detections per frame
        int32 numberOfDetectionsInFrame = detectionList.Size();
        P_LOG_INFO << "There were " << numberOfDetectionsInFrame << " detections";
        
        // now lets go through each detection and see what happended
        PImage image = frame.GetImage(); // get image for displaying features
        for(int32 i=0; i < numberOfDetectionsInFrame; i++) {
        
            P_LOG_INFO << "Investigating Detection " << i;
            
            // Get the detection
            PDetection detection = detectionList.Get(i);
            
            // Each detection has got a confidence that the detection is valid
            P_LOG_INFO << " - confidence: " << detection.GetConfidence();
            
            // Each detection has been assigned a unique-id
            // This can be useful to store them in hash tables, databases e.t.c.
            P_LOG_INFO << " - id: " << detection.GetObjectId();
            
            // Each detection uTsually relates to somewhere on the image
            // This information is stored in a FeatureMap.
            PFeatureMap featureMap = detection.GetFeatureMap();
                
            // Depending on the detector you will get a different features stored within it
            // They will either be points or rectangles.
            PStringList featurePointNames = featureMap.GetFeaturePointNames();
            P_LOG_INFO << " - featurePoints: " << featurePointNames.ToString();
            
            PStringList featureRectangleNames = featureMap.GetFeatureRectangleNames();
            P_LOG_INFO << " - featureRectangleNames: " << featureRectangleNames.ToString();
            
            // If you want to get to specific features 
            for(int32 j=0; j < featureRectangleNames.Size(); j++) {
            
                PString featureRectangleName = featureRectangleNames.Get(j);
                P_LOG_INFO << " -- feature: " << featureRectangleName;

                // Get feature
                PFeatureRectangle featureRectangle = featureMap.GetFeatureRectangleByName(featureRectangleName);
                
                // Each feature has an associated confidence
                P_LOG_INFO << " -- confidence: " << featureRectangle.GetConfidence();
                P_LOG_INFO << " -- rectangle: " << featureRectangle.GetRectangle().ToString();
            }
            
            // Each feature map will always have a bounding box, no matter what
            // This is computed to make sure all features are always inside it
            PRectanglef boundingBox = featureMap.GetBoundingBox();
            P_LOG_INFO << " - boundingBox: " << boundingBox;
            
            // Often it us useful to display the features on the image to display it
            // The image class has lots of functions for overlaying the features
            image.DrawRectangle(boundingBox, PColour3i::C_RED);
            
            
        }
        
        image.Display("My Detections"); 
        
        
        
        
    }
    
    
    
    
    return 0;
}
