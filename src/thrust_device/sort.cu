#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <valhalla_improved.hpp>

namespace vll {

    template <typename T>
    class SortWrapper : public WrapperBase
    {
        thrust::device_vector<T> d_data0;
        thrust::device_vector<T> d_data;

	public:
        void generate(const int num) override
        {
            thrust::host_vector<T> h_data(num);
			generate_raw_data(&h_data[0], num);
            d_data0 = h_data; 
            cudaDeviceSynchronize();
        }

        void reset() override
        {         
            d_data = d_data0;
            cudaDeviceSynchronize();
        }

        void run() override
        {
            thrust::sort(d_data.begin(), d_data.end());
            cudaDeviceSynchronize();            
        }
    };


	WrapperBase* sort_warpper;

    template<typename T>
    void generate(const int num)
    {
		sort_warpper = new SortWrapper<T>();
        try {
           sort_warpper->generate(num);
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    template<typename T>
    void reset()
    {
        try {
            sort_warpper->reset();
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    template<typename T>
    void run()
    {
        try {
            sort_warpper->run();
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
