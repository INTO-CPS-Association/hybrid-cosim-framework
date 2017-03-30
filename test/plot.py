from pandas import DataFrame, read_csv
import matplotlib.pyplot as plt
import pandas as pd
# define data location
df = read_csv('result.csv')
df2 = read_csv('result-sa.csv')

fig, axarr = plt.subplots(2, sharex=True)

#fig = plt.figure()
ax1=axarr[0]

ax1.title.set_text('ID_Window_SA_OUT_disp')
name='ID_Window_SA_OUT_disp'
ax1.plot(df['time'], df[name],color='b', label="window")
ax1.plot(df2['time'], df2[name], '--',color='r', label="window-sa")

#name='SA'
#ax1.plot(df['time'], df[name], label=name)


ax2 = axarr[1]
ax2.title.set_text('ID_Window_SA_OUT_tau')


name='ID_Window_SA_OUT_tau'
ax2.plot(df['time'], df[name],color='b',linewidth=2, label="window")
ax2.plot(df2['time'], df2[name], '--',color='r', label="window-sa")

#name='{FMU2}.FMU2Instance.fk'
#ax2.plot(df['time'], df[name], label=name)

#name='{FMU}.FMUInstance.x1'
#ax2.plot(df['time'], df[name], label=name)

#name='{FMU}.FMUInstance.v1'
#ax2.plot(df['time'], df[name], label=name)

plt.ylim( (0.8, 1.2) )

plt.legend(bbox_to_anchor=(1, 1),
           bbox_transform=plt.gcf().transFigure)

plt.show()
