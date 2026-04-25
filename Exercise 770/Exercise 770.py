d_h=lambda d:2/(1+2*d)
dsuiv=lambda d,i:d*(2*i+1)/(2*i+2)
def d(n):
    d=0.5;
    for i in range(n):d=dsuiv(d,i)
    return(d)
#4/Pi=1.2732

def ih(I,borne):
    i=I;di= d(i)
    while d_h(di)<borne:
       di=dsuiv(di,i);i+=1
    return(i,d_h(di))

from time import time
st=time()
print(ih(127311222,1.9999),time()-st,' s.')
#(127311223, 1.999900000000335) 23.439014196395874