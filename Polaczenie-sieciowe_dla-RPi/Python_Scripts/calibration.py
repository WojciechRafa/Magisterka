import numpy as np
import cv2 as cv
import glob
import pandas as pd

# termination criteria
criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

p_x = 7
p_y = 9

square_size = 20.0 # mm

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(p_x,5,0)
objp = np.zeros((p_x*p_y,3), np.float32)
objp[:, :2] = np.mgrid[0:p_y, 0:p_x].T.reshape(-1,2)

objp *= square_size

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

Chosen_camera = "Dell"
# Chosen_camera = "Hp"
# Chosen_camera = "IPhone"
display_points: bool = False

images = glob.glob('Calibration_Images/{}/*.png'.format(Chosen_camera), recursive=True)
cameras_matrix_folder = "Internal_camera_matrices"
distortion_folder = "Distortion_vectors"

idx_list = []

h:int = -1
w:int = -1

for idx, fname in enumerate(images):
    img = cv.imread(fname)

    if (h, w) != img.shape[:2] and (h != -1 or w != -1):
        raise RuntimeError("Image have different sizes")
    h, w = img.shape[:2]

    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    # Find the chess board corners
    found_chess, corners = cv.findChessboardCorners(gray, (p_x, p_y), None)

    # If found, add object points, image points (after refining them)
    if found_chess:
        objpoints.append(objp)

        corners2 = cv.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
        imgpoints.append(corners2)

        if display_points:
            cv.drawChessboardCorners(gray, (p_y, p_x), corners2, found_chess)
            cv.imshow('img', gray)
            cv.waitKey(100)
    cv.destroyAllWindows()

ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(objpoints, imgpoints, (w, h), None, None)

newcameramtx, roi = cv.getOptimalNewCameraMatrix(mtx, dist, (w, h), 1, (w, h))

cv.destroyAllWindows()

np.save(cameras_matrix_folder + "/{}.npy".format(Chosen_camera), newcameramtx)
pd.DataFrame(newcameramtx).to_excel(cameras_matrix_folder + "/{}.xlsx".format(Chosen_camera), index=False, header=False)
np.savetxt(cameras_matrix_folder + "/{}.csv".format(Chosen_camera), newcameramtx, delimiter=',', fmt='%f')

np.save(distortion_folder + "/{}.npy".format(Chosen_camera), dist)
pd.DataFrame(dist).to_excel(distortion_folder + "/{}.xlsx".format(Chosen_camera), index=False, header=False)
np.savetxt(distortion_folder + "/{}.csv".format(Chosen_camera), dist, delimiter=',', fmt='%f')
pass
