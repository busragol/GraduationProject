# GraduationProject-Autonomous Car</br>
The car advancing on rough terrain.</br></br>
**Introduction of Project:**</br>
In this project we aimed to make otonomous car that can proceed without hitting the obstacles.Firstly,we took a image of ground that we create using ground mat and 3d geometric shapes as obstacles using 3d Intel RealSense Depth Camera.Then we considered project as 3 stage that are 3d Image Processing,Path Planning and Ardunio but Ardunio step could not be realized.
</br></br>
We used the following requirements:</br>
                                                                      **Hardware:**<br>
                                                               `R200 Intel Realsense Depth Camera to take image`</br>
                                                               `Ardunio Mega`</br>
                                                               `The Car Platform`</br>
                                                               `Ground Mat`</br>
                                                               `Obstacles such as some 3d geometric shapes`</br>
                                                               `Camera Stabilizer Support`</br></br>
                                                                      **Software:**</br>
                                                               `PCL (Point Cloud Library) for cloud processing`</br>
                                                               `C++ Programming Languages`</br>
                                                               `Visual Studio 2015 Update3`</br>
                                                               `Intel RealSense SDK 1.0`</br>
                                                               `Intel RealSense R200 Depth Camera Manager 2.1.27.2853`</br>
                                                               `Cloud Compare or MeshLab to view the data`</br></br>
**Stage 1: 3D Image Processing**</br><br>
In the first thing,we have implemented code to take an image from the ground that we created before.We used Camera Stabilitizer to fix camera and to take image with R200 Intel RealSense Depth Camera and received raw data using this code.Then we saved raw data as pcd(point cloud data) file.We were able to view our file using Cloud Compare. </br></br>
![RawData_Side](https://github.com/busragol/GraduationProject/blob/master/Images/RawData_Side.PNG)</br>
Figure 1: Raw data from side</br></br>
![RawData_Top](https://github.com/busragol/GraduationProject/blob/master/Images/rawdata_Top.PNG)</br>
Figure 2: Raw Data from top side</br></br>
![RawData_Down](https://github.com/busragol/GraduationProject/blob/master/Images/RawData_Down.PNG)</br>
Figure 3:Raw data from down side</br></br>
After receive raw data,we applied Ground Plane Segmentation to detect the largest plane in a point cloud and we used RANSAC (Random Sample Consensus) method.RANSAC detects inliers from input data.</br></br>
![Filtered_Data](https://github.com/busragol/GraduationProject/blob/master/Images/Filtered_Data.PNG)</br>
Figure 4: Ground Plane after Ground Plane Segmentation</br></br>
And then we applied Euclidean Clustering Algorithm to extract the our boxes as clusters.This step is Object Segmentation.</br></br>
![EuclideanClustering](https://github.com/busragol/GraduationProject/blob/master/Images/EuclideanClustering.PNG)</br>
Figure 5: Dataset after Euclidean Clustering</br></br>
Then in the next step we implement 3d bounding box but we didn't need because only x and y coordinate is enough for project.The car won't move on the z axis.Thus,we decided to implement 2d bounding box and for this we firstly applied projection. </br></br>
![Projected_GroundSide](https://github.com/busragol/GraduationProject/blob/master/Images/Projected_GroundSide.PNG)</br>
Figure 6: Projected ground from side
![Projecter_GroundTop](https://github.com/busragol/GraduationProject/blob/master/Images/Projected_CloudTop.PNG)</br>
Figure 7: Projected ground from top side</br></br>
In the next step,we decided to find convex hull for boundary area.</br>
![ConvexHull](https://github.com/busragol/GraduationProject/blob/master/Images/ConvexHull.PNG)</br>
Figure 8: Convex Hull of Projection of Objects</br></br>
Finally, we finished first stage.</br></br>

**Stage 2: Path Planning**</br>
In the first stage we finished 3d Image Processing and now we are in second stage is Path Planning.In this stage we firstly researched Path Planning Algortihms and Dijkstra Algorithm was suitable for path planning.Then we researched how we implement Dijkstra Algorithm from Convex Hull of the boxes.We drew a raodmap for this.We firstly should visibility graph.Visibility Graph is a graph of intervisible locations,typically for a set of points and obstacles in the Euclidean plane.With Visibility graph we already edges between the points of each convex.Then we gave weigth to edges because Dijkstra Algorithm need this weight to find shortest path.Final of this roadmap is Dijkstra Algorithm.We implement these steps in Eclipse with Java Programming Languages.
In the final of the path planning stage, we found shortest path between every point.We could change source and destination points from file.</br></br>
![PathPlaning](https://github.com/busragol/GraduationProject/blob/master/Images/PathPlaning.PNG)
Figure 9: Path Planning Result</br></br>
**Stage 3: Ardunio**</br>
This stage could not be realized.
</br></br>
**NOTE**</br>
You can go to the following link for setup steps.</br>
[Setup Steps](https://coderxx.blogspot.com/2020/03/image-processing-goruntu-isleme.html)
