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
}

void max_pool(w_t *image,																// input image
							pair<uint32_t, uint32_t> image_size,			// input image size
							uint32_t channel,													// number of features in input image = channel
							pair<uint32_t, uint32_t> max_pool_size,		// pooling size
							uint32_t stride,													// strdie
							w_t *max_pool) {													// output image
		//FIXME
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
