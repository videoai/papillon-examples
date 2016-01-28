# Papillon Examples


In this repository are some examples which demonstrate the power and ease which Papillon
can be used to help build Video Analytics into your application.

Each folder attempts to highlight a single feature of the Papillon SDK.  These examples
are complimentary to the examples provided with your installation of Papillon.

To compile these examples you need an installed and working version of Papillon first. 
If you are interested in getting copy of the Papillon SDK please drop an email to 
kieron.messer@digitalbarriers.com.

Please note, these examples highlight the functionality of Papillon and demonstrate how you 
can incorporate Papillon into your video analytics applications.  They are not intended to be 
production quality.

| Example | Description |
| --- | --- |
| [MetaData](MetaData) | Learn how to extract meta-data from detectors |     
| [MemoryStream](MemoryStream) | Learn how to use memory streams, useful for storing blobs in databases |
| [FaceLogServer](FaceLogServer) | Easily transmit events and video across a network |

## Compiling

The easiest way to generate a makefile for your platform is to use CMake.  You will need to have your ```${PAPILLON_INSTALL_DIR}``` environment variable set.

```
cd papillon-examples
mkdir build
cd build
cmake ..
make -j4
```


