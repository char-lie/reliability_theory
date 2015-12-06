from numpy.random import exponential
from math import exp
from decimal import Decimal, getcontext

def G_Decimal(x):
    return 1 - (-x).exp()

def G(x):
    return 1 - exp(-x)

def get_t_Decimal(rho, n):
    return map(Decimal, exponential(1.0/rho, n))

def get_t(rho, n):
    return exponential(1.0/rho, n)

def get_p(t, a, n):
    accumulator = 0
    result = []
    for i in xrange(n):
        accumulator += t[i] * a[i]
        result.append(1 - G(accumulator))
    return result

