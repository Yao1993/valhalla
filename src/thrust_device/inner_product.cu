#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/inner_product.h>
#include <valhalla.hpp>

namespace vll {

    template<typename T>
    void inner_product(const int num, bool gen = true, int iter = 0)
    {
        static thrust::device_vector<T> d_A;
        static thrust::device_vector<T> d_B;
        

        if (gen) {
            thrust::host_vector<T> h_A(num);
            thrust::host_vector<T> h_B(num);            
            std::generate(h_A.begin(), h_A.end(), rand);
            std::generate(h_B.begin(), h_B.end(), rand);            
            d_A = h_A;
            d_B = h_B;
            cudaDeviceSynchronize();
        }

        for (int i = 0; i < iter; i++) {
            T x = thrust::inner_product(d_A.begin(), d_A.end(), d_B.begin(), T());
        }

        cudaDeviceSynchronize();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            inner_product<T>(num, true, 0);
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            inner_product<T>(-1, false, iter);
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
