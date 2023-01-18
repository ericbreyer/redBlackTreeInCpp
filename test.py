import subprocess
import numpy as np
import matplotlib.pyplot as plt
import timeit
from alive_progress import alive_bar

bar = None


def profile(structure, bar):
    def closure(itr):
        bar()
        return timeit.timeit(f"subprocess.run(['time', './build/main.exe', str({structure}), str({itr})], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)", number=1, setup='import subprocess')

    return closure


x = np.array([x for x in range(100, 20000, 100)])
# lhtRes = None
# rbTreeRes = None
with alive_bar(len(x)+1, title="Linked Hash Table", bar="filling") as bar:
    run = np.vectorize(profile(1, bar))
    lhtRes = run(x)
    plt.plot(x, lhtRes, label="linked hash table")
with alive_bar(len(x)+1, title="RBTree", bar="filling") as bar:
    run = np.vectorize(profile(2, bar))
    rbTreeRes = run(x)
    plt.plot(x, rbTreeRes, label="red black tree")
with alive_bar(len(x)+1, title="linked bloom", bar="filling") as bar:
    run = np.vectorize(profile(4, bar))
    rbTreeRes = run(x)
    plt.plot(x, rbTreeRes, label="linked bloom")
with alive_bar(len(x)+1, title="rb bloom", bar="filling") as bar:
    run = np.vectorize(profile(5, bar))
    rbTreeRes = run(x)
    plt.plot(x, rbTreeRes, label="rb bloom")

plt.legend()

plt.show()

# print(float(string[28: len(string) - 25]))
