# -*- coding: utf-8 -*-

from itertools import count
from statistics import mean, stdev
import numpy as np
import pandas as pd
from pathlib import Path
import matplotlib as mpl
import matplotlib.pyplot as plt
import math


def main(s):
    parent_parent = Path(__file__).resolve().parent.parent
    data = pd.read_csv(
        parent_parent.joinpath(Path(f"output/possible_transition_phase_p_{s}.csv")),
        header=0,
    )

    df = data["proportion"]

    print(f"possible_transition_phase_p_{s} proportion")
    print()
    print(df.describe())

    df = data["count"].values
    df = list(map(int, df))
    mx = max(df)
    df = pd.Series(np.array([i / mx for i in df], dtype=np.float64), name="count")

    print(f"possible_transition_phase_p_{s} count/max(count)")
    print()
    print(df.describe())


if __name__ == "__main__":
    main("4")
    main("5_3_2")
