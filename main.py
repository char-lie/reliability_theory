from cProfile import Profile
from pstats import Stats
from math import exp, log
from numpy.random import exponential, seed
from scipy.stats import poisson
from numpy import dot
from decimal import Decimal, getcontext
from joblib import Parallel, delayed
from operator import mul

from generate_n import generate_n
from generate_a import init_a
from generate_randoms import get_t, get_p

from sys import setrecursionlimit, stdout

setrecursionlimit(5000)
getcontext().prec = 400

def F(x, rho):
    return 1 -  exp(-rho * x)

def get_f_Decimal(rho):
    return lambda x: rho * (-rho * x).exp()

def get_f(rho):
    return lambda x: rho * exp(-rho * x)

def get_s(p):
    cache = {}
    inv_p = [1-probability for probability in p]
    p_muls = []
    inv_p_muls = []
    p_muls = [reduce(mul, p[:i+1], 1) for i in range(len(p))]
    inv_p_muls = [reduce(mul, inv_p[:i+1], 1) for i in range(len(inv_p))]
    def s(m, k):
        result = None
        if (m, k) in cache:
            return cache[(m, k)]
        if k == 0 and m == 0:
            result = 1
        elif k == 0:
            result = inv_p_muls[m-1]
        elif m == k:
            result = p_muls[m-1]
        else:
            result = s(m-1, k-1) * p[m-1] + (1 - p[m-1]) * s(m-1, k)
            cache[(m, k)] = result
        return result
    return s

def get_R_Decimal(n, p, r):
    result = Decimal(0)
    s = get_s(p)
    for k in xrange(r, n+1):
        result += s(n, k)
    return result

def get_R(n, p, r):
    result = 0
    s = get_s(p)
    for k in xrange(r, n+1):
        result += s(n, k)
    return result

def get_Q_Decimal(R, f, a, t, D=float):
    divisor = reduce(mul, [f(T) for A, T in zip(a,t)], Decimal(1))
    divident = reduce(mul, [A * f(A*T) for A, T in zip(a,t)], Decimal(1))
    return (D(R) * divident) / divisor

def get_Q(R, f, a, t):
    return reduce(mul, [A * f(A*T) / f(T) for A, T in zip(a,t)], R)

def calculate_Q(r, rho):
    k = 1
    c = Decimal(-rho).exp()
    result = 0
    for i in xrange(1, r):
        k *= i
        result += (rho**i)/k
    return 1 - c * (1 + result)

def run_task_Decimal(rho, r, n, a, f):
    t = get_t_Decimal(rho, n)
    p = get_p(t, a, n)

    R = get_R_Decimal(n, p, r)
    print 'R', R
    Q = get_Q_Decimal(R, f, a, t)
    return Q

#profile = Profile()
def run_task(rho, r, n, a, f, t):
    p = get_p(t, a, n)

    R = get_R(n, p, r)
    Q = get_Q(R, f, a, t)
    return Q

def stdev(real, experimental):
    avg = sum(experimental)/len(experimental)
    return sum((e - avg)**2 for e in experimental)/(sum(experimental))

def run_tasks(rho, r, A, m, iterations=1, parallel=False, D=float, get_task=None):
    realQ = D(calculate_Q(r, rho))

    Qs = []
    f = get_f(rho)
    if parallel:
        Qs = Parallel(n_jobs=4)(delayed(get_task)(A, m, i) for i in range(iterations))
    else:
        for i in range(iterations):
            #profile.enable()
            n, a, t = generate_n(epsilon, rho, m, A)
            print 'Running task', i
            Qs.append(run_task(rho, r, n, a, f, t))
            #profile.disable()
    experimentalQ = sum(Qs)/len(Qs)
    print 'Q', experimentalQ
    print 'Real is %.15f, experimental is %.15f, difference is %.15f%%, dev is %.15f'%(float(realQ), float(experimentalQ),
           100.0*abs(float((realQ-experimentalQ)/realQ)), float(stdev(realQ, Qs)))
    return experimentalQ, stdev(realQ, Qs), A, m, r, Qs


if __name__ == '__main__':
    iterations = 128
    r = 180
    rho = 100
    m = 2*r
    #epsilon = 1E-5
    epsilon = 1E-4
    #A = 0.9
    # r = 120, A = .93, m = 200
    # r = 140
    # Difference is 7.873009%, result is 0.000084, deviation is 0.000038 (a*=0.750000, m=280, r=140)
    # Difference is 1.466868%, result is 0.000090, deviation is 0.000112 (a*=0.800000, m=280, r=140)
    # Difference is 14.040876%, result is 0.000079, deviation is 0.000335 (a*=0.850000, m=280, r=140)
    # r = 160
    # Difference is 5.016462846447344%, result is 0.000000021535700, deviation is 0.000000007443211 (a*=0.620000, m=320, r=160)
    # Difference is 5.586814696371359%, result is 0.000000021652662, deviation is 0.000000005401941 (a*=0.630000, m=320, r=160)
    # Difference is 1.521000899725865%, result is 0.000000020818887, deviation is 0.000000011034985 (a*=0.640000, m=320, r=160)
    # r = 180
    # Difference is 0.790672701320627%, result is 0.000000000000407, deviation is 0.000000000000380 (a*=0.500000, m=360, r=180)
    # Difference is 1.419735970524463%, result is 0.000000000000405, deviation is 0.000000000000148 (a*=0.550000, m=360, r=180)
    # Difference is 15.120959474991036%, result is 0.000000000000348, deviation is 0.000000000000274 (a*=0.600000, m=360, r=180)

    realQ = float(calculate_Q(r, rho))
    f = get_f(rho)
    def get_task(A, m, i):
        seed()
        n, a, t = generate_n(epsilon, rho, m, A)
        Q = run_task(rho, r, n, a, f, t)
        print 'Compeleted task %04d for a*=%f, m=%d with n=%04d: %.15f (%.15f, %f%%)'%(i, A, m, n, Q, realQ, 100*abs(realQ-Q)/realQ)
        return Q

    results = []
    for i in xrange(3):
        results.append(run_tasks(rho, r, .5+.05*i, m, iterations, True, get_task=get_task))
    Qs = results[0][5]
    realQ = float(calculate_Q(r, rho))
    #devs = [stdev(realQ, Qs[:i]) for i in xrange(1,len(Qs)+1)]
    #print devs
    for result in results:
        print 'Difference is %.15f%%, result is %.15f, deviation is %.15f (a*=%f, m=%d, r=%d)'%(100*abs(realQ-result[0])/realQ, result[0], result[1], result[2], result[3], result[4])
    #Stats(profile).sort_stats('tottime').print_stats()

