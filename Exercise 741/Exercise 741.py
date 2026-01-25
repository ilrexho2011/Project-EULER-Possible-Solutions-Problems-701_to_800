import numpy as np
from time import time

### Modulo function
def mod1(n):
    return(int(n % 1000000007))
    
    
########### Get the reflection number of 45 degree######
def f(n): #number of reflection 45
    if n < 0 or n==1:
        return(0)
    if n == 0 or n == 2:
        return(1)
    result = k(n)+g(n)
    return(result)
def g(n):# no black in corner
    if n<=1:
        return(0)
    result = (n-1)*(n-2)//2*(2*f(n-3)+2*k(n-2)+2*g(n-2)+(n-3)*f(n-4))
    return(result)
def k(n):    #one black in corner
    if n==1 or n<0:
        return(0)
    if n==0:
        return(1)
    result = (n-1)*(f(n-2)+k(n-1))
    return(result)

def cal_ref4(n):
    if n<10:
        return(mod1(f(n)))    
    REF4 = np.zeros(n+1, dtype=np.int64)
    REF4_g = np.zeros(n+1, dtype=np.int64)
    REF4_k = np.zeros(n+1, dtype=np.int64)
    for i in range(0,10):  
        REF4[i] = mod1(f(i))
        REF4_g[i] = mod1(g(i))
        REF4_k[i] = mod1(k(i))
    for i in range(10,n+1):

        REF4_g[i] = mod1(mod1((i-1)*(i-2)//2)*mod1((mod1(2*REF4[i-3])+mod1(2*REF4_k[i-2])+mod1(2*REF4_g[i-2])+mod1((i-3)*REF4[i-4]))))
        REF4_k[i] = mod1((i-1)*mod1(REF4[i-2]+REF4_k[i-1]))
        REF4[i] = mod1(REF4_k[i] + REF4_g[i])
    return(REF4[-1])

####################reflection vertical or horizontal###############################
def cal_ref1(n):  
    if n<2 or n % 2 ==1:
        return(0)
    if n==2:
        return(1)
    REF1 = np.zeros((n+2)//2, dtype=np.int64)
    REF1[1]=1
    for i in range(2,n//2+1):
        REF1[i] = mod1(mod1(REF1[i-1])*mod1(i*(2*i-1))) 
    return(REF1[-1])


#######################################################


########### Get the number of rotation_90 ###############
def f3(n):
    if n<0: 
        return(0)
    if n==0:
        return(1)
    if n==1:
        return(0)
    if n==2:
        return(1)
    result = f3(n-2)+g3(n)
    return(result)
def g3(n):
    if n<=0:
        return(0)
    result = (n-2)//2*f3(n-4)+(n-2)*(n-4)//2*(2*f3(n-6)+(n-6)*f3(n-8)+2*g3(n-4))
    return(result)

def cal_rot90(n):
    if n<10:
        return(mod1(f3(n)))
    Rotation_90 = np.zeros(n+1, dtype=np.int64)
    R90_g3 = np.zeros(n+1, dtype=np.int64)
    for i in range(0,10,2):  
        Rotation_90[i] = mod1(f3(i))
        R90_g3[i] = mod1(g3(i))
    for i in range(10,n+1,2):
        R90_g3[i] = mod1(mod1((i-2)//2*Rotation_90[i-4]) +mod1( mod1((i-2)*(i-4)//2)*mod1(2*Rotation_90[i-6]+(i-6)*Rotation_90[i-8]+2*R90_g3[i-4])))
        Rotation_90[i] =  mod1(Rotation_90[i-2] + R90_g3[i])
    return(Rotation_90[-1])
##########################################################
    
#######################Get the number of Rotation 180##################
def f2(n): #number of rotation 180
    if n<0: 
        return(0)
    if n==0:
        return(1)
    if n==1:
        return(0)
    if n==2:
        return(1)
    if n % 2 == 0:
        result = f2(n-2)+k2(n)+2*g2(n)
    else:
        result = ((n-1)//2)**2*(2*f2(n-3)+(n-3)**2*k4(n-3))
    return(result)
def k4(n):    #for odd number rotation 180 case2
    if n < 2:
        return(0)
    else:
        return(2*f2(n-2)+(n-2)**2*k4(n-2))
def g2(n): #one pair black in the corner
    if n< 0:
        return(0)
    if n==0:
        return(1)
    result = (n-2)**2*(2*f2(n-4)+g2(n-2))
    return(result)
def k2(n):  #no black on the corner
    if n<0:
        return(0)
    if n==0:
        return(1)
    result = (n-2)*f2(n-2)- ((n-2)//2)**2*f2(n-4)+((n-2)*(n-4)//2)**2*(8*f2(n-6)+8*g2(n-4)+4*k2(n-4)+(n-6)**2*f2(n-8)+4*(n-6)*f2(n-6))
    return(result)  

def cal_rot180(n):
    if n<10:
        return(mod1(f2(n)))
    Rotation_180 = np.zeros(n+1, dtype=np.int64)
    R180_g2 = np.zeros(n+1, dtype=np.int64)    
    R180_k2 = np.zeros(n+1, dtype=np.int64) 
    R180_k4 = np.zeros(n+1, dtype=np.int64) 
    for i in range(0,10):  
        Rotation_180[i] = mod1(f2(i))
        R180_g2[i] = mod1(g2(i))
        R180_k4[i] = mod1(k4(i))
        R180_k2[i] = mod1(k2(i))
    for i in range(10,n+1):
        if i % 2  == 0:
            R180_k2[i] = mod1( mod1((i-2)*Rotation_180[i-2]) - mod1(((i-2)//2)**2)*mod1(Rotation_180[i-4]) +mod1(((i-2)*(i-4)//2)**2)*mod1(8*Rotation_180[i-6]+8*R180_g2[i-4]+4*R180_k2[i-4]+mod1((i-6)**2)*Rotation_180[i-8]+4*(i-6)*Rotation_180[i-6])) 
            R180_g2[i] = mod1(mod1((i-2)**2)*mod1(2*Rotation_180[i-4]+R180_g2[i-2]))
            R180_k4[i] = mod1(mod1(2*Rotation_180[i-2])  + mod1(mod1((i-2)**2)*mod1(R180_k4[i-2])))
            Rotation_180[i] = mod1(Rotation_180[i-2]+R180_k2[i]+2*R180_g2[i])  
        else:
            Rotation_180[i] = mod1(mod1(((i-1)//2)**2)*mod1(2*Rotation_180[i-3]+mod1((i-3)**2)*mod1(R180_k4[i-3])))
    return(Rotation_180[-1])
    
#################################################################################    
    
    
################################################################################    
#################### identity number #################
def f0(n): #similar idea 
    if n<0: 
        return(0)
    if n==0:
        return(1)
    if n==1:
        return(0)
    if n==2:
        return(1)
    result = k0(n)+g0(n)
    return(result)

def g0(n):
    if n<=0:
        return(0)
    result = (n-1)**2*(f0(n-2)+g0(n-1))
    return(result)
def k0(n):
    if n<=0:
        return(0)
    result = ((n-1)*(n-2)//2)**2*(2*f0(n-3)+4*g0(n-2)+(n-3)**2*f0(n-4)+2*(n-3)**2*g0(n-3)+4*k0(n-2))
    return(result)

def cal_rot0(n):
    if n<10:
        return(mod1(f0(n)))
    Rotation_0 = np.zeros(n+1, dtype=np.int64)
    R0_g0 = np.zeros(n+1, dtype=np.int64)    
    R0_k0 = np.zeros(n+1, dtype=np.int64)  
    for i in range(0,10):  
        Rotation_0[i] = mod1(f0(i))
        R0_g0[i] = mod1(g0(i))
        R0_k0[i] = mod1(k0(i))
    for i in range(10,n+1):
        R0_g0[i] = mod1(mod1((i-1)**2)*mod1((Rotation_0[i-2]+R0_g0[i-1]))) 
        R0_k0[i] = mod1( mod1(((i-1)*(i-2)//2)**2)*mod1(2*Rotation_0[i-3]+4*R0_g0[i-2]+mod1((i-3)**2)*mod1(Rotation_0[i-4])+2*mod1((i-3)**2)*mod1(R0_g0[i-3])+4*R0_k0[i-2]))
        Rotation_0[i] = mod1(R0_g0[i]+R0_k0[i]) 
    return(Rotation_0[-1])


#################Final Answer#######################
n0 =7**7
n1 = 8**8
print("Mission Started")   
id7 = cal_rot0(n0)
print("step 1 completed", id7)
id8 = cal_rot0(n1)
print("step 2 completed",id8)
r7_90 = 0
print("step 3 completed",r7_90)
r8_90 = cal_rot90(n1)
print("step 4 completed",r8_90)
r7_270 = r7_90
print("step 5 completed",r7_270)
r8_270 = r8_90
print("step 6 completed",r8_270)
r7_180 = cal_rot180(n0)
print("step 7 completed",r7_180)
r8_180 = cal_rot180(n1)
print("step 8 completed",r8_180)
f7_1 = cal_ref1(n0)
print("step 9 completed",f7_1)
f7_2 = f7_1
print("step 10 completed",f7_2)
f7_3 = cal_ref4(n0)
print("step 11 completed",f7_3)
f7_4 = f7_3
print("step 12 completed",f7_4)
f8_1 = cal_ref1(n1)
print("step 13 completed",f8_1)
f8_2 = f8_1
print("step 14 completed",f8_2)
f8_3 = cal_ref4(n1)
print("step 15 completed",f8_3)
f8_4 = f8_3
print("step 16 completed",f8_4)
print("Calculating final_answer:...")
ans = mod1(id7+r7_90+r7_180+r7_270+f7_1+f7_2+f7_3+f7_4+id8+r8_90+r8_180+r8_270+f8_1+f8_2+f8_3+f8_4)
print('result1=',mod1(ans*mod1(125000001)))