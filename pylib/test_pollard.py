

import sys
import math
from dataclasses import dataclass

# highest prime < 2**40 =
# btw - pi(2**40) = 41203088796
# 2**40=1_099_511_627_776
p1 = 1_099_511_627_689

p2 = 1009



# This program calculates #E(F(prime)) for elliptic curve
#


# Weirstrass form elliptic curve y**2 = x**3 + ax + b
class EC(object):

    @dataclass
    class Point:
        x: int
        y: int

        def __eq__(self):
            return x == x and y == y

    def __init__(self, modulo, a, b):
        self._a = a
        self._b = b
        self._mod = modulo

    def pinfo(self) -> str:
        'Print out some info'

        e = 2 * int(math.sqrt(self._mod))
        print(
            f"Hasse's theorem on EC order N: {self._mod+1-e} <= N <= {self._mod+e}"
        )

    def check_qr(self, x: int) -> bool:
        '''Use Euler's criterion to check whether quadratic residue'''

        return pow(x, (self._mod-1) // 2, self._mod) != 1

    def find_point(self, start_x: int = 2) -> Point:
        'Find a point on EC'
        while True:
            rh = (start_x * (start_x * start_x + self._a) + self._b) % self._mod
            if self.check_qr(rh):
                if self._mod % 4 == 3:
                    y = pow(rh, (self._mod + 1) // 4, self._mod)
                else:

                return self.Point(start_x, y)



    def neg(self, p: Point):
        'Group negation'

        return self.Point(p.x, (-p.y) % self.mod)


    def _imstep(self, l: int, p: Point, q: Point):
        'Intermediate step used in add and double'

        r_x = (l*l - p.x-q.x) % self._mod
        return self.Point(r_x, (l * (p.x-r_x) - p.y) % self._mod)


    def identity(self):
        'Return identity element'

        return self.Point(-1, 0)


    def add(self, p: Point, q: Point):
        'Group add operation: p + q'
        # Note - the below is independent of a and b !

        if p.x == -1: # identity
            return q
        elif q.x == -1:
            return p
        elif p == q:
            return self.double(p)

        l = pow(q.x - p.x, -1, self._mod) * (q.y - p.y)
        return self._imstep(l % self._mod, p, q)


    def double(self, p):
        'Group doubling: p + p'

        if p.x == -1:
            return p

        l = pow(2*p.y, -1, self._mod) * (3*p.x*p.x + self._b)
        return self._imstep(l % self._mod, p, p)






def main(args):

    ec1 = EC(p2, 0 ,4)
    ec1.pinfo()


main(sys.argv[1:])