import statistics as stat
import subprocess as subp

factor = 15

def do_test(n: int, error: float):
    input_string = f"{n} {error}"
    results = []

    for i in range(factor):
        proc = subp.run(["./execs/filter", f"{i}"], input=input_string.encode(), capture_output=True)
        results.append(float(proc.stdout.decode()))

    print(f"SIZE: {n}")
    print(f"ERROR: {error}")
    print(f"STATS: mean = {stat.mean(results)}, median = {stat.median(results)}")
    print("<================================================>")

do_test(3000, 0.01)
do_test(3000, 0.05)
do_test(3000, 0.15)
do_test(3000, 0.30)
do_test(3000, 0.50)
