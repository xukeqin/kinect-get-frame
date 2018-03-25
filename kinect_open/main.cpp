/////////////////////////////////////////
/// COPYRIGHT NOTICE
/// All rights reserved.
///
/// kinect2深度图采集
///
/// @version 1.0
/// @author  七克 
/// @E-mail：xkeqin@gmail.com
/// @date    2018/3/25
/////////////////////////////////////////
#include <fstream>  
#include <conio.h>
#include <Windows.h>
#include <Kinect.h>
#include <opencv2\opencv.hpp>‘
#include <opencv2/imgproc/imgproc.hpp>
#include "stdio.h"

using namespace cv;
using namespace std;


char key;
// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

int main()
{
	/***Current Kinect***/
	IKinectSensor*          m_pKinectSensor;
	HRESULT hr;
	/********Depth********/
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;
	int count = 0;
	// Depth reader
	IDepthFrameReader*		m_pDepthFrameReader = NULL;
	

	hr = GetDefaultKinectSensor(&m_pKinectSensor);

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get the depth reader
		IDepthFrameSource* pDepthFrameSource = NULL;

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
		}

		SafeRelease(pDepthFrameSource);
	}

	while (1)
	{

		IDepthFrame* pDepthFrame = NULL;
		HRESULT hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

		if (SUCCEEDED(hr))
		{
			IFrameDescription* pFrameDescription = NULL;
			/*************************************************************************/
			//局部变量定义处
			/*************************************************************************/
			int nWidth = 512;
			int nHeight = 424;
			USHORT nDepthMinReliableDistance = 0;
			USHORT nDepthMaxDistance = 0;
			UINT nBufferSize = 0;
			UINT16 *pBuffer = NULL;
			USHORT depth;
			
		
			if (SUCCEEDED(hr))
			{

				hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
			}
			if (SUCCEEDED(hr))
			{
				hr = pFrameDescription->get_Width(&nWidth);
			}
			if (SUCCEEDED(hr))
			{
				hr = pFrameDescription->get_Height(&nHeight);
			}
			if (SUCCEEDED(hr))
			{
				hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
			}

			if (SUCCEEDED(hr))
			{
				// In order to see the full range of depth (including the less reliable far field depth)
				// we are setting nDepthMaxDistance to the extreme potential depth threshold
				nDepthMaxDistance = USHRT_MAX;

				// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
				//hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
			}
			if (SUCCEEDED(hr))
			{
				hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
			}

			if (SUCCEEDED(hr))
			{
				//DepthImage
				Mat DepthImage(nHeight, nWidth, CV_16UC1, pBuffer);
				Mat depth_image(nHeight, nWidth, CV_16UC1);
				const UINT16* pBufferEnd = pBuffer + (nWidth * nHeight);   //结束点

				depth_image = DepthImage.clone();

				char filename[200];
				
				sprintf(filename, "Picture%d.png", ++count); 
				imwrite(filename, depth_image);//图片保存到本工程目录中  
			
				imshow("depth", depth_image*100);

				waitKey(300);
			}
			SafeRelease(pFrameDescription);
		}
		
		SafeRelease(pDepthFrame);
	}
	return 0;
}

