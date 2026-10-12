class TwoPower(object):
    def __init__(self,bin_len=32):
        self.biggest = (2**bin_len)-1
    def __call__(self,x):
        return x&((self.biggest^x)+1)


class Fenwick(object):
    def __init__(self,N):
        """N is the maximim value that will need to be accounted for"""
        power = 2
        exp = 1
        while power < N:
            power *= 2
            exp += 1
        self.N = power
        self.two = TwoPower(exp+1)
        self.data = [0 for i in range(power)]

    def query(self,ind):
        """Returns the cummulative sum of all indices from 1 to ind"""
        res = 0
        while ind > 0:
            res += self.data[ind-1]
            ind -= self.two(ind)
        return res


    def add(self,ind,val):
        """Add val at the index ind"""
        assert ind > 0, "This implementation requieres ind > 0, ind = "+str(ind\
)

        if ind > self.N:
            # Need to extend the tree                                           
            power = self.N
            total = self.query(self.N)
            while power < ind:
                self.data += [0 for i in range(power-1)]+[total]
                power *= 2
                self.two.biggest *= 2
                self.two.biggest += 1
            self.N = power

        while ind <= self.N:
            self.data[ind-1] += val
            ind += self.two(ind)