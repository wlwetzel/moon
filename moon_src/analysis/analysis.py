import numpy as np
import pandas as pd
import plotly.express as px
import plotly.graph_objs as go
path = "/home/wyatt/Documents/g4_projects/moon/build/moon_dat_nt_MOON.csv"
dat = pd.read_csv(path,skiprows=6,header=None,names=['ene','dep'])
dat_no_zero = dat[dat['dep']>0]
dat_no_ten = dat[dat['dep']>.050]
# print('average')
# print(dat_no_zero.mean())
# print('average no ten')
# print(dat_no_ten.mean())
# print("percent of hits")
# print( len(dat[dat['dep']!=0]['dep']) / len(dat['dep']) )
# nbins=70
# fig = px.histogram(dat_no_ten['dep'],range_x = [0,.7] , nbins=nbins)
# fig.update_layout(xaxis_title_text='Deposited Energy (MeV)',
#                   title_text = 'Deposited Energy in 10keV Bins, <50keV Removed')
# fig.show(
x=dat_no_ten['ene'].to_numpy()
y=dat_no_ten['dep'].to_numpy()
# df = pd.DataFrame(data = y,index=x)
#
# fig = px.scatter(x=x,y=y,marginal_x='histogram',
#                    marginal_y='histogram')
# fig.update_layout(xaxis_title_text='Incident Energy (MeV)',
#                 yaxis_title_text='Deposited Energy (MeV)',
#                 title_text = 'Energy Deposited in Silicon')
# fig.update_traces(marker=dict(size=2),selector=dict(type='scatter'))
# fig.show()
print(len(y))
hist,edges = np.histogram(y,bins=np.linspace(0,.7,70))
dens = hist / np.diff(edges)
dE = np.diff(edges)
print(np.sum(dens * np.diff(edges)))
abscissa = (edges[:-1] + edges[1:]) /2
#weighted integral int(density * E dE)
print(np.sum(dens * abscissa *dE ))
