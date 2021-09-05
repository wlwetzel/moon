import numpy as np
import pandas as pd
import plotly.express as px
import plotly.graph_objs as go
master_path = "/home/wyatt/Documents/g4_projects/moon/build/moon_dat_"
# parts = ["1cm_elec","2cm_elec","3cm_elec","1cm_prot","2cm_prot","3cm_prot"]
parts = ["4mm_prot","8mm_prot"]
paths = [master_path+ item + "_nt_MOON.csv" for item in parts]

id = 0
for path in paths:
    dat = pd.read_csv(path,skiprows=7,header=None,usecols=[0,1],names=['ene','dep'])
    mean_dat = dat.groupby("ene").mean()
    mean_dat["std"] = dat.groupby("ene").std()
    fig = px.scatter(x=dat["ene"],y=dat["dep"])
    fig.update_layout(xaxis_title_text="Incident Energy (MeV)",
                      yaxis_title_text="Deposited Energy (MeV)",
                      title_text=parts[id])
    fig.write_image("/home/wyatt/Documents/g4_projects/moon/moon_src/analysis/"+parts[id]+".png")
    # fig = px.scatter(mean_dat,error_y="std")
    fig.show()
    id+=1
