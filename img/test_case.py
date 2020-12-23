# -*- encoding: utf-8 -*-

import matplotlib.pyplot as plt

n = [[0, 0, 0, 0], [1, 1, -2, -1], [2, 2, -2, 1], [3, 2, -1, 2], [4, 1, -1, 1],
     [5, 1, 2, -1], [6, 1, 1, 1], [7, 2, 1, 2], [8, 3, 2, 2], [9, 3, 2, 3],
     [10, 4, 3, 3]]

c = [[0, 1], [0, 5], [0, 6], [0, 4], [2, 4], [3, 4], [7, 6], [7, 8], [7, 9],
     [9, 10]]

x1 = [x[2] for x in n]
y1 = [x[3] for x in n]

plt.scatter(x1, y1, c='k', marker='.')
for x in n:
    plt.text(x[2],
             x[3], (x[0], x[1], x[2], x[3]),
             ha='center',
             va='bottom',
             fontsize=11)

for x in c:
    plt.plot([n[x[0]][2], n[x[1]][2]], [n[x[0]][3], n[x[1]][3]],
             color='r',
             markerfacecolor='blue',
             marker='o')

# plt.legend()
# plt.show()
plt.savefig("1.png", format='png')
