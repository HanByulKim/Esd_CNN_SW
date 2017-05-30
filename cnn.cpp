#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>

#include "cnn.h"	
#include "config.h"

using namespace std;
w_t test_image[1000][1 * 28 * 28];

void conv(w_t *image,                           // input image
          pair<uint32_t, uint32_t> image_size,  // input image size
          uint32_t num_features,                // number of features in input = channel
          w_t *filter,                          // convolution filter source
          w_t *bias,                            // convolution bias source
          uint32_t num_filters,                 // number of output 
          w_t *feature_map,                     // output image
          pair<uint32_t, uint32_t> filter_size, // filter size
          int32_t pad,                          // number of padding
          uint32_t stride) {                    // number of stride
		//FIXME

		// tips
		pair<int32_t, int32_t> lt=make_pair((-1)*pad,(-1)*pad);
		pair<int32_t, int32_t> rt=make_pair((-1)*pad,0);
		pair<int32_t, int32_t> lb=make_pair(0,(-1)*pad);
		pair<int32_t, int32_t> rb=make_pair(0,0);
		
		// filter 5개였따..
		// Fit to output image
		for(int i=0; i<(image_size.x+(2*pad)-filter_size.x)/stride+1; i++){
			for(int j=0; j<(image_size.y+(2*pad)-filter_size.y)/stride+1; j++){
				// iteration inside filter
				for(int lt_i=lt.x; lt_i<=lb.x; lt_i++){ //i
					for(int lt_j=lt.y; lt_j<=rt.y; lt_j++){ //j
						if(lt_i>=0 && lt_j>=0){
							feature_map[i][j]+=filter[lt_i-lt.x][lt_j-lt.y]*image[lt_i][lt_j];
						}
					}
				}

				// update tips
				pair<int32_t, int32_t> temp_f=make_pair(stride,0);
				lt=lt+temp_f;
				rt=rt+temp_f;
				lb=lb+temp_f;
				rb=rb+temp_f;
			}
			// update tips
			pair<int32_t, int32_t> temp=make_pair(0,stride);
			lt.x=0; lb.x=0;
			rt.x=filter_size.x-1; rb.x=filter_size.x-1;
			lt=lt+temp;
			rt=rt+temp;
			lb=lb+temp;
			rb=rb+temp;
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
		pair<int32_t, int32_t> rt=make_pair(max_pool_size-1,0);
		pair<int32_t, int32_t> lb=make_pair(0,max_pool_size-1);
		pair<int32_t, int32_t> rb=make_pair(max_pool_size-1,max_pool_size-1);

		// Fit to output image
		for(int i=0; i<(image_size.x-max_pool_size.x)/stride+1; i++){
			for(int j=0; j<(image_size.y-max_pool_size.y)/stride+1; j++){
				// iteration inside filter
				for(int lt_i=lt.x; lt_i<=lb.x; lt_i++){ //i
					for(int lt_j=lt.y; lt_j<=rt.y; lt_j++){ //j
						if(lt_i>=0 && lt_j>=0){
							feature_map[i][j]+=filter[lt_i-lt.x][lt_j-lt.y]*image[lt_i][lt_j];
						}
					}
				}

				// update tips
				pair<int32_t, int32_t> temp_f=make_pair(max_pool_size,0);
				lt=lt+temp_f;
				rt=rt+temp_f;
				lb=lb+temp_f;
				rb=rb+temp_f;
			}
			// update tips
			pair<int32_t, int32_t> temp=make_pair(0,max_pool_size);
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
		//FIXME
}

void TanH(w_t *image, 															// input image
				pair<uint32_t, uint32_t> image_size,				// input image size
				uint32_t num_output,												// number of output feature
				w_t *output){																// output
		//FIXME
}

void ip(w_t *input, pair<uint32_t, uint32_t> input_size,			// input image
				uint32_t num_features,																// number of 1 input's features
				w_t *weight,																					// weights
				w_t *bias,																						// bias
				uint32_t num_output,																	// number of output neurons
				w_t *output){																					// output
		//FIXME
}

/*
void accuracy(uint32_t iter,
							uint32_t *label,
							w_t *output){
		//FIXME
}
*/
