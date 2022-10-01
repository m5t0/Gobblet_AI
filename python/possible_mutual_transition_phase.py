# -*- coding: utf-8 -*-

from itertools import count
from statistics import mean, stdev
import numpy as np
import pandas as pd
from pathlib import Path
import matplotlib as mpl
import matplotlib.pyplot as plt
import math


def main(s,f):
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

    print(f"possible_transition_phase_p_{s} count/sum(count)")
    print()
    print(df2.describe())

    plt.xlabel("互いに到達可能な局面数/総局面数", fontname="MS Gothic")
    plt.ylabel("局面数", fontname="MS Gothic")

    if not f:
        plt.yscale("log")
    plt.hist(df2, bins=50, weights=df)

    if not f:
        plt.savefig(parent_parent.joinpath(Path(f"output/possible_mutual_transition_phase_log_{s}.png")))
    else:
        plt.savefig(parent_parent.joinpath(Path(f"output/possible_mutual_transition_phase_{s}.png")))
    plt.show()


if __name__ == "__main__":
    main("3",True)
    main("3",False)
    main("4",True)
    main("4",False)
    main("5_3_2",True)
    main("5_3_2",False)
