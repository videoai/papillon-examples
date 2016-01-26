# FaceLog Server and Client

This is a very simple example of how to use the FaceLog analytic to process a video stream
and publish events.  These events then can be read (subsscribed) by any client (with relevant permissions)
and processed further.  The example code consists of two main processes.

 - A FaceLog server analyses a video stream.  Every event it finds get published on a port.

 - A FaceLog client listens on this port.  Every event it recieves it displays.


# Running

First start the client, which listens for events.

```
doFaceLogClient
```

Then start the server which loads processes the video and publishes the events.
```
doFaceLogServer
```

Both programs have a few options which can be detailed by specifying --help on the command-line.

