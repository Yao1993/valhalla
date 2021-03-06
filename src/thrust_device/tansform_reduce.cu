#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform_reduce.h>
#include <valhalla.hpp>

namespace vll {

    template<class T>
	struct threshold_square : thrust::unary_function<T, T>
	{
		T a;
		threshold_square(T a_) : a(a_) {}

		__host__ __device__ T operator() (T x) const
		{
			return (x - a) * (x - a);
		}

	};

    template<typename T>
    void reduce_transform(const int num, bool gen = true, int iter = 0)
    {
        static thrust::device_vector<T> d_vec;

        if (gen) {
            thrust::host_vector<T> h_vec(num);
            std::generate(h_vec.begin(), h_vec.end(), rand);
            d_vec = h_vec;
            cudaDeviceSynchronize();
        }

        for (int i = 0; i < iter; i++) {
            T x = thrust::transform_reduce(d_vec.begin(), d_vec.end(), threshold_square<T>(T(1)), T(0), thrust::plus<float>());
        }

        cudaDeviceSynchronize();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            reduce_transform<T>(num, true, 0);
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            reduce_transform<T>(-1, false, iter);
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
