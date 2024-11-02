import cv2
import os
import time


# Function to capture and save an image
def capture_and_save_image(folder_path):
    # Open the webcam
    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("Error: Could not open webcam.")
        return

    print("Press 's' to capture an image and save it, or 'q' to quit.")

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Failed to capture frame.")
            break

        # Display the live video frame
        cv2.imshow("Press 's' to capture or 'q' to quit", frame)

        # Check for key press
        key = cv2.waitKey(1) & 0xFF
        if key == ord('s'):  # Press 's' to save the image
            if folder_path:
                # Generate a unique filename based on the current time
                filename = f"capture_{int(time.time())}.png"
                full_path = os.path.join(folder_path, filename)

                # Save the image as a PNG file
                cv2.imwrite(full_path, frame)
                print(f"Image saved at {full_path}")
            else:
                print("No folder path provided.")

        elif key == ord('q'):  # Press 'q' to quit the program
            break

    # Release the capture and close OpenCV windows
    cap.release()
    cv2.destroyAllWindows()


# Specify the folder path to save images (change this to your desired path)
folder_path = "Calibration_Images/new_photos"

# Ensure the folder exists
print("Run script path: ")
print(os.path.abspath(__file__))
if not os.path.exists(folder_path):
    print(os.path)
    raise RuntimeError("Folder not found")


# Run the function to capture and save images
capture_and_save_image(folder_path)
