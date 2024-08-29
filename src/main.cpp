#include <iostream>
#include "opencv2/opencv.hpp"
#include "filter.h"


const std::string DIRECTORY_IMAGES = "D:\\opencv\\ObjectDetection\\images";

std::string pathCombine(const std::string dir, const std::string file) {
	return dir + "\\" + file;
}

int main() {

	std::string file = "4.png";
	auto pathToImage = pathCombine(DIRECTORY_IMAGES, file);

	auto img = cv::imread(pathToImage);
	auto flag = contains_rectangle(img);

	std::cout << flag << std::endl;
}