import numpy as np
import matplotlib.pyplot as plt

# Simulation results
T, magnetization = np.loadtxt("data/magnetization.csv", delimiter=",", skiprows=1, unpack=True)

# Onsager's exact solution for an infinite grid
Tc = 2 / np.log(1 + np.sqrt(2))
T_exact = np.linspace(1.4, 3.6, 500)
m_exact = np.maximum(1 - np.sinh(2 / T_exact) ** -4, 0) ** (1 / 8)

# Plot: exact curve first
plt.plot(T_exact, m_exact, color="black", label="Onsager exact solution (infinite grid)")
plt.plot(T, magnetization, "bo", label="Monte Carlo simulation, L = 16")
plt.axvline(Tc, linestyle="--", color="gray", label=r"$T_c$")

plt.title(r"2D Ising model, $L = 16$")
plt.xlabel(r"Temperature $T$ [$J/k_B$]")
plt.ylabel(r"$\langle |m| \rangle$")
plt.legend()

plt.savefig("analysis/magnetization.png", dpi=150)
plt.show()