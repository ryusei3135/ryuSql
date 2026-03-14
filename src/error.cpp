#include "errs.hpp"


void output_err_log(Errors err_kind) {
    switch (err_kind) {
        default:
            std::cout << "err" << std::endl;
    }
}