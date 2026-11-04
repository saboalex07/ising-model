import numpy as np
import matplotlib.pyplot as plt
from scipy.special import ellipk

# Simulation results, written by ising.cpp (columns: T, m, e, c, chi)
T, magnetization, energy, c, chi = np.loadtxt("data/magnetization.csv", delimiter=",", skiprows=1, unpack=True)

# Onsager's exact solution for an infinite grid
Tc = 2 / np.log(1 + np.sqrt(2))
T_exact = np.linspace(1.4, 3.6, 500)
beta = 1 / T_exact

m_exact = np.maximum(1 - np.sinh(2 * beta) ** -4, 0) ** (1 / 8)

kappa = 2 * np.sinh(2 * beta) / np.cosh(2 * beta) ** 2
e_exact = -1 / np.tanh(2 * beta) * (1 + (2 / np.pi) * (2 * np.tanh(2 * beta) ** 2 - 1) * ellipk(kappa ** 2))

c_exact = np.gradient(e_exact, T_exact)

# Figure 1: magnetization
plt.plot(T_exact, m_exact, color="black", label="Onsager exact solution (infinite grid)")
plt.plot(T, magnetization, "bo", label="Monte Carlo simulation, L = 16")
plt.axvline(Tc, linestyle="--", color="gray", label=r"$T_c$")
plt.title(r"2D Ising model, $L = 16$")
plt.xlabel(r"Temperature $T$ [$J/k_B$]")
plt.ylabel(r"$\langle |m| \rangle$")
plt.legend()
plt.savefig("analysis/magnetization.png", dpi=150)

# Figure 2: energy per spin
plt.figure()
plt.plot(T_exact, e_exact, color="black", label="Onsager exact solution (infinite grid)")
plt.plot(T, energy, "ro", label="Monte Carlo simulation, L = 16")
plt.axvline(Tc, linestyle="--", color="gray", label=r"$T_c$")
plt.title(r"2D Ising model, $L = 16$")
plt.xlabel(r"Temperature $T$ [$J/k_B$]")
plt.ylabel(r"Energy per spin $e$ [$J$]")
plt.legend()
plt.savefig("analysis/energy.png", dpi=150)

# Figure 3: heat capacity per spin
plt.figure()
plt.plot(T, c, "ro", label=r"Heat capacity per spin $c$")
plt.plot(T_exact, c_exact, color="black", label="Onsager exact solution (infinite grid)")
plt.axvline(Tc, linestyle="--", color="gray", label=r"$T_c$")
plt.title(r"2D Ising model, $L = 16$")
plt.xlabel(r"Temperature $T$ [$J/k_B$]")
plt.ylabel(r"Heat capacity per spin $c$")
plt.legend()
plt.savefig("analysis/heat_capacity.png", dpi=150)

#Figure 4: susceptibility per spin
plt.figure()
plt.plot(T, chi, "ro", label=r"Susceptibility per spin $\chi$")
plt.axvline(Tc, linestyle="--", color="gray", label=r"$T_c$")
plt.title(r"2D Ising model, $L = 16$")
plt.xlabel(r"Temperature $T$ [$J/k_B$]")
plt.ylabel(r"Susceptibility per spin $\chi$")
plt.legend()
plt.savefig("analysis/heat_capacity.png", dpi=150)

plt.show()