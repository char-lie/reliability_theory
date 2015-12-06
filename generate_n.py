from scipy.stats import expon
from decimal import Decimal, getcontext
from generate_a import init_a
from generate_randoms import get_t
from numpy import dot

def get_n(epsilon, rho, a, t):
    w = expon.ppf(epsilon, rho)
    accumulator = 0
    n = 0
    while accumulator <= w:
        accumulator += a[n] * t[n]
        n += 1
        if n == len(a):
            return None
    return n

def generate_n(epsilon, rho, m, A, initial=None):
    n = m if initial is None else initial
    w = expon.ppf(1-epsilon)
    t = list(get_t(rho, n))
    a = init_a(n, m, A)
    accumulator = dot(a, t)
    while True:
        # print n, accumulator, w
        if accumulator > w:
            return (n, a, t)
        n += 1
        t.append(get_t(rho, 1)[0])
        a.append(1.0)
        accumulator += t[-1]
    return None

