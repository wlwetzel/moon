import numpy as np
import pandas as pd
import plotly.express as px
path = "/home/wyatt/Documents/g4_projects/moon/build/moon_dat_nt_MOON.csv"
dat = pd.read_csv(path,skiprows=6,header=None)
dat = dat[0]
print(dat)
hist,bins = np.histogram(dat.to_numpy(),bins=1000)
centers = (bins[1:] + bins[:-1]) / 2
df = pd.DataFrame({"flux":hist} , index=centers)
fig = px.scatter(df)
# fig.update_yaxes(type='log')
fig.update_layout(xaxis_title_text='Energy (MeV)',yaxis_title_text='Intensity (Arb.)',title_text='Generated Cs-137 Spectrum')

fig.show()
