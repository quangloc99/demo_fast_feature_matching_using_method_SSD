# Demo match các FAST FEATURE bằng phương pháp SSD

## Cài đặt và chạy DEMO
    $ cmake .
    $ make
    $ ./main

## Sử dụng matcher
Hai file chính là FastFeatureMatcher.hpp và FastFeatureMatcher.cpp.
Matcher sử dụng cấu trúc dữ liệu BallTree nên cần có các file BallTree.cpp và BallTree.hpp để chạy.

Các hàm trong namespace FastFeatureMatcher:
- `std::vector<std::vector<float>> getDescriptors(cv::Mat image, const std::vector<cv::KeyPoint>&)` hàm nhận 1 ảnh và KeyPoints tương ứng khi được match bằng Fast Feature Detector của opencv. Trả về descriptors cho các KeyPoints là 16 điểm ảnh quanh mỗi keypoint.
-
`std::vector<cv::DMatch> match(cv::Mat img0, const std::vector<cv::KeyPoint>& kp0, cv::Mat img1, const std::vector<cv::KeyPoint>& kp1, float threshold = 5000);
`
hàm nhận 2 ảnh và KeyPoints tương ứng với chúng và trả về matching giữa 2 tập KeyPoint. Hàm này đã
gọi hàm `getDescriptors` nên không cần truyền thêm descriptors vào. Tham số `threshold` sử dụng để lọc
ra các matching tốt, tức là ta sẽ giữ lại các matching có `SSD < threshold`. Truyền vào `-1` nếu như
không muốn giới hạn threshold.

Ngoài ra sau khi lấy được KeyPoint từ Fast cần sử dụng KeyPointsFilter::retainBest để lọc ra các điểm nổi trội (khoảng 1000 điểm).
Nguyên nhân là có rất nhiều điểm nhiễu nếu không lọc và khi quá nhiều điểm thì code sẽ chạy lâu hơn.

## Tính hiệu quả
- Chất lượng: đây là phương pháp match giữa 2 frames liên tiếp nên không cần các param như rotate angle hay scale ratio.
Chất lượng khá tốt, sau khi test với bộ dataset thì số matching bị hụt rất thấp, có thể nói là không ảnh hưởng đến quá trình tính pos ;)
- Tốc độ: với 1000 KeyPoints matcher chạy trong thời gian 60-70ms.

## Chú thích
Dataset lấy từ [đây](https://tev.fbk.eu/technologies/collaborative-3d-reconstruction-with-smartphones).
