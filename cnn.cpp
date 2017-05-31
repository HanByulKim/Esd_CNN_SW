#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h> //tanh

#include "cnn.h"	
#include "config.h"

#define INFINITY

using namespace std;
w_t test_image[1000][1 * 28 * 28];

void conv(w_t *image,                           // max_pool1(12*12*5) input image
          pair<uint32_t, uint32_t> image_size,  // 12 12 input image size 12 12
          uint32_t num_features,                // 5 number of features in input = channel
          w_t *filter,                          // _weights_conv2 convolution filter source
          w_t *bias,                            // _bias_conv2 convolution bias source
          uint32_t num_filters,                 // 5 number of output 
          w_t *feature_map,                     // feature_map2(8*8*5) output image
          pair<uint32_t, uint32_t> filter_size, // 5 5 filter size
          int32_t pad,                          // 0 number of padding
          uint32_t stride) {                    // 1 number of stride
		//FIXME

		// tips
		pair<int32_t, int32_t> lt=make_pair((-1)*pad,(-1)*pad);
		pair<int32_t, int32_t> rt=make_pair((-1)*pad,0);
		pair<int32_t, int32_t> lb=make_pair(0,(-1)*pad);
		pair<int32_t, int32_t> rb=make_pair(0,0);
		
		pair<uint32_t, uint32_t> feature_size = make_pair((image_size.x+(2*pad)-filter_size.x)/stride+1,image_size.y+(2*pad)-filter_size.y)/stride+1);


		// Fit to output image

		for(int ch=0; ch<num_features; ch++){ //*12 *12 or *8 *8
			uint32_t channel = ch*feature_size.x*feature_size.y;
			for(int filt=0; filt<num_filters; filt++){
				uint32_t filter = filt*filter_size.x*filter_size.y;
				for(int i=0; i<feature_size.x; i++){ //8
					for(int j=0; j<feature_size.y; j++){ //8
						// iteration inside filter
						for(int lt_i=lt.x; lt_i<=lb.x; lt_i++){ //i
							for(int lt_j=lt.y; lt_j<=rt.y; lt_j++){ //j
								if(lt_i>=0 && lt_j>=0){
									feature_map[channel+feature_size.y*i+j]+=filter[filter+filter_size.y*(lt_i-lt.x)+(lt_j-lt.y)]*image[image_size.y*lt_i+lt_j];
								}
							}
						}

						// update tips(think as 2d)
						pair<int32_t, int32_t> temp_f=make_pair(stride,0);
						lt=lt+temp_f;
						rt=rt+temp_f;
						lb=lb+temp_f;
						rb=rb+temp_f;
					}
					// update tips(think as 2d)
					pair<int32_t, int32_t> temp=make_pair(0,stride);
					lt.x=0; lb.x=0;
					rt.x=filter_size.x-1; rb.x=filter_size.x-1;
					lt=lt+temp;
					rt=rt+temp;
					lb=lb+temp;
					rb=rb+temp;
				}
			}
		}

		//bias
		for(int i=0; i<feature_size.x; i++){ //8
			for(int j=0; j<feature_size.y; j++){ //8
				feature_map[feature_size.y*i+j]+=bias[feature_size.y*i+j];
			}
		}
}

void max_pool(w_t *image,																// input image
							pair<uint32_t, uint32_t> image_size,			// input image size
							uint32_t channel,													// number of features in input image = channel
							pair<uint32_t, uint32_t> max_pool_size,		// pooling size
							uint32_t stride,													// strdie
							w_t *max_pool) {													// output image
		//FIXME
		// tips
		pair<int32_t, int32_t> lt=make_pair(0,0);
		pair<int32_t, int32_t> rt=make_pair(max_pool_size.x-1,0);
		pair<int32_t, int32_t> lb=make_pair(0,max_pool_size.y-1);
		pair<int32_t, int32_t> rb=make_pair(max_pool_size.x-1,max_pool_size.y-1);
		int32_t temp_m;

		// Fit to output image
		for(int i=0; i<(image_size.x-max_pool_size.x)/stride+1; i++){
			for(int j=0; j<(image_size.y-max_pool_size.y)/stride+1; j++){
				temp_m = (-1)*INFINITY;
				// iteration inside filter
				for(int lt_i=lt.x; lt_i<=lb.x; lt_i++){ //i
					for(int lt_j=lt.y; lt_j<=rt.y; lt_j++){ //j
						if(temp_m<image[lt_i][lt_j])
							temp_m=image[lt_i][lt_j];
					}
				}
				max_pool[i][j]=temp_m;
				// update tips
				pair<int32_t, int32_t> temp_f=make_pair(max_pool_size.x,0);
				lt=lt+temp_f;
				rt=rt+temp_f;
				lb=lb+temp_f;
				rb=rb+temp_f;
			}
			// update tips
			pair<int32_t, int32_t> temp=make_pair(0,max_pool_size.y);
			lt.x=0; lb.x=0;
			rt.x=filter_size.x-1; rb.x=filter_size.x-1;
			lt=lt+temp;
			rt=rt+temp;
			lb=lb+temp;
			rb=rb+temp;
		}

}

void ReLu(w_t *image,
				pair<uint32_t, uint32_t> image_size,
				uint32_t num_output,												
				w_t *output) {
		//FIXME		//FIXME
		// tips

		// Fit to output image
		for(int i=0; i<image_size.x-1; i++){
			for(int j=0; j<image_size.y-1; j++){
				if(image[i][j] > 0) output[i][j] = image[i][j];
				else output[i][j]=0;
			}
		}
}

void TanH(w_t *image, 															// input image
				pair<uint32_t, uint32_t> image_size,				// input image size
				uint32_t num_output,												// number of output feature
				w_t *output){																// output
		//FIXME
		for(int i=0; i<image_size.x-1; i++){
			for(int j=0; j<image_size.y-1; j++){
				output[i][j] = tanh(image[i][j]);
			}
		}
}

void ip(w_t *input, pair<uint32_t, uint32_t> input_size, //4 4			// input image
				uint32_t num_features,			// 5													// number of 1 input's features
				w_t *weight,																				// weights
				w_t *bias,																						// bias
				uint32_t num_output,																	// number of output neurons
				w_t *output){	
																									// output
		//FIXME 
		// tips
		
		// Fit to output image
		for(int i=0; i<input_size.x-1; i++){
			for(int j=0; j<input_size.y-1; j++){
				weight[i][j]*input[i][j]+bias[i][j];
			}
		}


}

/*
void accuracy(uint32_t iter,
							uint32_t *label,
							w_t *output){
		//FIXME
}
*/
