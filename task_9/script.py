import statistics as stat
import subprocess as subp
from random import randrange

factor = 3

def do_test(matrix_size):
    input1 = b""
    for _ in range(matrix_size):
        for _ in range(matrix_size):
            input1 += str(randrange(0, 100)).encode()
            input1 += b" "

    input2 = b""
    for _ in range(matrix_size):
        for _ in range(matrix_size):
            input2 += str(randrange(0, 100)).encode()
            input2 += b" "

    input_string = f"{matrix_size} {input1} {input2}"

    method1 = []
    method3 = []

    for _ in range(factor):
        proc = subp.run(["./execs/method_1"], input=input_string.encode(), capture_output=True)
        method1.append(float(proc.stdout.decode()))

    for _ in range(factor):
        proc = subp.run(["./execs/method_3"], input=input_string.encode(), capture_output=True)
        method3.append(float(proc.stdout.decode()))

    print(f"Report on matrix with size: {matrix_size}:")
    print("\tTrivial algorithm:      {:.2f} <=> {:.2f} <=> {:.2f}".format(stat.mean(method1), stat.harmonic_mean(method1), stat.pstdev(method1)))
    print("\tStrassen algorithm:     {:.2f} <=> {:.2f} <=> {:.2f}".format(stat.mean(method3), stat.harmonic_mean(method3), stat.pstdev(method3)))
    print()

sizes = [128, 256, 512, 1024, 2048]

for i in sizes:
    do_test(i)
    do_test(i)

    print("<=====================================>")
    print()
