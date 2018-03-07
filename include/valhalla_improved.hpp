#pragma once
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <stdexcept>

//#if __cplusplus <= 199711L
//    #error "C++11 support is required"
//#endif

#include <chrono>

#include <algorithm>
#include <functional>
#include <random>

namespace vll
{
    using std::chrono::system_clock;

	using d_microseconds = std::chrono::duration<double, std::micro>;

    template<typename T> void generate(const int iter);
    template<typename T> void reset();
    template<typename T> void run();    

    template<typename T>
    void bench_wrapper(const int start, const int end, const int step, const bool ismul)
    {
        const int warmup_iter = 10;
        const int bench_iter = 1000;
        std::cout << "Size\t\tTime(us)\t" << std::endl;
        for (int num = start; num <= end; ismul ? num *= step : num += step) {
            generate<T>(num);

            for (int i = 0; i < warmup_iter; i++)
            {
                reset<T>();
                run<T>(); // warmup 
            }

            system_clock::time_point begin;
            system_clock::time_point now;

            double u_sec(0);
			int i;
            for (i = 0; i < bench_iter; i++)
            {   
                reset<T>();
                begin = system_clock::now();
                run<T>();
                now = system_clock::now();
                u_sec += std::chrono::duration_cast<d_microseconds>(now - begin).count();

                if (u_sec > 10000000 && i > 20)
                    break;
            }
            u_sec /= (i + 1);
            
            // int repeat = 1;
            // while (u_sec < 10000 && repeat < 10 ) {
            //     double u_sec_per_repeat(0);
            //     for (int i = 0; i < bench_iter; i++)
            //     {
            //         reset<T>();
            //         begin = system_clock::now();
            //         run<T>();
            //         now = system_clock::now();

            //         u_sec_per_repeat += std::chrono::duration_cast<d_microseconds>(now - begin).count();
            //     }
            //     u_sec += u_sec_per_repeat / bench_iter;
            //     repeat++;
            // }

            std::cout << std::setw(10) << std::left << num << "\t" << u_sec << std::endl;
        }
    }


    template<typename T>
    void generate_raw_data(T* output, int num, int seed=1993)
    {

        // Specify the engine and distribution.
        std::mt19937 mersenne_engine(seed);
        std::uniform_real_distribution<T> dist(0, 5000);

        auto gen = std::bind(dist, mersenne_engine);
        generate(output, output+num, gen);
    }


    class WrapperBase
	{
	public:
		virtual void generate(const int num)
		{}

		virtual void reset()
		{}

		virtual  void run()
		{}
	};

    static void launch(const int argc,
                       const char **args)
    {
        if (argc < 3) {
            printf("Usage: %s <data_type> <start> [<end>  [<step>]]\n", args[0]);
            printf("data_type should be one of the following\n");
            printf("float\n");
            printf("double\n");
            throw std::runtime_error("Incorrect number of arguments");
        }

        try {
            const int start = atoi(args[2]);
            const int end  = argc > 3 ? atoi(args[3]) : start;
            bool ismul = false;
            int step = start;

            if (argc > 4) {
                ismul = args[4][0] == 'x' || args[4][0] == 'X';
                step = atoi(args[4] + 1);
            }

            if (args[1][0] == 'f') {
                bench_wrapper<float>(start, end, step, ismul);
            } else if (args[1][0] == 'd') {
                bench_wrapper<double>(start, end, step, ismul);
            } else {
                throw std::runtime_error("Data type not supported");
            }
        } catch(...) {
            std::cout << "Something went wrong" << std::endl;
        }
    }
}
