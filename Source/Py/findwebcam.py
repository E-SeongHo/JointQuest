import cv2

def find_available_webcam():
    index = 0
    available_cameras = []
    while True:
        cap = cv2.VideoCapture(index)
        if not cap.isOpened():
            break
        ret, frame = cap.read()
        if ret:
            available_cameras.append(index)
        cap.release()
        index += 1
    return available_cameras

available_cameras = find_available_webcam()
if available_cameras:
    print(f"Available webcam indices: {available_cameras}")
else:
    print("No available webcams found.")