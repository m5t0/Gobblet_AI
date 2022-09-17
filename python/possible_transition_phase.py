import numpy as np
import pandas as pd
from pathlib import Path
import matplotlib as mpl
import matplotlib.pyplot as plt

if __name__ == "__main__":
    parent_parent = Path(__file__).resolve().parent.parent
    df = pd.read_csv(
        parent_parent.joinpath(Path("output/possible_transition_phase_p_4.csv")),
        header=0,
    )

    print(df["proportion"].describe())
    df["proportion"].hist(bins=100)
    plt.xlabel("proportion")
    plt.ylabel("quantity")
    plt.show()
