// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p1132r7.html

#include <iostream>
#include <memory>


// Some C API

struct res_t {};

int res_open(res_t** res);
void res_free(res_t* res);

// Deleter wrapper

struct res_deleter
{
    void operator()(res_t* res) const noexcept
    {
        res_free(res);
    }
};

using unique_res = std::unique_ptr<res_t, res_deleter>;

// inout_ptr

template<typename Smart>
class inout_ptr
{
public:
    using pointer = typename Smart::pointer;

    explicit inout_ptr(Smart& smart)
        : _smart{smart}
        , _ptr{smart.get()}
    {
    }

    ~inout_ptr() noexcept
    {
        _smart.reset(_ptr);
    }

    operator pointer*()
    {
        return &_ptr;
    }

private:
    Smart& _smart;
    pointer _ptr;
};


int main()
{
    unique_res res;

    if (res_open(inout_ptr(res)) == -1) {
        std::cerr << "error\n";
    }
    std::clog << "res: " << res.get() << '\n';
}


int res_open(res_t** res)
{
    *res = new res_t;
    return 0;
}


void res_free(res_t* res)
{
    delete res;
}
