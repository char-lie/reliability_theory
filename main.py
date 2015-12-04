from math import exp, log
from numpy.random import exponential
from scipy.stats import poisson
from numpy import dot
from decimal import Decimal, getcontext

#getcontext().prec = 100

def F(x, rho):
    return 1 -  exp(-rho * x)

def get_f(rho):
    return lambda x: rho * Decimal(-rho * x).exp()

def G(x):
    return 1 - Decimal(-x).exp()

def get_t(rho, n):
    return map(Decimal, exponential(1/rho, n))

def get_p(t, a, n):
    accumulator = Decimal(0)
    result = []
    for i in xrange(n):
        accumulator += t[i] * a[i]
        result.append(1 - G(accumulator))
    return result

def get_s(p):
    cache = {}
    def s(m, k):
        result = None
        if (m, k) in cache:
            return cache[(m, k)]
        if k == 0 and m == 0:
            result = 1
        elif k == 0:
            result = (1 - p[m-1]) * s(m-1, 0)
        elif m == k:
            result = p[m-1] * s(m-1, k-1)
        else:
            result = p[m-1] * s(m-1, k-1) + (1 - p[m-1]) * s(m-1, k)
        cache[(m, k)] = result
        return result
    return s

def get_R(n, p, r):
    result = 0
    s = get_s(p)
    for k in xrange(r, n+1):
        r = s(n, k)
        result += s(n, k)
    return result

def get_Q(R, f, a, t):
    dividends = [A * f(A*T) for A, T in zip(a,t)]
    divisors = [f(T) for T in t]
    mul = lambda x, y: x*y
    di = reduce(mul, dividends, 1)
    d = reduce(mul, divisors, 1)
    return Decimal(R) * di / d

def get_n(epsilon, rho, a, t):
    w = poisson.isf(epsilon, 1.0/rho)
    accumulator = 0
    n = 0
    while accumulator <= w:
        accumulator += a[n] * t[n]
        n += 1
    return n

def get_a(phi, n):
    return [Decimal(phi(i)) for i in xrange(1, n+1)]

def get_phi(m, A):
    b = (exp(1) - exp(A)) / (m - 1)
    a = exp(A) - b
    return (lambda x: log(a + b * x))

if __name__ == '__main__':
    r = 100
    rho = Decimal(150)
    n = 400
    m = 300
    epsilon = 1E-4
    A = 0.9

    phi = get_phi(m, A)
    print 'Init a'
    a = get_a(phi, n)
    #print a
    print 'Init t'
    t = get_t(rho, n)
    #print t
    print 'Init p'
    p = get_p(t, a, n)
    #print p
    print 'Init f'
    f = get_f(rho)

    print 'Init R'
    R = get_R(n, p, r)
    print 'Init Q'
    Q = get_Q(R, f, a, t)
    print 'Result is', Q

