# -*- coding: utf-8 -*-

from itertools import count
from statistics import mean, stdev
import numpy as np
import pandas as pd
from pathlib import Path
import matplotlib as mpl
import matplotlib.pyplot as plt
import math


def main(s,f, color=None, title=None):
    parent_parent = Path(__file__).resolve().parent.parent
    data = pd.read_csv(
        parent_parent.joinpath(Path(f"output/possible_transition_phase_p_{s}.csv")),
        header=0,
    )

    df = data["count"].values
    df = list(map(int, df))
    mx = sum(df)
    df2 = pd.Series(np.array([(i / mx) for i in df], dtype=np.float64), name="mutual_proportion")

    data = pd.merge(data, df2, left_index=True, right_index=True, how="left")
    data.to_csv(
        parent_parent.joinpath(
            Path(f"output/possible_mutual_transition_phase_{s}.csv")
        ),
        index=False,
    )

    mn = min(df2)
    mx = max(df2)
    cnt = sum(df)
    mean = sum([df2[i] * df[i] / cnt for i in range(len(df2))])
    std = math.sqrt(
        sum([(df2[i] - mean) ** 2 * df[i] / cnt for i in range(len(df2))])
    )
    crr = pd.Series(np.array([i/mx for i in df2],dtype=np.float64)).corr(pd.Series(np.array(df),dtype=np.float64))

    print(f"possible_mutual_transition_phase_p_{s}")
    print()
    print("count", str(cnt).rjust(42))
    print("mean", str(mean).rjust(43))
    print("std", str(std).rjust(44))
    print("min", str(mn).rjust(44))
    print("max", str(mx).rjust(44))
    print("corr", str(crr).rjust(43))

    if title is not None:
        plt.title(title)
    else:
        plt.title(s)

    plt.xlabel("互いに到達可能な局面数/総局面数", fontname="MS Gothic")
    plt.ylabel("局面数", fontname="MS Gothic")
    
    if not f:
        plt.yscale("log")
    plt.hist(df2, bins=50, weights=df, color=color)

    if not f:
        plt.savefig(parent_parent.joinpath(Path(f"output/possible_mutual_transition_phase_log_{s}.png")))
    else:
        plt.savefig(parent_parent.joinpath(Path(f"output/possible_mutual_transition_phase_{s}.png")))
    plt.show()


if __name__ == "__main__":
    main("3",True, color=None, title="3x3")
    main("3",False, color=None, title="3x3")
    main("4",True, color="orange", title="4x4")
    main("4",False, color="orange", title="4x4")
    main("5_3_2",True)
    main("5_3_2",False)
