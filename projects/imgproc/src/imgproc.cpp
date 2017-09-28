#include "imgproc/imgproc.h"
#include "common/logging.h"
#include "imgproc/find_square_contours.h"
#include "imgproc/apply_corner_cluster_filtering.h"
#include "imgproc/apply_size_filtering.h"
#include "imgproc/test/square_ordering_test.h"
#include "imgproc/order_squares.h"

namespace cbr
{

void find_board(const cv::Mat& image)
{
	const auto rawSquares = find_squares(image);
	auto filteredSquares = apply_size_filtering(rawSquares, image.size());
	filteredSquares = apply_cluster_filtering(filteredSquares);
	order_squares(filteredSquares);
	// run_square_ordering_test(filteredSquares);
}

}


