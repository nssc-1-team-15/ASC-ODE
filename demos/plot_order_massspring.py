import numpy as np
import matplotlib.pyplot as plt
import subprocess
import csv

# Integrators
solvers = [
    "ExplicitEuler",
    "ImprovedEuler",
    "ImplicitEuler",
    "CrankNicolson"
]

# Number of timesteps
Ns = 2**np.arange(4, 13)

# Path to executable
executable = "../build/solve_massspring"

# Store errors
errors = {solver: [] for solver in solvers}

# Run simulations
for solver in solvers:
    for N in Ns:
        result = subprocess.run(
            [executable, solver, str(int(N))],
            capture_output=True,
            text=True,
            check=True
        )

        error = float(result.stdout.strip())
        errors[solver].append(error)

        print(f"{solver:20s} N={N:5d} error={error:.6e}")


# Convert errors to numpy arrays
for solver in solvers:
    errors[solver] = np.array(errors[solver])


# Estimate convergence orders
orders = {}

for solver in solvers:
    # Fit log(error) = slope * log(N) + intercept
    slope, intercept = np.polyfit(np.log(Ns), np.log(errors[solver]), 1)

    # Since error ~ N^(-p), the convergence order is -slope
    orders[solver] = -slope

    print(f"{solver:20s} estimated order: {orders[solver]:.4f}")


# Save results to CSV
with open("massspring_convergence_errors.csv", "w", newline="") as file:
    writer = csv.writer(file)

    writer.writerow(["N"] + solvers)

    for i, N in enumerate(Ns):
        writer.writerow(
            [N] + [errors[solver][i] for solver in solvers]
        )


# Plot convergence
plt.figure(figsize=(8, 6))

for solver in solvers:
    plt.loglog(
        Ns,
        errors[solver],
        marker="o",
        label=f"{solver} (observed order {orders[solver]:.2f})"
    )


# Reference slopes of order 1 and 2
N_ref = Ns.astype(float)

# Anchor reference lines at the first ExplicitEuler data point
E_ref = errors["ExplicitEuler"][0]

plt.loglog(
    N_ref,
    E_ref * (N_ref / N_ref[0])**(-1),
    "--",
    color="black",
    alpha=0.6,
    label="Reference: order 1"
)

plt.loglog(
    N_ref,
    E_ref * (N_ref / N_ref[0])**(-2),
    ":",
    color="black",
    alpha=0.6,
    label="Reference: order 2"
)


plt.xlabel("Number of timesteps $N$")
plt.ylabel(r"Error $\|y_N-y(T)\|_2$")
plt.title("Convergence of Integrators: Mass-Spring System")
plt.legend()
plt.grid(True, which="both")
plt.tight_layout()

plt.savefig("massspring_convergence.png", dpi=150)
plt.show()