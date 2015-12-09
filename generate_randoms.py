from numpy.random import exponential
from math import exp
from decimal import Decimal, getcontext
from random import randint

def G_Decimal(x):
    return 1 - (-x).exp()

def G(x):
    return 1 - exp(-x)

DEC_MAX = 10**getcontext().prec
def get_t_Decimal(rho, n):
    return [-(Decimal(randint(1, DEC_MAX))/DEC_MAX).ln()/rho for i in range(n)]

def get_t(rho, n):
    return exponential(1.0/rho, n)

def get_p(t, a, n):
    accumulator = 0
    result = []
    for i in xrange(n):
        accumulator += t[i] * a[i]
        result.append(1 - G(accumulator))
    return result

def get_p_Decimal(t, a, n):
    accumulator = Decimal(0)
    result = []
    for i in xrange(n):
        accumulator += t[i] * a[i]
        result.append(Decimal(1) - G_Decimal(accumulator))
    return result

