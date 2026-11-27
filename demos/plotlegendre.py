import numpy as np
import matplotlib.pyplot as plt


data = np.loadtxt("output_legendre.txt")

x = data[:, 0]
P = data[:, 1:7]
dP = data[:, 7:13]

for k in range(6):
    plt.plot(x, data[:, k+1], label=f"P{k}")

plt.xlabel("x")
plt.ylabel("P(x)")
plt.title("Legendre Polynomials")
plt.legend()
plt.grid()
plt.savefig("legendre_polynomials.png", dpi=150, bbox_inches="tight")
plt.close()

for k in range(6):
    plt.plot(x, data[:, k+7], label=f"P{k}'")

plt.xlabel("x")
plt.ylabel("P'(x)")
plt.title("Derivatives of Legendre Polynomials")
plt.legend()
plt.grid()
plt.savefig("legendre_derivatives.png", dpi=150, bbox_inches="tight")
plt.close()