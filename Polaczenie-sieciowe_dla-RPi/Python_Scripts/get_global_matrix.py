import cv2 as cv
import numpy as np

def draw(img, corners, imgpts):
    corner = tuple(corners[0].ravel())
    img = cv.line(img, corner, tuple(imgpts[0].ravel()), (255,0,0), 5)
    img = cv.line(img, corner, tuple(imgpts[1].ravel()), (0,255,0), 5)
    img = cv.line(img, corner, tuple(imgpts[2].ravel()), (0,0,255), 5)
    return img

p_x = 7
p_y = 9
camera_name = "Dell"
square_size = 40 # mm

display_points = True
display_time = 1500
display_time_2 = 2000

result_by_device_folder = "Result_by_device"
by_device_matrix_name = "Camera_external_parameters"

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

    if display_points:
        cv.drawChessboardCorners(gray, (p_y, p_x), corners_refined, found_chess)
        cv.imshow('img', gray)
        cv.waitKey(display_time)

    internal_matrix = np.load(internal_cameras_matrix_folder + "/" + camera_name + ".npy")
    distraction_vector = np.load(distortion_folder + "/" + camera_name + ".npy")

    ret, rvec, tvec = cv.solvePnP(objp, corners_refined, internal_matrix, distraction_vector, flags=cv.SOLVEPNP_ITERATIVE)

    if ret:
        print("Macierz rotacji:\n", rvec)
        rotation_matrix, _ = cv.Rodrigues(rvec)
        angles = cv.decomposeProjectionMatrix(np.hstack((rotation_matrix, np.zeros((3, 1)))))[-1]
        print("Kąty Eulera (XYZ):\n", angles.flatten() )
        # print("Kąt w stopniach:\n", rvec * (180 / np.pi))
        print("Macierz transformacji:\n", tvec)

        # Convert rotation vector to rotation matrix
        rotation_matrix, _ = cv.Rodrigues(rvec)

        # Create the 4x4 global pose matrix
        external_matrix = np.eye(4)
        external_matrix[:3, :3] = rotation_matrix
        external_matrix[:3, 3] = tvec.T

        print("Global Pose Matrix:")
        print(external_matrix)

        np.save(external_cameras_matrix_folder + "/{}.npy".format(camera_name), external_matrix)

        np.savetxt(result_by_device_folder + "/" + camera_name + "/{}.csv".format(by_device_matrix_name),
                   external_matrix[:3, :], delimiter=',', fmt='%f')

        # TODO fix xlsx generation
        # pd.DataFrame(external_matrix).to_excel(external_cameras_matrix_folder + "/{}.xlsx".format(external_matrix), index=False, header=False)
        np.savetxt(external_cameras_matrix_folder + "/{}.csv".format(camera_name), external_matrix, delimiter=',', fmt='%f')

        axis = np.float32([[0, 0, 0], [50, 0, 0], [0, 50, 0], [0, 0, -50]])
        imgpts, jac = cv.projectPoints(axis, rvec, tvec, internal_matrix, distraction_vector)
        img = cv.drawContours(img, [imgpts[:4].astype(int)], -1, (0, 255, 0), 3)
        cv.imshow('Axes', img)
        cv.imwrite("axes.png", img)
        cv.waitKey(display_time_2)



if __name__ == '__main__':
    main()
