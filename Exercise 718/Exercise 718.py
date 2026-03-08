from gmpy2 import mpz, divm, divexact, mpfr

def euler718(k):
  d1,d2,d3 = mpz(17)**k,mpz(19)**k,mpz(23)**k
  a1,a2,a3 = min_rel_greenberg(d1,d2,d3)
  c, s = fel(d1,d2,d3,a1,a2,a3)
  s_d = d1+d2+d3
  return s + divexact(s_d*(s_d-1),2) + c*s_d
      
def fel(d1,d2,d3,a1,a2,a3):
  ad1,ad2,ad3 = a1*d1, a2*d2, a3*d3
  pi_d = d1*d2*d3
  pi_a = a1*a2*a3
  s_d = d1+d2+d3
  s_ad = ad1+ad2+ad3
  g0 = 1 - s_d - pi_a + s_ad
  g1 = s_ad*(s_ad-3*s_d-2*pi_a) + s_d*(s_d+3*pi_a) + (d1*d2 + d1*d3 + d2*d3) + pi_d - 1
  g1 += ad1*ad1 + ad2*ad2 + ad3*ad3
  return divexact(g0,2), divexact(g1,12)
  
def min_rel_greenberg(a,b,c):
  bf, cf = mpfr(b), mpfr(c)*1.0000001
  s, g = divm(c,b,a), a
  z, e = mpz(1), mpz(0)
  while True:
    q, g = divmod(g, s)
    e += q*z
    q, s = divmod(s, g)
    z += q*e
    if bf*mpfr(s) < cf*mpfr(z):
      diff = c*z - b*s
      if diff > 0:
        break
  den = c*e+b*g
  gamma = diff//den
  s += (gamma+1)*g
  z -= gamma*e
  return divexact(den,a), s, z