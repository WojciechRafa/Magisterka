import numpy as np
import cv2 as cv
import glob
import pandas as pd

# termination criteria
criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

p_x = 7
p_y = 9

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(p_x,5,0)
objp = np.zeros((p_x*p_y,3), np.float32)
objp[:, :2] = np.mgrid[0:p_y, 0:p_x].T.reshape(-1,2)

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

Chosen_camera = "Samsung_A50_WPR"
# Chosen_camera = "IPhone"

images = glob.glob('Calibration_Images/{}/*.png'.format(Chosen_camera), recursive=True)

ret_list = []
mtx_list = []
dist_list = []
rvecs_list = []
tvecs_list = []

alpha_x_list = []
alpha_y_list = []
vx_list = []
vy_list = []

idx_list = []

for idx, fname in enumerate(images):
    img = cv.imread(fname)
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    # Find the chess board corners
    ret, corners = cv.findChessboardCorners(gray, (p_y, p_x), None)

    # If found, add object points, image points (after refining them)
    if ret == True:
        objpoints.append(objp)

    corners2 = cv.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
    imgpoints.append(corners2)

    ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

    h, w = img.shape[:2]
    newcameramtx, roi = cv.getOptimalNewCameraMatrix(mtx, dist, (w, h), 1, (w, h))

    alpha_x_list.append(newcameramtx[0, 0])
    alpha_y_list.append(newcameramtx[1, 1])
    vx_list.append(newcameramtx[0, 2])
    vy_list.append(newcameramtx[1, 2])

    rot_matrix, _ = cv.Rodrigues(rvecs[-1])

    trans_matrix = np.eye(4)

    trans_matrix[0:3, 0:3] = rot_matrix

    trans_matrix[3, 0:3] = tvecs[-1].reshape((3,))

    print("\n")
    print(rot_matrix)
    print(tvecs)
    print("\n")

    ret_list.append(ret)
    mtx_list.append(mtx)
    dist_list.append(dist)
    rvecs_list.append(rvecs)
    tvecs_list.append(tvecs)

    print("\r", end="")
    print("\rProcent - {} %".format((100 * idx) // len(images)), end="")

    idx_list.append(idx)
    
    #if ret == True: 
    #    corners2 = cv.cornerSubPix(gray,corners, (11,11), (-1,-1), criteria)
    #    imgpoints.append(corners2)
    #    cv.drawChessboardCorners(gray, (p_y, p_x), corners2, ret)
    #    cv.imshow('img', gray)
    #    cv.waitKey(500)


cv.destroyAllWindows()

np.save("{}.npy".format(Chosen_camera), newcameramtx)
pd.DataFrame(newcameramtx).to_excel("{}.xlsx".format(Chosen_camera), index=False, header=False)
# df.to_excel("{}.npy".format(Chosen_camera), newcameramtx)

pass
