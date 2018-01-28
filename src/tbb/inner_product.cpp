//---------------------------------------------------------------------------//
// Copyright (c) 2013-2014 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://boostorg.github.com/compute for more information.
//---------------------------------------------------------------------------//

#include <algorithm>
#include <vector>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <valhalla.hpp>


#include <numeric>

// https://github.com/jjallaire/TBB/blob/master/inst/examples/parallel-inner-product.cpp
template<typename T>
struct InnerProductBody {

	// source vectors
	T * const x;
	T * const y;

	// sum that I have accumulated
	T product;

	// standard and splitting constructor  
	InnerProductBody(T * const x, T * const y)
		: x(x), y(y), product(0) {}
	InnerProductBody(InnerProductBody& body, tbb::split)
		: x(body.x), y(body.y), product(0) {}

	// accumulate just the element of the range I've been asked to
	void operator()(const tbb::blocked_range<size_t>& r) {
		product += std::inner_product(x + r.begin(),
			x + r.end(),
			y + r.begin(), T());
	}

	// join my inner product with another one
	void join(InnerProductBody& rhs) { product += rhs.product; }
};

template<class T>
T ParallelInnerProduct( T A[], T B[], size_t n ) {
    InnerProductBody<T> product(A, B);
    tbb::parallel_reduce( tbb::blocked_range<size_t>( 0, n),
                     product );
    return product.product;
}

namespace vll {
    template<typename T>
    void product(const int num, bool gen = true, int iter = 0)
    {
        static std::vector<T> A(1000);
        static std::vector<T> B(1000);
        

        if (gen) {
            A.resize(num);
            B.resize(num);            
            std::generate(A.begin(), A.end(), rand);
            std::generate(B.begin(), B.end(), rand);            
        }

        for (int i = 0; i < iter; i++) {
            ParallelInnerProduct<T>(&A[0], &B[0], A.size());
        }
    }

    template<typename T>
    void generate(const int num)
    {
        product<T>(num, true, 0);
    }

    template<typename T>
    void run(const int iter)
    {
        product<T>(-1, false, iter);
    }
}
int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
