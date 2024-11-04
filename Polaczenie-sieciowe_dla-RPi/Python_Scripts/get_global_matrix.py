import cv2 as cv
import numpy as np
import pandas as pd

p_x = 7
p_y = 9
camera_name = "Hp"
square_size = 2.0 # 2 cm


internal_cameras_matrix_folder = "Internal_camera_matrices"
external_cameras_matrix_folder = "External_camera_matrices"
distortion_folder = "Distortion_vectors"

# Function to capture and save an image
def capture_image():
    # Open the webcam
    cap = cv.VideoCapture(0)

    if not cap.isOpened():
        print("Error: Could not open webcam.")
        return

    print("Press 's' to capture an image and save it, or 'q' to quit.")

    image_nr: int = 1

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Failed to capture frame.")
            break

        # Display the live video frame
        cv.imshow("Press 's' to capture or 'q' to quit", frame)

        # Check for key press
        key = cv.waitKey(1) & 0xFF
        if key == ord('s'):  # Press 's' to save the image
            cap.release()
            cv.destroyAllWindows()

            return  frame

        elif key == ord('q'):  # Press 'q' to quit the program
            cap.release()
            cv.destroyAllWindows()

            return None

def main():
    img = capture_image()

    if img is None:
        return 0
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    # Find the chess board corners
    found_chess, corners = cv.findChessboardCorners(gray, (p_x, p_y), None)

    objp = np.zeros((p_x * p_y, 3), np.float32)
    objp[:, :2] = np.mgrid[0:p_x, 0:p_y].T.reshape(-1, 2)
    objp *= square_size

    corners_refined = cv.cornerSubPix(gray, corners, (11, 11), (-1, -1),
                                       criteria=(cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001))

    internal_matrix = np.load(internal_cameras_matrix_folder + "/" + camera_name + ".npy")
    distraction_vector = np.load(distortion_folder + "/" + camera_name + ".npy")

    ret, rvec, tvec = cv.solvePnP(objp, corners_refined, internal_matrix, distraction_vector)

    if ret:
        # Convert rotation vector to rotation matrix
        rotation_matrix, _ = cv.Rodrigues(rvec)

        # Create the 4x4 global pose matrix
        external_matrix = np.eye(4)
        external_matrix[:3, :3] = rotation_matrix
        external_matrix[:3, 3] = tvec.T

        print("Global Pose Matrix:")
        print(external_matrix)

        np.save(external_cameras_matrix_folder + "/{}.npy".format(camera_name), external_matrix)

        # TODO fix xlsx generation
        # pd.DataFrame(external_matrix).to_excel(external_cameras_matrix_folder + "/{}.xlsx".format(external_matrix), index=False, header=False)
        np.savetxt(external_cameras_matrix_folder + "/{}.csv".format(camera_name), external_matrix, delimiter=',', fmt='%f')

if __name__ == '__main__':
    main()
