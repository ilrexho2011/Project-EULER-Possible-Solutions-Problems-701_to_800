def f(n):
    """Ways of summing to n"""
    if n == 0:
        return 1
    return sum(f(n-x) for x in range(1, n+1))


def g(n):
    """Ways of summing to n including a 2"""
    if n == 0:
        return 0
    t = 0
    for x in range(1, n+1):
        if x == 2:
            t += f(n-x)
        else:
            t += g(n-x)
    return t


def euler710(n):
    t = 0
    if n % 2 == 0:
        # n is even so can have even in the middle
        for m in range(2, n+1, 2):
            x = (n-m)//2
            if m == 2:
                t += f(x)
            else:
                t += g(x)
        # or pair in the middle
        for m in range(1, n//2+1, 1):
            x = (n-2*m)//2
            if m == 2:
                t += f(x)
            else:
                t += g(x)
    else:
        # n is odd so need odd number in the middle
        for m in range(1, n+1, 2):
            x = (n-m)//2
            t += g(x)
    return t
