from math import exp, log
from decimal import Decimal, getcontext

def get_phi_Decimal(m, A):
    b = (Decimal(1).exp() - A.exp()) / (m - 1)
    a = A.exp() - b
    return (lambda x: (a + b * x).ln())

def get_phi(m, A):
    b = (exp(1) - exp(A)) / (m - 1)
    a = (exp(A) * m - exp(1)) / (m - 1) # exp(A) - b
    return (lambda x: log(a + b * x))

def get_a(phi, n):
    return [min(phi(i), 1.0) for i in xrange(1, n+1)]

def init_a_Decimal(n, m, A):
    phi = get_phi_Decimal(m, A)
    a = get_a(phi, n)
    return a

def init_a(n, m, A):
    phi = get_phi(m, A)
    a = get_a(phi, n)
    return a

