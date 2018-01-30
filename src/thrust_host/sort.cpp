#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <valhalla_improved.hpp>

namespace vll {


    template <typename T>
    class SortWrapper : public WrapperBase
    {
        thrust::host_vector<T> h_data0;
        thrust::host_vector<T> h_data;

	public:
        void generate(const int num) override
        {
            h_data0.resize(num);
			generate_raw_data(&h_data0[0], num);   
        }

        void reset() override
        {
            h_data = h_data0;
        }

        void run() override
        {
            thrust::sort(h_data.begin(), h_data.end());
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
