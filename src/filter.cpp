#include "filter.h"
#include <vector>

const int MIN_COUNT_PIXELS_BY_X = 30;
const int MIN_COUNT_PIXELS_BY_Y = 15;

struct helper {
public:
	int len;
	int start;
	int row;
	cv::Vec3b pixel;

	helper(int len, int start, int row, cv::Vec3b pixel) {
		this->len = len;
		this->start = start;
		this->row = row;
		this->pixel = pixel;
	}
};


cv::MatIterator_<cv::Vec3b> find_lines(const int row, 
	const cv::MatIterator_<cv::Vec3b> cbegin, 
	const cv::MatIterator_<cv::Vec3b> pointer,
	const cv::MatIterator_<cv::Vec3b> end,  
	std::map<int, std::vector<helper>>& lines) {

	auto found_match = std::adjacent_find(pointer, end);
	auto len = 0;

	if (found_match == end)
		return found_match;

	int index = std::distance(cbegin, found_match);
	for (auto i = found_match; i != end && *i == *found_match; i++) 
		len++;
	
	if (len > MIN_COUNT_PIXELS_BY_X) 
		lines[index].push_back(helper(len, index, row, *found_match));
	
	return found_match + len + 1;
}


bool is_equal(const helper& a, const helper& b) {
	return a.len == b.len && 
		a.start == b.start && 
		a.pixel == b.pixel &&
		std::abs(a.row - b.row) > MIN_COUNT_PIXELS_BY_Y;
}


bool is_rectangle(const helper& auditable, 
		   const std::vector<helper> collection, 
		   cv::Mat& img) {

	for each (auto current in collection)
	{
		if (!is_equal(auditable, current))
			continue;

		auto start = std::min(auditable.row, current.row);
		auto end = std::max(auditable.row, current.row);

		bool flag = true;
		for (auto i = start; i < end; i++) {
			if ((img.at<cv::Vec3b>(i, auditable.start) != auditable.pixel) &&
				(img.at<cv::Vec3b>(i, auditable.start + auditable.len) != auditable.pixel)) {
				flag = false;
				break;
			}				
		}
		if(flag)
			return true;
	}

	return false;
}


bool contains_rectangle(cv::Mat& img) {
	
	auto lines = std::map<int, std::vector<helper>>();

	for (auto row = 0; row < img.rows - 1; row++) {
		auto current_rows = img.rowRange(row, row + 1);
		auto cbegin = current_rows.begin<cv::Vec3b>();

		auto pointer = find_lines(row, cbegin, cbegin, current_rows.end<cv::Vec3b>(), lines);
		while (pointer != current_rows.end<cv::Vec3b>()) {
			pointer = find_lines(row, cbegin, pointer, current_rows.end<cv::Vec3b>(), lines);
		}
	}

	for each (auto pair in lines)
	{		
		if (pair.second.size() < 2)
			continue;

		for each (auto h in pair.second)
		{
			if (is_rectangle(h, pair.second, img))
				return true;
		}
	}

	return false;
}