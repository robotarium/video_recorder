from pymba import *
import numpy as np
import cv2
import time

# start Vimba
with Vimba() as vimba:
    # get system object
    system = vimba.getSystem()

    # list available cameras (after enabling discovery for GigE cameras)
    if system.GeVTLIsPresent:
        system.runFeatureCommand("GeVDiscoveryAllOnce")
        time.sleep(0.2)
    cameraIds = vimba.getCameraIds()
    for cameraId in cameraIds:
        print(cameraId)

    # get and open a camera
    camera = vimba.getCamera(cameraIds[0])
    camera.openCamera()

    # list camera features
    cameraFeatureNames = camera.getFeatureNames()
    for name in cameraFeatureNames:
        print(name)

    print('---')

    # set the value of a feature
    camera.AcquisitionMode = 'Continuous'  # 'SingleFrame'
    camera.PixelFormat = 'BGR8Packed'
    time.sleep(0.2)

    # get the value of a feature
    print('Aquisition Mode: '+camera.AcquisitionMode)
    print('Pixel Format: '+camera.PixelFormat)

    try:
        #gigE camera
        print("Packet size:", camera.GevSCPSPacketSize)
	camera.BandwidthControlMode = 'StreamBytesPerSecond'
        camera.StreamBytesPerSecond = 115000000
        print("BPS:", camera.StreamBytesPerSecond)
    except:
        #not a gigE camera
        pass

    # create new frames for the camera
    frame = camera.getFrame()    # creates a frame

    # announce frame
    frame.announceFrame()

    # capture a camera image
    camera.startCapture()

    framecount = 0
    droppedframes = []
    displayedframes = []

    while True:
        print(framecount)

        try:
            frame.queueFrameCapture()
            success = True
            displayedframes.append(framecount)
        except:
            droppedframes.append(framecount)
            success = False

        camera.runFeatureCommand('AcquisitionStart')
        camera.runFeatureCommand('AcquisitionStop')

        frame.waitFrameCapture(100)

        # get image data...
        frame_data = frame.getBufferByteData()

        if success:
            img = np.ndarray(buffer = frame_data,
                            dtype = np.uint8,
                            shape = (frame.height,
                                    frame.width,
                                    frame.pixel_bytes)
                            )
            cv2.imshow("image", img);

        framecount += 1
        k = cv2.waitKey(1)
        if k == 0x1b:
            cv2.destroyAllWindows()
            print("Frames displayed: %s" % displayedframes)
            print("Frames dropped: %s" % droppedframes)
            break

    # clean up after capture
    camera.endCapture()
    camera.revokeAllFrames()

    # close camera
    camera.closeCamera()
