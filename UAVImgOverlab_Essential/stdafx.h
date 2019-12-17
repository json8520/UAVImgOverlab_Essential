// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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