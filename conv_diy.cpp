#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int your_conv( cv::Mat src,
               cv::Mat dst,
               cv::Mat kernel,
               int stride,
               int padding
               )
{
    int src_height = src.rows;
    int src_width = src.cols;

    int kernel_height = kernel.rows;
    int kernel_width = kernel.cols;

    int dst_height = (src_height-kernel_height+2*padding)/stride + 1;
    int dst_width = (src_width-kernel_width+2*padding)/stride + 1;
    
    int a_height = (src_height-kernel_height+2*padding)/stride + 1;
    int a_width = (src_height-kernel_height+2*padding)/stride + 1;

    int pad_height = src_height + 2*padding;
    int pad_width = src_width + 2*padding;

    cv::Mat pad1,pad2,pad3,a1,a2,a3;

    a1 = (cv::Mat_<float>(a_height, a_width));
    a2 = (cv::Mat_<float>(a_height, a_width));
    a3 = (cv::Mat_<float>(a_height, a_width));
    pad1 = (cv::Mat_<float>(pad_height, pad_width));
    pad2 = (cv::Mat_<float>(pad_height, pad_width));
    pad3 = (cv::Mat_<float>(pad_height, pad_width));
    // src.ptr<unsigned char>(i)[ calculate INDEX ]
    // MAKE YOUR OWN CONVOLUTION PROCESS
    for(int i=0; i<src_height; i++){
		for(int j=0; j<src_width; j++){
			pad1.at<float>(i+padding,j+padding) = src.at<float>(i,j);
		}
    }
    for(int i=1; i<src_height; i++){
		for(int j=0; j<src_width; j++){
			pad2.at<float>(i+padding,j+padding) = src.at<float>(i,j);
		}
    }
    for(int i=0; i<src_height; i++){
		for(int j=0; j<src_width; j++){
			pad3.at<float>(i+padding,j+padding) = src.at<float>(i,j);
		}
    }
    
    for(int i=0; i<a_height; i++){
		for(int j=0; j<a_width; j++){
			for(int k=0; k<kernel_height; k++){
				for(int l=0; l<kernel_width; l++){
					a1.at<float>(i,j) +=pad1.at<float>(k+i*stride,l+j*stride)*kernel.at<float>(l,k,0);
					a2.at<float>(i,j) +=pad2.at<float>(k+i*stride,l+j*stride)*kernel.at<float>(l,k,1);
					a3.at<float>(i,j) +=pad3.at<float>(k+i*stride,l+j*stride)*kernel.at<float>(l,k,2);
				}
			}
		}
    }
    for(int i=0; i<dst_height; i++){
		for(int j=0; j<dst_width; j+=3){
			for(int l=0; l<dst_height/3; l++){
				int k=j/3;
				dst.at<float>(i,j) = a1.at<float>(l,k);
				dst.at<float>(i,j+1) = a2.at<float>(l,k);
				dst.at<float>(i,j+2) = a3.at<float>(l,k);
			}
		}
    }
    
    
    
    // if success
    return 0;

    // if fail - in the case of wrong parameters...
    // return -1
}

int main ( int argc, char** argv )
{
    if (argc < 2) {
        std::cout << "no filename given." << std::endl;
        std::cout << "usage: " << argv[0] << " image" << std::endl;
        return -1;
    }

    cv::Mat src, kernel, dst;

    // Load an image
    src = cv::imread( argv[1] );
    if( !src.data )  { return -1; }

    // Make filter
    kernel = (cv::Mat_<float>(3, 3, 3) << -1, -1, -1, -1,  8, -1, -1, -1, -1,
					   0,  0,  0,  0,  0,  0,  0,  0,  0,
 					   0,  0,  0,  0,  0,  0,  0,  0,  0);


    // Run 2D filter
    //cv::filter2D(src, dst, -1 , kernel, cv::Point( -1, -1 ), 0, cv::BORDER_DEFAULT );

    // ---------------------------
    your_conv(src,dst,kernel,1,0);
    // ---------------------------

    cv::namedWindow( "filter2D Demo", cv::WINDOW_AUTOSIZE );
    cv::imshow( "filter2D Demo", dst );

    cv::waitKey(0);
    return 0;
}
