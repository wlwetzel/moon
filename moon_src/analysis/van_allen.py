import os
os.environ["CDF_LIB"] = "~/CDF/lib"
from spacepy import pycdf
import numpy as np
import pandas as pd
import plotly.express as px
from scipy.optimize import curve_fit
import plotly.graph_objects as go


def gen_csv():
    cdf = pycdf.CDF("/home/wyatt/Documents/g4_projects/moon/moon_src/analysis/SW_ECTREPT_RBSPA_2017264_L2.cdf")
    keys = ["FEIO_2","FEIO_4","FEIO_9"]
    dat = pd.DataFrame(data={"2MeV":cdf["FEIO_2"][...],"5MeV":cdf["FEIO_4"][...],"10MeV":cdf["FEIO_9"][...]},index=cdf["Epoch"][...])
    points = dat.iloc[400]
    counts = [points["2MeV"],points["5MeV"],points["10MeV"]]
    ener = [2,5,10]
    power = lambda e,a,k: a*(e)**(-k)
    exp = lambda e,a,k: a*np.exp(-k*e)
    xdat = np.logspace(0,1.2,50)

    popt,cov = curve_fit(power,ener,counts)
    spec = power(xdat,popt[0],popt[1])
    frame = pd.DataFrame(data={"Energies":xdat,"Counts":spec})
    frame.to_csv("/home/wyatt/Documents/g4_projects/moon/moon_src/analysis/van_hist.csv",
                 index=None,sep=" ",float_format='%.2f')

#analyze data
def whatever():
    path = "/home/wyatt/Documents/g4_projects/moon/build/moon_dat_1cm_elec_nt_MOON.csv"
    dat = pd.read_csv(path,skiprows=7,header=None,usecols=[0,1],names=['ene','dep'])
    total_dep = dat["dep"].sum()
    num_particles = len(dat.index)
    fig = px.histogram(dat['ene'])
    fig.update_layout(xaxis_range=[0,14],xaxis_title_text="Electron Energy (MeV)",
                      title_text=f"Electron Energy Spectrum,Tot. Dep. Energy: {total_dep:.2f} MeV,Tot. Num. Electrons: {num_particles}",
                      yaxis_title_text="Num Particles Fired")
    fig.show()
#trying to define a response function
path = "/home/wyatt/Documents/g4_projects/moon/build/moon_dat_1cm_elec_nt_MOON.csv"
dat = pd.read_csv(path,skiprows=7,header=None,usecols=[0,1],names=['ene','dep'])
energy_response_df = dat[dat['dep']!=0].groupby('ene').mean()

hits_df = dat[dat['dep']!=0].groupby('ene').size()
totals_df = dat.groupby('ene').size()
geo_response_df = hits_df/totals_df
print(geo_response_df)
