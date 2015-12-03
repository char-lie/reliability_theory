from math import exp, log
from numpy.random import exponential
from scipy.stats import poisson
from numpy import dot

def F(x, rho):
    return 1 - exp(-rho * x)

def G(x):
    return 1 - exp(-x)

def get_time(rho, n):
    return exponential(rho, n)

def get_probabilities(t, a):
    return 1 - G(dot(t,a))

def s(m, k, p):
    if m == 0 and k == 0:
        return 1
    elif k == 0:
        return (1 - p[m-1]) * s(m-1, 0)
    elif m == k:
        return p[m-1] * s(m-1, k-1)
    else:
        return p[m-1] * s(m-1, k-1) + (1 - p[m-1]) * s(m-1, k)

def get_R(n, k, p, r):
    return sum(s(m, k, p) for m in xrange(r, n+1))

def get_Q(R, f, a, t):
    dividends = [A * f(A*T) for A, T in zip(a,t)]
    divisors = [f(T) for T in t]
    return R * reduce(lambda x, y: x*y, dividends, 1) / reduce(lambda x, y: x*y, divisors, 1)

def get_n(epsilon, rho, a, t):
    w = poisson.isf(epsilon, 1.0/rho)
    accumulator = 0
    n = 0
    while accumulator <= w:
        accumulator += a[n] * t[n]
        n += 1
    return n

def get_a(phi, a, b, n):
    return [phi(i) for i in xrange(1, n+1)]

def phi(x, m, A):
    a = exp(A) - b
    b = (exp(1) - exp(A)) / (m - 1)
    return log(a + b * x)

if __name__ == '__main__':
    r = 100
    rho = 100
    n = 10
    epsilon = 1E-4

