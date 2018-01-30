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
#include <tbb/parallel_sort.h>
#include <valhalla_improved.hpp>

namespace vll {

	template <typename T>
	class SortWrapper : public WrapperBase
	{
		std::vector<T> data0;
		std::vector<T> data;

	public:
		void generate(const int num) override
		{
			data0.resize(num);
			generate_raw_data(&data0[0], num);
		}

		void reset() override
		{
			data = data0;
		}

		void run() override
		{
			tbb::parallel_sort(data.begin(), data.end());
		}
	};


	WrapperBase* sort_warpper;

	template<typename T>
	void generate(const int num)
	{
		sort_warpper = new SortWrapper<T>();
		sort_warpper->generate(num);
	}

	template<typename T>
	void reset()
	{

		sort_warpper->reset();
	}

	template<typename T>
	void run()
	{
		sort_warpper->run();
	}
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
