#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <unistd.h>
#include <iostream>
#include <cstdlib>  // for atoi

int main(int argc, char** argv)
{
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <width> <height> <pictureNumber> <mode: A|B>" << std::endl;
        return 1;
    }

    int targetWidth = std::atoi(argv[1]);
    int targetHeight = std::atoi(argv[2]);
    int pictureNumber = std::atoi(argv[3]);
    char mode = argv[4][0];

    int captureWidth, captureHeight;

    // Mode A: capture directly at desired resolution
    // Mode B: capture at lower resolution and upscale later

    if (mode == 'A') {
        captureWidth = targetWidth;
        captureHeight = targetHeight;
    } else if (mode == 'B') {
        captureWidth = targetWidth / 4;
        captureHeight = targetHeight / 4;
    } else {
        std::cerr << "Invalid mode. Use 'A' for native resolution, 'B' for upscale from lower resolution." << std::endl;
        return 1;
    }

    // Set up camera
    cv::VideoCapture cap;
    cap.set(cv::CAP_PROP_FRAME_WIDTH, captureWidth);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, captureHeight);
    cap.open(0);

    if (!cap.isOpened()) {
        std::cerr << "Failed to open camera." << std::endl;
        return 1;
    }

    const int w = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    const int h = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    fprintf(stderr, "Camera resolution: %d x %d\n", w, h);

    cv::Mat selected;

    for (int i = 0; i < pictureNumber; i++) {
        cv::Mat current;
        cap >> current;

        if (mode == 'B') {
            cv::resize(current, current, cv::Size(targetWidth, targetHeight), 0, 0, cv::INTER_CUBIC);
        }
        
        if (current.empty()) {
            std::cerr << "Empty frame at " << i << std::endl;
            return 1;
        }

        if (i == 0) {
            selected = current.clone();  // keep only this one
        }

    }

    cap.release();

    cv::imwrite("out.jpg", selected);

    std::cout << "Saved first frame out of " << pictureNumber <<  "as out.jpg (" << selected.cols << "x" << selected.rows << ")" << std::endl;

    return 0;
}
