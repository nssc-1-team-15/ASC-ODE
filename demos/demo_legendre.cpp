#include <iostream>
#include <vector>
#include <fstream>
#include <autodiff.hpp>

using namespace ASC_ode;

template <typename T>
void LegendrePolynomials(int n, T x, std::vector<T>& P) {
    if (n < 0) {
        P.clear();
        return;
    }
    P.resize(n + 1);
    P[0] = T(1);
    if (n == 0) return;
    P[1] = x;
    for (int k = 2; k <= n; ++k) {
        P[k] = ((T(2 * k - 1) * x * P[k - 1]) - T(k - 1) * P[k - 2]) / T(k);
    }
}

int main(){
    int n = 5;
    int N = 1000;

    std::ofstream results;
    results.open("output_legendre.txt");
    results << "#x\tP0\tP1\tP2\tP3\tP4\tP5\tP0'\tP1'\tP2'\tP3'\tP4'\tP5'\n";

    for (int i = 0; i <= N; i++){
        double x = -1. + 2. * i / N;
        results << x << "\t";

        AutoDiff<1> adx = Variable<0>(x);
        std::vector<AutoDiff<1>> P;

        LegendrePolynomials(n, adx, P);

        for (int k = 0; k <= n; k++){
            results << P[k].value() << "\t";
        }

        for (int k = 0; k <= n; k++){
            results << P[k].deriv()[0] << "\t";
        }

        results << "\n";
    }

    results.close();

    return 0;
}