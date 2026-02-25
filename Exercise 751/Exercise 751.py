import numpy as np
import time
from random import randint


def lst_to_decimal(lst):
    str_lst = "".join([str(e) for e in lst])
    d = len(str_lst) - 1
    res = int(str_lst)
    return res, d


def problem312(precision):
    cont = 0
    while cont < 1:
        coeffs = [1, 0] # b1 = theta
        lower_bound_theta, power = 2, 0
        a = 2
        theta = [2]
        cont_loop = True
        while cont_loop and len(theta) < precision:
            coeffs[0], coeffs[1] = a * coeffs[0], a * coeffs[1] - a*(a-1)
            lower_bound_a = coeffs[0] * lower_bound_theta + coeffs[1] * 10**power 
            upper_bound_a = coeffs[0] * (lower_bound_theta+1) + coeffs[1] * 10**power
            l, h = np.floor(lower_bound_a / 10**power), np.ceil(upper_bound_a / 10**power)-1
            if l > h or h < 0:
                cont_loop = False
            else:
                a = randint(l, h)
                theta.append(a)
                lower_bound_theta, power = lst_to_decimal(theta)

        cont += 1

        if len(theta) >= precision:
            print("Sol", lower_bound_theta)


if __name__ == "__main__":
    start = time.process_time()
    print(problem312(precision=17))
    print("Time taken : ", time.process_time()-start)