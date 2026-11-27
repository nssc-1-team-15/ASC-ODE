// this file integrates the massspring ode for a 
// stepper and number of steps N handed over the command line
// it returns ||y_N - y(T)||_2 for T = 2pi via the command line
#include <iostream>
#include <memory>
#include <cmath>
#include <numbers>
#include <string>

#include <nonlinfunc.hpp>
#include <timestepper.hpp>

using namespace ASC_ode;


class MassSpring : public NonlinearFunction
{
private:
    double mass;
    double stiffness;

public:
    MassSpring(double m, double k)
        : mass(m), stiffness(k) {}

    size_t dimX() const override { return 2; }
    size_t dimF() const override { return 2; }

    void evaluate(VectorView<double> x, VectorView<double> f) const override
    {
        f(0) = x(1);
        f(1) = -stiffness / mass * x(0);
    }

    void evaluateDeriv(VectorView<double> x, MatrixView<double> df) const override
    {
        df = 0.0;
        df(0, 1) = 1.0;
        df(1, 0) = -stiffness / mass;
    }
};


// Generic integration routine
double solve(TimeStepper& stepper, int N)
{
    const double T = 2.0 * std::numbers::pi;
    const double tau = T / N;

    Vector<> y = {1.0, 0.0};

    for (int i = 0; i < N; ++i)
    {
        stepper.doStep(tau, y);
    }

    // Exact solution at T = 2*pi: y(T) = (1, 0)
    const double error_x = y(0) - 1.0;
    const double error_v = y(1) - 0.0;

    return std::sqrt(error_x * error_x + error_v * error_v);
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <solver> <N>\n";
        return 1;
    }

    const std::string solver_name = argv[1];
    const int N = std::stoi(argv[2]);

    if (N <= 0)
    {
        std::cerr << "N must be positive.\n";
        return 1;
    }

    auto rhs = std::make_shared<MassSpring>(1.0, 1.0);

    double error;

    std::unique_ptr<TimeStepper> stepper;

    if (solver_name == "ExplicitEuler")
    {
        stepper = std::make_unique<ExplicitEuler>(rhs);
    }
    else if (solver_name == "ImprovedEuler")
    {
        stepper = std::make_unique<ImprovedEuler>(rhs);
    }
    else if (solver_name == "ImplicitEuler")
    {
        stepper = std::make_unique<ImplicitEuler>(rhs);
    }
    else if (solver_name == "CrankNicolson")
    {
        stepper = std::make_unique<CrankNicolson>(rhs);
    }
    else
    {
        std::cerr << "Unknown solver: " << solver_name << "     Try ExplicitEuler, ImprovedEuler, ImplicitEuler or CrankNicolson" << '\n';
        return 1;
    }

    error = solve(*stepper, N);

    std::cout << error << '\n';

    return 0;
}