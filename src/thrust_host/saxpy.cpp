#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/functional.h>
#include <valhalla.hpp>

namespace vll {

    template<typename T>
    struct saxpy_functor : public thrust::binary_function<T, T, T>
    {
        const T a;

        saxpy_functor(T _a) : a(_a) {}

            __host__ __device__
            T operator()(const T& x, const T& y) const {
                return a * x + y;
            }
    };

    template<typename T>
    void saxpy(const int num, bool gen = true, int iter = 0)
    {
        static thrust::host_vector<T> x;
        static thrust::host_vector<T> y;
        static thrust::host_vector<T> res;
        static T a = 3.5;

        if (gen) {
            x.resize(num);
            std::generate(x.begin(), x.end(), rand);

            y.resize(num);            
            std::generate(y.begin(), y.end(), rand);

            res = thrust::host_vector<T>(num);
        }

        for (int i = 0; i < iter; i++) {
            thrust::transform(x.begin(), x.end(),
                              y.begin(), res.begin(),
                              saxpy_functor<T>(a));
        }
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            saxpy<T>(num, true, 0);
        } catch(thrust::system::system_error &err) {
            std::cout <<  err.what() << std::endl;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            saxpy<T>(-1, false, iter);
        } catch(thrust::system::system_error &err) {
            std::cout <<  err.what() << std::endl;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
