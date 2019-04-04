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

    int dst_height;
    int dst_width;
    
    cv::Mat pad1,pad2,pad3,a1,a2,a3;

    // src.ptr<unsigned char>(i)[ calculate INDEX ]

    // MAKE YOUR OWN CONVOLUTION PROCESS
    for(int i=0; i<src_height; i++){
		for(int j=0; j<src_width/3; j++){
			for(int k=j*3; k<j*3+2; k++){
				pad1<float>(i+padding,j+padding) = src<float>(i,k);
				pad2<float>(i+padding,j+padding) = src<float>(i,k+1);
				pad3<float>(i+padding,j+padding) = src<float>(i,k+2);
			}
		}
    }
    for(int i=0; i<((src_height-kernel_height+2*padding)/stride)+1; i++){
		for(int j=0; j<((src_width-kernel_width+2*padding)/stride)+1; j++){
			for(int k=0; k<kernel_height; k++){
				for(int l=0; l<kernel_width; l++){
					a1<float>(i,j) +=pad1<float>(k+i*stride,l+j*stride)*kernel<float>(l,k);
					a2<float>(i,j) +=pad2<float>(k+i*stride,l+j*stride)*kernel<float>(l,k);
					a3<float>(i,j) +=pad3<float>(k+i*stride,l+j*stride)*kernel<float>(l,k);
				}
			}
		}
    }
    for(int i=0; i<src_height; i++){
		for(int j=0; j<src_width/3; j++){
			for(int k=j*3; k<j*3+2; k++){
				dst<float>(i,j) = a1<float>(i,k);
				dst<float>(i,j) = a1<float>(i,k+2);
				dst<float>(i,j) = a1<float>(i,k+3);
			}
		}
    }



    // if success
    return 0

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
    kernel = (cv::Mat_<float>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1);


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
