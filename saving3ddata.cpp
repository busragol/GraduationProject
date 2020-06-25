#include <windows.h>
#include <wchar.h>
#include "pxcsensemanager.h"
#include <opencv2/opencv.hpp>
#include <iostream> 
#include "util_render.h"
//#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/io/ply_io.h>

using namespace std;
#define MAX_FRAMES 150
int wmain(int argc, WCHAR* argv[]) {
	// initialize the util render
	//UtilRender *renderColor = new UtilRender(L"COLOR STREAM");
	UtilRender *renderDepth = new UtilRender(L"DEPTH STREAM");
	// create the PXCSenseManager
	PXCSenseManager *psm = 0;
	psm = PXCSenseManager::CreateInstance();
	if (!psm) {
		wprintf_s(L"Unable to create the PXCSenseManager\n");
		return 1;
	}
	// select the color stream of size 640x480 and depth stream of size 320x240
	//psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 640, 480);
	psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 480, 360);
	// initialize the PXCSenseManager
	if (psm->Init() != PXC_STATUS_NO_ERROR) return 2;
	//PXCImage *colorIm, *depthIm;
	//PXCImage::ImageData depthImage;
	//PXCPoint3DF32 * pos2d = new PXCPoint3DF32[480 * 360];
	PXCPoint3DF32 *pos3D = new PXCPoint3DF32[480 * 360];
	for (int i = 0; i < MAX_FRAMES; i++) {
		// This function blocks until all streams are ready (depth and color)
		// if false streams will be unaligned
		if (psm->AcquireFrame(true) < PXC_STATUS_NO_ERROR) break;
		// retrieve all available image samples
		PXCCapture::Sample *sample = psm->QuerySample();
		PXCImage  *depthMap = sample->depth;
		renderDepth->RenderFrame(sample->depth);
		PXCImage::ImageData depthImage;
		depthMap->AcquireAccess(PXCImage::ACCESS_READ, &depthImage);
		PXCImage::ImageInfo imgInfo = depthMap->QueryInfo();
		int depth_stride = depthImage.pitches[0] / sizeof(pxcU16);
		PXCPoint3DF32 * pos3d = new PXCPoint3DF32[480 * 360];
		PXCCaptureManager* captureManager = psm->QueryCaptureManager();
		PXCCapture::Device* device = captureManager->QueryDevice();
		PXCProjection* projection = device->CreateProjection();
		//PXCProjection * projection = device->CreateProjection();
		pxcU16 *dpixels = (pxcU16*)depthImage.planes[0];
		unsigned int dpitch = depthImage.pitches[0] / sizeof(pxcU16);
		//PXCPoint3DF32 *pos3D = new PXCPoint3DF32[480*360];
		pxcStatus sts = psm->AcquireFrame(true);
		sts = projection->QueryVertices(depthMap, &pos3D[0]);
		if (sts < pxcStatus::PXC_STATUS_NO_ERROR) {
			wprintf_s(L"Projection was unsuccessful! \n");
			psm->Close();
		}


		// retrieve the image or frame by type from the sample
		//colorIm = sample->color;
		//depthIm = sample->depth;
		//depthIm->AcquireAccess(PXCImage::ACCESS_READ, &depthImage);
		//PXCImage::ImageInfo imgInfo = depthIm->QueryInfo();
		//cout << "inImg->QueryInfo() " << depthImage.pitches<< endl;

		//int depth_stride = depthImage.pitches[0] / sizeof(pxcU16);
		//cout << depth_stride << endl;

		/*for (int y = 0, k = 0; y < 360; y++) {
		for (int x = 0; x < 480; x++, k++) {
		pos2d[k].x = (pxcF32)x;
		pos2d[k].y = (pxcF32)y;
		pos2d[k].z = ((short *)depthImage.planes[0])[y* depth_stride + x];

		}
		}*/
		//cout << "x:  " << pos2d->x;
		// render the frame
		//if (!renderColor->RenderFrame(colorIm)) break;
		if (!renderDepth->RenderFrame(depthMap)) break;
		// release or unlock the current frame to fetch the next frame
		psm->ReleaseFrame();
	}

	for (int k = 0; k < 480 * 360; k++) {

		if (pos3D[k].x != 0) {
			cout << " xx is: " << pos3D[k].x << endl;
		}

		if (pos3D[k].y != 0) {

			cout << " yy is: " << pos3D[k].y << endl;
		}
		if (pos3D[k].z != 0) {
			cout << " zz is: " << pos3D[k].z << endl;
		}
		//Sleep(100);
	}
	pcl::PointCloud<pcl::PointXYZRGBNormal> cloud;
	// Fill in the cloud data
	cloud.width = 480;
	cloud.height = 360;
	cloud.is_dense = true;
	cloud.points.resize(cloud.width * cloud.height);
	cloud.sensor_orientation_ = Eigen::Quaternionf(0, 1, 0, 0);

	int writeCount = 0;
	for (std::size_t i = 0; i < cloud.points.size(); ++i)
	{
	cloud.points[i].x = pos3D[i].x;
	cloud.points[i].y = pos3D[i].y;
	cloud.points[i].z = pos3D[i].z;
	cloud.points[i].normal[0] = 1.f;
	cloud.points[i].normal[1] = 0.f;
	cloud.points[i].normal[2] = 0.0f;
	cloud.points[i].r = 255;
	cloud.points[i].g = 125;
	cloud.points[i].b = 0;
	}
	writeCount++;
	//char filename[100];
	//sprintf(filename, "C:/Users/Hp/Desktop/ornek.xyz", writeCount);
	//pcl::io::savePCDFileASCII(filename, cloud);
	pcl::io::savePLYFile("C:/Users/Busra/Desktop/sahne_ply.ply",cloud);
	std::cerr << "Saved " << cloud.points.size() << " data points to sahne.ply." << std::endl;
	for (std::size_t i = 0; i < cloud.points.size(); ++i)
	std::cerr << "    " << cloud.points[i].x << " " << cloud.points[i].y << " " << cloud.points[i].z << std::endl;
	

	/*PXCCaptureManager* captureManager = psm->QueryCaptureManager();
	PXCCapture::Device* device = captureManager->QueryDevice();
	PXCProjection* projection = device->CreateProjection();
	unsigned int wxhDepth = 480 * 360;
	//PXCPoint3DF32 *pos3d = new PXCPoint3DF32[wxhDepth];
	//Point3DF32 * pos3d = NULL;
	PXCPoint3DF32 * pos3d = new PXCPoint3DF32[480 * 360];
	PXCPoint3DF32 * vertices = new PXCPoint3DF32[480 * 360];
	//Projection * projection = device->CreateProjection();

	projection->ProjectDepthToCamera(480 * 360, pos2d, pos3d);
	//projection->QueryVertices(depthIm, vertices);*****************************************************************
	cout << "x is " << pos3d->x * 1000 << endl;
	cout << "y is " << pos3d->y * 1000 << endl;
	cout << "z is " << pos3d->z * 1000 << endl;*/

	//delete renderColor;
	delete renderDepth;
	// close the last opened streams and release any session and processing module instances
	psm->Release();
	return 0;
}
/*
// Create a SenseManager instance.
PXCSenseManager *sm = 0;// PXCSenseManager::CreateInstance();
sm = PXCSenseManager::CreateInstance();
// Set the coordinate system
PXCSession *session = sm->QuerySession();
//session->SetCoordinateSystem(PXCSession::COORDINATE_SYSTEM_OPENCV);
// Select the color and depth streams
sm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 640,480);
sm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 480,360);
// Initialize and Stream Samples
sm->Init();
PXCImage::ImageData depthImage;

PXCCapture::Sample *sample = sm->QuerySample();
PXCImage *depthMap = sample->depth;

depthMap->AcquireAccess(PXCImage::ACCESS_READ, &depthImage);
PXCImage::ImageInfo imgInfo = depthMap->QueryInfo();
cout << "inImg->QueryInfo() " << depthImage.format << endl;
PXCPoint3DF32 * pos2d = new PXCPoint3DF32[480*360];
int depth_stride = depthImage.pitches[0] / sizeof(pxcU16);

for (int y = 0, k = 0; y < 360; y++) {
for (int x = 0; x < 480; x++, k++) {
pos2d[k].x = (pxcF32)x;
pos2d[k].y = (pxcF32)y;
pos2d[k].z = ((short *)depthImage.planes[0])[y* depth_stride + x];
}
}
//int depth_stride = depthImage.pitches[0] / sizeof(pxcU16);
//Get projectizon operator
PXCCaptureManager* captureManager = sm->QueryCaptureManager();
PXCCapture::Device* device = captureManager->QueryDevice();
PXCProjection* projection = device->CreateProjection();
unsigned int wxhDepth = 480 * 360;
//PXCPoint3DF32 *pos3d = new PXCPoint3DF32[wxhDepth];
//Point3DF32 * pos3d = NULL;
PXCPoint3DF32 * pos3d = new PXCPoint3DF32[480*360];
PXCPoint3DF32 * vertices = new PXCPoint3DF32[480 * 360];
//Projection * projection = device->CreateProjection();
projection->ProjectDepthToCamera(480*360, pos2d, pos3d);
cout << "x is " << pos3d->x * 1000 << endl;
cout << "y is " << pos3d->y * 1000 << endl;
cout << "z is " << pos3d->z * 1000 << endl;
// create the array of depth coordinates + depth value (posUVZ) within the defined ROI
//PXCPoint3DF32* posUVZ = new PXCPoint3DF32[wxhDepth];
//pxcU16 *dpixels = (pxcU16*)depthImage.planes[0];
//unsigned int dpitch = depthImage.pitches[0] / sizeof(pxcU16);
//PXCPoint3DF32 *pos3D = new PXCPoint3DF32[wxhDepth];






// Clean up
sm->Release();*/

