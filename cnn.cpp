#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h> //tanh

#include "cnn.h"	
#include "config.h"



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

		
		pair<uint32_t, uint32_t> feature_size = make_pair((image_size.first+(2*pad)-filter_size.first)/stride+1,(image_size.second+(2*pad)-filter_size.second)/stride+1);


		for (int filt = 0; filt < num_filters; filt++){
			for (int i = 0; i < feature_size.second; i++){ //8
				for (int j = 0; j < feature_size.first; j++){ //8
					feature_map[filt*feature_size.second*feature_size.first + feature_size.first*i + j] = bias[filt];
				}
			}
		}
		for(int filt=0; filt<num_filters; filt++){
			for(int ch=0; ch<num_features; ch++){
				for(int i=0; i<feature_size.second; i++){ //8
					for(int j=0; j<feature_size.first; j++){ //8
						for(int lt_i=0; lt_i<filter_size.second; lt_i++){ //i
							for(int lt_j=0; lt_j<filter_size.first; lt_j++){ //j
								if ((lt_i + i*stride - pad >= 0) || (lt_j + j*stride - pad >= 0) || (lt_i + i*stride - pad < image_size.second) || (lt_j + j*stride - pad < image_size.first)){
									feature_map[filt*feature_size.second*feature_size.first+feature_size.first*i + j] += filter[filt*num_features*filter_size.second*filter_size.first + ch*filter_size.second*filter_size.first + lt_i*filter_size.first + lt_j] * image[ch*image_size.second*image_size.first + (lt_i + i*stride - pad)*image_size.first + (lt_j + j*stride - pad)];
								}
							}
						}
					}
				}
			}
		}
}

void max_pool(w_t *image,																// input image
							pair<uint32_t, uint32_t> image_size,			// input image size
							uint32_t channel,													// number of features in input image = channel
							pair<uint32_t, uint32_t> max_pool_size,		// pooling size
							uint32_t stride,													// stride
							w_t *max_pool) {													// output image

		w_t temp_m = 0;

		pair<uint32_t, uint32_t> feature_size = make_pair((image_size.first-max_pool_size.first)/stride+1,(image_size.second-max_pool_size.second)/stride+1);

		// Fit to output image
		for(int ch=0; ch<channel; ch++){ //*12 *12 or *8 *8
			for(int i=0; i<feature_size.second; i++){
				for(int j=0; j<feature_size.first; j++){
					temp_m = image[ch*image_size.second*image_size.first + (i*stride)*image_size.first + j*stride];
					for(int lt_i= 0; lt_i<max_pool_size.second; lt_i++){ //i
						for(int lt_j=0; lt_j<max_pool_size.first; lt_j++){ //j
							if (temp_m < image[ch*image_size.second*image_size.first + (i*stride + lt_i)*image_size.first + j*stride + lt_j]){
								temp_m = image[ch*image_size.second*image_size.first + (i*stride + lt_i)*image_size.first + j*stride + lt_j];
							}
						}
					}
					max_pool[ch*feature_size.second*feature_size.first + feature_size.first*i+j]=temp_m;
				}

			}
		}

}

void ReLu(w_t *image,
				pair<uint32_t, uint32_t> image_size,
				uint32_t num_output,												
				w_t *output) {

		/*
		for(int ch=0; ch<num_output; ch++){
			int ch_all=ch*image_size.first*image_size.second;
			for(int i=0; i<image_size.second; i++){
				for(int j=0; j<image_size.first; j++){
					if(image[ch_all+image_size.first*i+j] > 0) output[ch_all+image_size.first*i+j] = image[ch_all+image_size.first*i+j];
					else output[ch_all+image_size.first*i+j]=0;
				}
			}
		}
		*/

		for (int i = 0; i < num_output*image_size.second*image_size.first; i++)
		{
			if (image[i] > 0)
			{
				output[i] = image[i];
			}
			else
			{
				output[i] = 0;
			}
		}
}

void TanH(w_t *image, 															// input image
				pair<uint32_t, uint32_t> image_size,				// input image size
				uint32_t num_output,												// number of output feature
				w_t *output){																// output
	///*
	for (int i = 0; i < image_size.first*image_size.second*num_output; i++)
	{
		output[i] = tanh(image[i]);
	}
	//*/
	//ReLu(output, image_size, num_output, image);

}

void ip(w_t *input, pair<uint32_t, uint32_t> input_size, //4 4			// input image
				uint32_t num_features,			// 5													// number of 1 input's features
				w_t *weight,																				// weights
				w_t *bias,																						// bias
				uint32_t num_output,																	// number of output neurons
				w_t *output){	
																									// output

	for (uint32_t i = 0; i < num_output; i++)
	{
		output[i] = 0;
		for (uint32_t j = 0; j < input_size.second*input_size.first*num_features; j++)
		{
			output[i] += weight[i*input_size.second*input_size.first*num_features + j]*input[j];
		}

	}
	for (uint32_t i = 0; i < num_output; i++){
		output[i] += bias[i];
	}



}



int accuracy(uint32_t iter,
							uint32_t *label,
							w_t *output){
		//FIXME
		int temp=0;
		int number = 0;
		for(int i=0; i<10; i++){
			if (output[i]>temp)
			{
				temp = output[i];
				number = i;
			}
		}
		cout << label[iter] << " " << number << endl;
		if(label[iter]==number) return 1;
		return 0;
}

