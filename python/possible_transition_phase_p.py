# -*- coding: utf-8 -*-

import numpy as np
import pandas as pd
from pathlib import Path
import matplotlib as mpl
import matplotlib.pyplot as plt
import math

# https://oku.edu.mie-u.ac.jp/~okumura/python/hist-median.html 引用
def hquantile(hist, q=0.5):
    n = sum(hist)
    t = n * q
    i = 0
    s = hist[i]
    while s < t:
        i += 1
        s += hist[i]
    if s == t and hist[i + 1] == 0:
        j = i + 2
        while hist[j] == 0:
            j += 1
        return (i + j) / 2
    return i + (t - s) / hist[i] + 0.5


def f(p, hist):
    if p - math.floor(p) < 0.5:
        return hist[math.floor(p)]
    elif p - math.floor(p) > 0.5:
        return hist[math.ceil(p)]
    else:
        return (hist[math.floor(p)] + hist[math.ceil(p)]) / 2

def main(s):
    parent_parent = Path(__file__).resolve().parent.parent
    df = pd.read_csv(
        parent_parent.joinpath(Path(f"output/possible_transition_phase_p_{s}.csv")),
        header=0,
    )

    df = df[["proportion", "count"]].values
    df = df[np.argsort(df[:, 0])]
    df = np.transpose(df)
    df = df.tolist()
    df[1] = list(map(int, df[1]))
    mn = min(df[0])
    mx = max(df[0])
    cnt = sum(df[1])
    mean = sum([df[0][i] * df[1][i] / cnt for i in range(len(df[0]))])
    std = math.sqrt(
        sum([(df[0][i] - mean) ** 2 * df[1][i] / cnt for i in range(len(df[0]))])
    )

    #p1 = hquantile(df[1], 0.25) - 1
    #p2 = hquantile(df[1], 0.5) - 1
    #p3 = hquantile(df[1], 0.75) - 1

    print("count", str(cnt).rjust(32))
    print("mean", str(mean).rjust(33))
    print("std", str(std).rjust(34))
    print("min", str(mn).rjust(34))
    #print("25%", str(f(p1,df[0])).rjust(34))
    #print("50%", str(f(p2,df[0])).rjust(34))
    #print("75%", str(f(p3,df[0])).rjust(34))
    print("max", str(mx).rjust(34))

    plt.hist(df[0], bins=50, weights=df[1])
    #plt.title("possible_transition_phase_p_4")
    plt.xlabel("到達可能な局面数/総局面数", fontname="MS Gothic")
    plt.ylabel("開始局面数", fontname="Meiryo")
    plt.yscale("log")
    plt.savefig(parent_parent.joinpath(Path(f"output/possible_transition_phase_p_log_{s}.png")))
    plt.show()

    #print("corr:",pd.Series(df[0]).corr(pd.Series(df[1])))
    plt.scatter(df[0], df[1])
    #plt.title("possible_transition_phase_p_plot_4")
    plt.xlabel("到達可能な局面数/総局面数", fontname="MS Gothic")
    plt.ylabel("開始局面数", fontname="Meiryo")
    plt.yscale("log")
    plt.savefig(parent_parent.joinpath(Path(f"output/possible_transition_phase_p_plot_log_{s}.png")))
    plt.show()

if __name__ == "__main__":
    main("5_3_2")