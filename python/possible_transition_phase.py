# -*- coding: utf-8 -*-

from itertools import count
from statistics import mean, stdev
import numpy as np
import pandas as pd
from pathlib import Path
import matplotlib as mpl
import matplotlib.pyplot as plt
import math


if __name__ == "__main__":
    parent_parent = Path(__file__).resolve().parent.parent
    data = pd.read_csv(
        parent_parent.joinpath(Path("output/possible_transition_phase_p_4.csv")),
        header=0,
    )

    df = data["proportion"]

    print(df.describe())

    df.hist(bins=100)
    plt.title("possible_transition_phase_proportion_4")
    plt.xlabel("proportion")
    plt.ylabel("piece pair quantity")
    # plt.yscale("log")
    plt.show()

    df = data["count"].tolist()
    df = list(map(int, df))
    mx = max(df)
    df = pd.DataFrame([d / mx for d in df],columns=["count"])

    print(df.describe())

    df.hist(bins=100)
    plt.title("possible_transition_phase_count_4")
    plt.xlabel("count proportion")
    plt.ylabel("piece pair quantity")
    plt.xlim((1e-20,10.))
    plt.xscale("log")
    plt.yscale("log")
    plt.show()
