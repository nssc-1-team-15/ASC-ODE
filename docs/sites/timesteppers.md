# Time-stepping methods

ASC-ODE advances an approximation \(y_n \approx y(t_n)\) for an ODE \(y'=f(y)\). A stepper receives the step size \(\tau\) and updates the state in place:

```cpp
stepper.doStep(tau, y);
```

All methods implement `TimeStepper`, which holds a shared pointer to the right-hand-side function. The function must provide the dimensions and evaluation (and, for Newton-based methods, its derivative) through the `NonlinearFunction` interface.

## Available methods

| Class | Update / method | Practical notes |
| --- | --- | --- |
| `ExplicitEuler` | \(y_{n+1}=y_n+\tau f(y_n)\) | One right-hand-side evaluation per step. Simple, first-order, and may require small steps for stability. |
| `ImprovedEuler` | Explicit midpoint Runge–Kutta: evaluate a slope at \(y_n\), estimate the midpoint, then use the midpoint slope for the full step. | Two right-hand-side evaluations per step; second-order accuracy. Despite the class name, this implementation is the explicit midpoint variant. |
| `ImplicitEuler` | Solve \(y_{n+1}-y_n-\tau f(y_{n+1})=0\) with Newton's method. | First-order and useful for problems where explicit Euler would need very small stable steps. Each step requires a converged nonlinear solve. |
| `ImplicitRungeKutta` | Solve for the coupled stage slopes from the supplied Runge–Kutta tableau \((A,b,c)\). | Supports implicit collocation methods, including the provided Gauss–Legendre coefficients and generated Gauss–Legendre or Radau IIA coefficients. Each step uses Newton's method. |

The implicit solvers use `NewtonSolver`. Implement `NonlinearFunction::evaluateDeriv` correctly for your right-hand side so the nonlinear solver can form Jacobians. For vector-valued systems, `dimX()` and `dimF()` must match the state and derivative dimensions.

## Choosing a method

- Start with `ExplicitEuler` for a small, simple problem when you want a baseline result.
- Use `ImprovedEuler` when you want a simple explicit method with better accuracy per step.
- Consider an implicit method for stiff systems or when explicit methods become unstable unless the step size is very small. Check that Newton iterations converge and compare results after reducing \(\tau\).
- `ImplicitRungeKutta` is useful when you need higher-order implicit integration. The provided demo uses a three-stage Gauss–Legendre method.

No method removes the need to choose a suitable step size. Compare solutions at successively smaller step sizes to assess accuracy for your problem.

## Usage pattern

The example below follows `demos/test_ode.cpp`. `MyRightHandSide` stands for a class derived from `NonlinearFunction` that implements the dimensions and evaluations for your system.

```cpp
using namespace ASC_ode;

auto rhs = std::make_shared<MyRightHandSide>(/* model parameters */);
Vector<> y = {/* initial state */};
double tau = 0.01;

ImprovedEuler stepper(rhs);
for (int n = 0; n < number_of_steps; ++n)
{
    stepper.doStep(tau, y);  // y is updated in place
}
```

To use implicit Runge–Kutta with the provided two-stage Gauss–Legendre tableau:

```cpp
ImplicitRungeKutta stepper(rhs, Gauss2a, Gauss2b, Gauss2c);
```

For a three-stage Gauss–Legendre method, the demo computes the tableau coefficients from the nodes:

```cpp
auto [a, b] = computeABfromC(Gauss3c);
ImplicitRungeKutta stepper(rhs, a, b, Gauss3c);
```

Include `timestepper.hpp` for Euler methods and `implicitRK.hpp` for the implicit Runge–Kutta class and its coefficient helpers. The full mass–spring example is in [`demos/test_ode.cpp`](https://github.com/nssc-1-team-15/ASC-ODE/blob/main/demos/test_ode.cpp).
