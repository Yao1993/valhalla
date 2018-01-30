#include <arrayfire.h>
#include <af/traits.hpp>
#include <valhalla_improved.hpp>
#include <vector>

namespace vll {

    using namespace af;

    array a0;
    array a;

    template<typename T>
    void generate(const int num)
    {
        try {
            std::vector<T> h_a0(num);
            generate_raw_data(&h_a0[0], num);            
            a0 = array(num, &h_a0[0]);
			a0.eval();
            af::sync();
        } catch(const af::exception &ex) {
            std::cout << ex.what() << std::endl;
            throw;
        }
    }

    template<typename T>
    void run()
    {
        try {
            sort(a);
            a.eval();
            af::sync();            
        } catch(const af::exception &ex) {
            std::cout << ex.what() << std::endl;
            throw;
        }
    }

    template<typename T>
    void reset()
    {
        try {
            a = a0;
			a.eval();
            af::sync();
        } catch(const af::exception &ex) {
            std::cout << ex.what() << std::endl;
            throw;
        }
    }

}

int main(int argc, const char **args)
{
	vll::launch(argc, args);
}