/*#include "util_render.h"  //SDK provided utility class used for rendering (packaged in libpxcutils.lib)
// maximum number of frames to process if user did not close the rendering window
#define MAX_FRAMES 5000
int wmain(int argc, WCHAR* argv[]) {
// initialize the util render
UtilRender *renderColor = new UtilRender(L"COLOR STREAM");
UtilRender *renderDepth = new UtilRender(L"DEPTH STREAM");
// create the PXCSenseManager
PXCSenseManager *psm = 0;
psm = PXCSenseManager::CreateInstance();
if (!psm) {
wprintf_s(L"Unable to create the PXCSenseManager\n");
return 1;
}
// select the color stream of size 640x480 and depth stream of size 320x240
psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 640, 480);
psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 480, 360);
// initialize the PXCSenseManager
if (psm->Init() != PXC_STATUS_NO_ERROR) return 2;
PXCImage *colorIm, *depthIm;
for (int i = 0; i<MAX_FRAMES; i++) {
// This function blocks until all streams are ready (depth and color)
// if false streams will be unaligned
if (psm->AcquireFrame(true)<PXC_STATUS_NO_ERROR) break;
// retrieve all available image samples
PXCCapture::Sample *sample = psm->QuerySample();
// retrieve the image or frame by type from the sample
colorIm = sample->color;
depthIm = sample->depth;
// render the frame
if (!renderColor->RenderFrame(colorIm)) break;
if (!renderDepth->RenderFrame(depthIm)) break;
// release or unlock the current frame to fetch the next frame
psm->ReleaseFrame();
}
UtilRender *renderColor = new UtilRender(L"COLOR STREAM");
UtilRender *renderDepth = new UtilRender(L"DEPTH STREAM");
// create the PXCSenseManager
PXCSenseManager *psm = 0;
psm = PXCSenseManager::CreateInstance();
if (!psm) {
wprintf_s(L"Unable to create the PXCSenseManager\n");
return 1;
}
// select the color stream of size 640x480 and depth stream of size 320x240
psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 640, 480);
psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 480, 360);
// initialize the PXCSenseManager
if (psm->Init() != PXC_STATUS_NO_ERROR) return 2;
PXCImage *colorIm, *depthIm;
for (int i = 0; i<MAX_FRAMES; i++) {
// This function blocks until all streams are ready (depth and color)
// if false streams will be unaligned
if (psm->AcquireFrame(true)<PXC_STATUS_NO_ERROR) break;
// retrieve all available image samples
PXCCapture::Sample *sample = psm->QuerySample();
// retrieve the image or frame by type from the sample
colorIm = sample->color;
depthIm = sample->depth;
// render the frame
if (!renderColor->RenderFrame(colorIm)) break;
if (!renderDepth->RenderFrame(depthIm)) break;
// release or unlock the current frame to fetch the next frame
psm->ReleaseFrame();
}
// delete the UtilRender instance
delete renderColor;
delete renderDepth;
// close the last opened streams and release any session and processing module instances
psm->Release();
return 0;
}
*/