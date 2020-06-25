# GraduationProject-Autonomous Car</br>
The car advancing on rough terrain.</br></br>
**Description:**</br>
An image of the region will be taken with the depth camera in the environment and obstacle recognition and detection will be made by 3d image processing. 
Then, it will be sent to the Arduino's MCU via Bluetooth and the car will advance to the target without hitting the obstacles.</br></br>
**Hardware:**<br>
`R200 Intel Realsense Depth Camera`</br>
`Ardunio Mega`</br>
`The Car Platform`</br>
`Ground Mat`</br>
`Obstacles such as some 3d geometric shapes`</br>
`Camera Stabilizer Support`</br></br>
**Software:**</br>
`PCL (Point Cloud Library) for cloud processing`</br>
`C++`</br>
`Visual Studio 2015`</br>
`Intel RealSense SDK 1.0`</br>
`Intel RealSense R200 Depth Camera Manager 2.1.27.2853`</br>
`Cloud Compare or MeshLab `</br></br>

This project consists of 3 stages which are 3D Image Processing, Path Planning and advancing the car with Arduino.
But the last stage could not be realized.</br>

**Stage 1: 3D Image Processing**</br><br>
The first thing we needed to do was create the raw dataset.
For this purpose, we bought the equipment necessary for the scene setup with my project friend Meliha Dikmen and realized the scene setup. 
With the Intel Real Sense R200 depth camera, we took the image of the scene and objects at a height of 2 meters.
Then we saved it as a file with the .pcd(point cloud data) extension and created our dataset.We were able to view our file using Cloud Compare. </br></br>
![RawData_Side](https://github.com/busragol/GraduationProject/blob/master/Images/RawData_Side.PNG)</br>
Figure 1: Raw data from side</br></br>
![RawData_Top](https://github.com/busragol/GraduationProject/blob/master/Images/rawdata_Top.PNG)</br>
Figure 2: Raw Data from top side</br></br>
![RawData_Down](https://github.com/busragol/GraduationProject/blob/master/Images/RawData_Down.PNG)</br>
Figure 3:Raw data from down side</br></br>
After creating the dataset, we had to segment the plane and objects. We decided to start with implementation of Ground Plane Segmentation, 
and we used RANSAC(Random Sample Consensus) algorithm for that.</br></br>
![Filtered_Data](https://github.com/busragol/GraduationProject/blob/master/Images/Filtered_Data.PNG)</br>
Figure 4: Ground Plane after Ground Plane Segmentation</br></br>
Then we continued with object segmentation. And we used Euclidean Clustering algorithm for this.</br></br>
![EuclideanClustering](https://github.com/busragol/GraduationProject/blob/master/Images/EuclideanClustering.PNG)</br></br>
Figure 5: Dataset after Euclidean Clustering</br></br>
Now that we have finished our segmentation processes, we can proceed the detection process. For object detection, we first thought of using bounding boxes.
We did not need to find a 3-dimensional bounding box because the car would not be displaced in the 3rd dimension. For us, only the width and height measurements were sufficient.
So we decided to find a 2-dimensional bounding box.For this we firstly applied projection. </br></br>
![Projected_GroundSide](https://github.com/busragol/GraduationProject/blob/master/Images/Projected_GroundSide.PNG)</br>
Figure 6: Projected ground from side
![Projecter_GroundTop](https://github.com/busragol/GraduationProject/blob/master/Images/Projected_CloudTop.PNG)</br>
Figure 7: Projected ground from top side</br></br>
And then we decided to find convex hull for boundary area.</br>
![ConvexHull](https://github.com/busragol/GraduationProject/blob/master/Images/ConvexHull.PNG)</br>
Figure 8: Convex Hull of Projection of Objects</br></br>
And we finished the first stage of my graduation project, is the 3d image processing stage.</br></br>

**Stage 2: Path Planning**</br>
Now that we have finished the 3D image processing stage, we had to set the starting and target points for the car. 
Then we had to find the shortest path to get the car to the destination.
Dijkstra algorithm was the most suitable for us in the shortest path algorithms because it was both faster and lower cost.

While implementing path planning, we drew a road for ourselves. 
And we took steps by adhering to this road. The our road consists of these steps: Creating visibility graph from convex hulls, Weighting the edges of the visibility graph,
Finding the shortest path with Dijkstra. 
It seemed more comfortable to implement path planning with Java. But we had a problem.
Our Convex hull coordinates were very large and contained negative values. Java only works with positive values, 
and the STDDRAW library we use for graphical operations accepts values between 0 and 1. As a solution to this, 
we first added the smallest negative value to all coordinate points. So we offset all the points equally. 
We divide them all by 10 ^ 3 to ensure that there are values between 0 and 1. But we still had values exceeding 1. To get rid of this, 
we divide all the coordinates by 2.</br>
In the final of the path planning stage, we found shortest path between every point.We could change source and destination points from file.</br></br>
![PathPlaning](https://github.com/busragol/GraduationProject/blob/master/Images/PathPlaning.PNG)
Figure 9: Path Planning Result</br></br>
**Stage 3: Advancing the car with Ardunio**</br>
This stage could not be realized.
</br></br>
**NOTE**</br>
You can go to the following link for setup steps.</br>
[Setup Steps](https://coderxx.blogspot.com/2020/03/image-processing-goruntu-isleme.html)
