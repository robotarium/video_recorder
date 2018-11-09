from pymba import *
import numpy as np
import cv2
import time

with Vimba() as vimba:
    # get system object
    try:
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

        # Set up the camera with the right parameters
        # Includes examples of setting up the output TTLs
        #camera.PixelFormat = 'Mono8'
        camera.AcquisitionMode = 'Continuous'
        #camera.BinningHorizontal = 4
        #camera.BinningVertical = 4
        camera.Height = camera.HeightMax
        camera.Width = camera.WidthMax
        #camera.AcquisitionFrameRateAbs = 60
        camera.SyncOutSelector = 'SyncOut1'
        camera.SyncOutSource = 'Exposing'
        camera.TriggerSource = 'FixedRate'

        try:
            #gigE camera
            print("Packet size:", camera.GevSCPSPacketSize)
            camera.GevSCPSPacketSize = 1500
            #camera.PayloadSize = 5760000
            camera.BandwidthControlMode = 'StreamBytesPerSecond'
            camera.StreamBytesPerSecond = 100000000 #115000000
            print("BPS:", camera.StreamBytesPerSecond)
        except Exception as e:
            print(repr(e))
            #not a gigE camera
            pass

        # create new frames for the camera
        frame = camera.getFrame()
        # announce frame
        frame.announceFrame()

        camera.startCapture()

        framecount = 0
        droppedframes = []
        displayedframes = []

        fourcc = cv2.VideoWriter_fourcc(*'MJPG')
        out = cv2.VideoWriter('output.mp4',fourcc, 20.0, (640,480))

        camera.runFeatureCommand('AcquisitionStart')

        while True:
            print(framecount)

            # capture a camera image

            try:
                frame.queueFrameCapture()
                success = True
                displayedframes.append(framecount)
            except:
                droppedframes.append(framecount)
                success = False

            frame.waitFrameCapture(10000)

            # get image data...
            frame_data = frame.getBufferByteData()

            if success:
                img = np.ndarray(buffer=frame_data,
                                 dtype=np.uint8,
                                 shape=(frame.height, frame.width, frame.pixel_bytes))
                cv2.imshow("test", img)
                #out.write(img)

            framecount += 1
            k = cv2.waitKey(1)
            if k == 0x1b:
                cv2.destroyAllWindows()
                print("Frames displayed: %s" % displayedframes)
                print("Frames dropped: %s" % droppedframes)
                break

        #out.release()
        camera.runFeatureCommand('AcquisitionStop')

        # clean up after capture
        camera.flushCaptureQueue()
        camera.endCapture()
        camera.revokeAllFrames()
        print('Got here')
        # close camera
        camera.closeCamera()
    except Exception as e:
        print(repr(e))
        print(e)
