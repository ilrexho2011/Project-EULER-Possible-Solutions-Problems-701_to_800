class Node: 
    def __init__(self,val): 
        self.val = val 
        self.left = None
        self.right = None 
  
        # denotes number of times (frequency) 
        # an element has occurred. 
        self.elecount = 1
  
        # denotes the number of nodes on left 
        # side of the node encountered so far.  
        self.lcount = 0  
  
class Tree: 
    def __init__(self,root): 
        self.root = root 
    def insert(self,node): 
  
        curr = self.root 
        cnt = 0
        while curr!=None: 
            prev = curr 
            if node.val>curr.val: 
                cnt += (curr.elecount+curr.lcount) 
                curr=curr.right 
            elif node.val<curr.val: 
                curr.lcount+=1
                curr=curr.left 
            else: 
                prev=curr 
                prev.elecount+=1
                break
        if prev.val>node.val: 
            prev.left = node 
        elif prev.val<node.val: 
            prev.right = node 
        else: 
            return cnt+prev.lcount 
        return cnt 
  
def constructArray(arr,n): 
    t = Tree(Node(arr[-1])) 
    ans = [0] 
    for i in range(n-2,-1,-1): 
        ans.append(t.insert(Node(arr[i]))) 
    return reversed(ans) 
  
def doublePerm(perm):
    l = len(perm)
    resPermLo = []
    resPermHi = []
    for k in xrange(0,l-1):
        resPermLo += [2 * perm[k] - 1]
        resPermHi += [2 * perm[k+1]]
    return resPermLo + [2,2*l - 1] + resPermHi

mod = 10**9 + 7
exp = 25
n = 2**exp

factMod = [1]
for j in xrange(1,n+1):
    factMod += [(j * factMod[j-1]) % mod]

perm = [1,3,2,4]

for j in xrange(2,exp):
    perm = doublePerm(perm)

print "perm calculated..."
lehmerTree = constructArray(perm,n)
print "tree calculated..."
c = n - 1
res = 1
for e in lehmerTree:
    if c % 100 == 0: print c
    res +=  e * factMod[c]
    c -= 1

print res % mod