// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
// Visual C++
#include <iostream>
#include <fstream>
#include <io.h>
using namespace std;

// OpenMP
#include <omp.h>

// OpenCV
#include <opencv2/opencv.hpp>
using namespace cv;

#ifdef _DEBUG

#pragma comment(lib,"C:\\JSon\\Library\\OpenCV348\\build\\x64\\vc14\\lib\\opencv_world348d.lib")

#else

#pragma comment(lib,"C:\\JSon\\Library\\OpenCV348\\build\\x64\\vc14\\lib\\opencv_world348.lib")

#endif