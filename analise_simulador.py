import numpy as np
import matplotlib.pyplot as plt
import os

nfiles =11
nprocessos="10" 
fout = "sjf_n"+nprocessos+".png" 

#----------------------------------------------------------
espera_media = np.ones(nfiles-1)
tf_media = np.ones(nfiles-1)
tr_media = np.ones(nfiles-1)
dt_media = np.ones(nfiles-1)

l=0
for i in range(1,nfiles):
    fname = "out_"+str(i)+"_n"+nprocessos+".txt"
    print fname

 
    with open(fname) as f:
        content = f.readlines()

    content = [x.split() for x in content]

    sz = len(content) -1
    tf = np.ones(sz)
    tr = np.ones(sz)
    t0 = np.ones(sz)
    dt = np.ones(sz)
    espera = np.ones(sz)

    k=0
    for j in content[:-1]:
        tf[k] = j[1]
        tr[k] = j[2]
        dt[k] = j[3]
        t0[k] = tf[k] - tr[k]
        espera[k] = tr[k] - dt[k]
        #print t0[k], tf[k], tr[k] , espera[k]
        k = k+1
    # estats
    espera_media[l] = np.nanmean(espera)
    tr_media[l] = np.nanmean(tr)
    tf_media[l] = np.nanmean(tf)
    dt_media[l] = np.nanmean(dt)
    l=l+1

# boxplot
plt.figure(figsize=(16,9),dpi=90)
ax = plt.subplot(221)
plt.title(' Espera media (#'+nprocessos + " processos)") 
plt.boxplot(espera_media,showmeans=True,meanline=True)
plt.ylabel('Tempo (clock)',fontsize=14)
plt.grid(True)
ax.set_xticklabels(['30 execucoes '],fontsize=14)
#ax.set_ylim([6.4, 6.8])
ax = plt.subplot(222)
plt.title('dt (#'+nprocessos + " processos)" ) 
plt.boxplot(dt_media,showmeans=True,meanline=True)
plt.ylabel('Tempo (clock)',fontsize=14)
ax.set_xticklabels(['30 execucoes '],fontsize=14)
plt.grid(True)
ax = plt.subplot(223)
plt.title('tf (#'+nprocessos + " processos)" ) 
plt.boxplot(tf_media,showmeans=True,meanline=True)
plt.ylabel('Tempo (clock)',fontsize=14)
ax.set_xticklabels(['30 execucoes '],fontsize=14)
plt.grid(True)
ax = plt.subplot(224)
plt.title('tr (#'+nprocessos + " processos)" ) 
plt.boxplot(tr_media,showmeans=True,meanline=True)
plt.ylabel('Tempo (clock)',fontsize=14)
ax.set_xticklabels(['30 execucoes '],fontsize=14)
plt.grid(True)

# save fig
plt.savefig(fout,format='png', bbox_inches=0)
# remove margens brancas 
os.system('convert -trim ' + fout + ' ' + fout)
plt.show()
