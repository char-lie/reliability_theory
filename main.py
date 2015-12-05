from math import exp, log
from numpy.random import exponential
from scipy.stats import poisson
from numpy import dot
from decimal import Decimal
from joblib import Parallel, delayed

def F(x, rho):
    return 1 -  exp(-rho * x)

def get_f(rho):
    return lambda x: rho * exp(-rho * x)

def G(x):
    return 1 - exp(-x)

def get_t(rho, n):
    return exponential(1./rho, n)

def get_p(t, a, n):
    accumulator = 0
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

def get_Q(R, f, a, t, D=float):
    d = [A * f(A*T) / f(T) for A, T in zip(a,t)]
    return D(R) * reduce(lambda x,y: D(x)*D(y), d, D(1))

def get_n(epsilon, rho, a, t):
    w = poisson.isf(epsilon, 1.0/rho)
    accumulator = 0
    n = 0
    while accumulator <= w:
        accumulator += a[n] * t[n]
        n += 1
    return n

def get_a(phi, n):
    return [phi(i) for i in xrange(1, n+1)]

def get_phi(m, A):
    b = (exp(1) - exp(A)) / (m - 1)
    a = exp(A) - b
    return (lambda x: log(a + b * x))

def calculate_Q(r, rho):
    k = 1
    c = Decimal(-rho).exp()
    result = 0
    for i in xrange(1, r):
        k *= i
        result += c * (rho**i)/k
    return 1 - c - result

def run_task(rho, r, n, a, f, D=float):
    t = get_t(rho, n)
    p = get_p(t, a, n)

    R = get_R(n, p, r)
    Q = get_Q(R, f, a, t, D)
    return Q

def init_a(n, m, A):
    phi = get_phi(m, A)
    a = get_a(phi, n)
    return a

def stdev(real, experimental):
    return sum((Decimal(e) - real)**2 for e in experimental)/len(experimental)

if __name__ == '__main__':
    iterations = 100
    r = 100
    rho = 150
    n = 300
    m = 2*r
    epsilon = 1E-4
    D = Decimal
    A = 1 - D('1E-10')
    realQ = D(calculate_Q(r, rho))

    Qs = []
    a = init_a(n, m, A)
    f = get_f(rho)
    def get_task(i):
        print 'Running task', i
        return run_task(rho, r, n, a, f, D)
    #Qs = Parallel(n_jobs=4)(delayed(get_task)(i) for i in range(iterations))

    for i in range(iterations):
        print 'Running task', i
        Qs.append(run_task(rho, r, n, a, f, D))
    experimentalQ = sum(Qs)/len(Qs)
    print 'Real is %f, experimental is %f, difference is %f%%, dev is %f'%(float(realQ), float(experimentalQ),
           100.0*abs(float((realQ-experimentalQ)/realQ)), float(stdev(realQ, Qs)))

