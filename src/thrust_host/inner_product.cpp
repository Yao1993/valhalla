#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/inner_product.h>
#include <valhalla.hpp>

namespace vll {

    template<typename T>
    void inner_product(const int num, bool gen = true, int iter = 0)
    {
        static thrust::host_vector<T> h_A(1000);
        static thrust::host_vector<T> h_B(1000);
        

        if (gen) {        
			h_A.resize(num);
			h_B.resize(num);
            std::generate(h_A.begin(), h_A.end(), rand);
            std::generate(h_B.begin(), h_B.end(), rand);            
        }

        for (int i = 0; i < iter; i++) {
            T x = thrust::inner_product(h_A.begin(), h_A.end(), h_B.begin(), T());
        }

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
